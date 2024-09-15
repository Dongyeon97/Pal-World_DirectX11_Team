#include "pch.h"
#include "FbxModel.h"
#include "FbxBone.h"
#include "FbxMesh.h"
#include "FbxAnimation.h"
#include <iostream>
#include <filesystem>

// RapidJSON
#include "document.h"

vector<wstring>	CModel::m_vecReservedMeshes;

shared_ptr<CModel> FbxBinary::CModel::Create(EModelType _eModelType, const wstring _wstrModelFilePath)
{
	shared_ptr<CModel> spInstance = make_shared<CModel>();

	CHECKF(spInstance != nullptr, L"CModel Create Failed");
	spInstance->Initialize(_eModelType, _wstrModelFilePath);

	return spInstance;
}

HRESULT FbxBinary::CModel::Initialize(EModelType _eModelType, const wstring _wstrModelFilePath)
{
	// 모델 경로, 모델 타입 결정
	m_wstrFilePath = m_wstrModelName = _wstrModelFilePath;
	while (true)
	{
		if (m_wstrModelName.find(L"/") == wstring::npos)
			break;

		m_wstrModelName = m_wstrModelName.substr(m_wstrModelName.find(L"/") + 1);
	}
	m_wstrModelName = m_wstrModelName.substr(0, m_wstrModelName.find(L"."));
	m_eModelType = _eModelType;

	// 로컬 정점으로 가져오기
	int32 iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_OptimizeMeshes;

	string strConvertToString = WConvertToString(_wstrModelFilePath);
	m_pAiScene = m_Importer.ReadFile(strConvertToString.c_str(), iFlag);

	CHECKF(m_pAiScene != nullptr, L"CModel Create Failed");
	CHECKF(S_OK == InitBones(m_pAiScene->mRootNode, -1), L"CModel::InitBone Failed");
	CHECKF(S_OK == InitMeshes(), L"CModel::InitMeshes Failed");
	CHECKF(S_OK == InitAnimations(), L"CModel::InitAnimation Failed");
	CHECKF(S_OK == InitObjects(m_pAiScene->mRootNode, Matrix::Identity), L"CModel::InitObjects Failed");
	CHECKF(S_OK == InitColliders(), L"CModel::InitCollision Failed");

	if (m_eModelType == EModelType::ANIM || m_eModelType == EModelType::NONANIM)
	{
		CHECKF(S_OK == InitMaterials(), L"CModel::InitMaterial Failed");
	}
	else if (m_eModelType == EModelType::MAP_LANDSCAPE || m_eModelType == EModelType::MAP_OBJECT)
	{
		CHECKF(S_OK == InitMaterials_Map(), L"CModel::InitMaterial_Map Failed");
	}
	else if (m_eModelType == EModelType::EXTERN)
	{
		CHECKF(S_OK == InitMaterials_Extern(), L"CModel::InitMaterial_Extern Failed")
	}

	return S_OK;
}

void FbxBinary::CModel::SaveToDat()
{
	m_wstrFilePath.replace(m_wstrFilePath.find(L"/Fbx/"), 5, L"/Dat/");
	m_wstrFilePath.replace(m_wstrFilePath.find(L".fbx"), 4, L".dat");

	ofstream ofFile(m_wstrFilePath, std::ios::binary);
	ofFile.write(reinterpret_cast<char*>(&m_eModelType), sizeof(m_eModelType));

	int32 iNumBones = m_vecBones.size();
	ofFile.write(reinterpret_cast<char*>(&iNumBones), sizeof(iNumBones));
	for (auto& pBone : m_vecBones)
	{
		pBone->SaveToDat(ofFile);
	}

	for (auto& pMesh : m_vecMeshBuffers)
	{
		pMesh->SaveToDat(ofFile, shared_from_this());
	}

	int32 iNumAnimations = m_umapAnimations.size();
	ofFile.write(reinterpret_cast<char*>(&iNumAnimations), sizeof(iNumAnimations));
	for (auto& Pair : m_umapAnimations)
	{
		wstring wstrAnimationName = Pair.first;

		while (true)
		{
			if (wstrAnimationName.find(L"|") == wstring::npos)
				break;

			wstrAnimationName = wstrAnimationName.substr(wstrAnimationName.find(L"|") + 1);
		}

		int32 iAnimationNameLen = wstrAnimationName.length();
		ofFile.write(reinterpret_cast<char*>(&iAnimationNameLen), sizeof(iAnimationNameLen));
		ofFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(wstrAnimationName.c_str())), iAnimationNameLen * sizeof(wchar_t));
		Pair.second->SaveToDat(ofFile);
	}

	int32 iNumObjects = m_vecObjects.size();
	ofFile.write(reinterpret_cast<char*>(&iNumObjects), sizeof(iNumObjects));
	for (auto& tObject : m_vecObjects)
	{
		int32 iObjectNameLen = tObject.wstrObjectName.length();
		ofFile.write(reinterpret_cast<char*>(&iObjectNameLen), sizeof(iObjectNameLen));
		ofFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(tObject.wstrObjectName.c_str())), iObjectNameLen * sizeof(wchar_t));

		int32 iNumObjectMeshes = tObject.vecMeshNames.size();
		ofFile.write(reinterpret_cast<char*>(&iNumObjectMeshes), sizeof(iNumObjectMeshes));
		for (int32 i = 0; i < iNumObjectMeshes; ++i)
		{
			int32 iMeshNameLen = tObject.vecMeshNames[i].length();
			ofFile.write(reinterpret_cast<char*>(&iMeshNameLen), sizeof(iMeshNameLen));
			ofFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(tObject.vecMeshNames[i].c_str())), iMeshNameLen * sizeof(wchar_t));
		}

		for (int32 i = 0; i < 4; ++i)
		{
			for (int32 j = 0; j < 4; ++j)
			{
				ofFile.write(reinterpret_cast<char*>(&tObject.matWorld.m[i][j]), sizeof(tObject.matWorld.m[i][j]));
			}
		}
	}

	int32 iNumColliders = m_vecColliders.size();
	ofFile.write(reinterpret_cast<char*>(&iNumColliders), sizeof(iNumColliders));
	for (auto& tCollider : m_vecColliders)
	{
		ofFile.write(reinterpret_cast<char*>(&tCollider.eColliderType), sizeof(tCollider.eColliderType));

		int32 iColliderNameLen = tCollider.wstrColliderName.length();
		ofFile.write(reinterpret_cast<char*>(&iColliderNameLen), sizeof(iColliderNameLen));
		ofFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(tCollider.wstrColliderName.c_str())), iColliderNameLen * sizeof(wchar_t));

		int32 iParentBoneNameLen = tCollider.wstrParentBoneName.length();
		ofFile.write(reinterpret_cast<char*>(&iParentBoneNameLen), sizeof(iParentBoneNameLen));
		ofFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(tCollider.wstrParentBoneName.c_str())), iParentBoneNameLen * sizeof(wchar_t));

		ofFile.write(reinterpret_cast<char*>(&tCollider.vCenter.x), sizeof(tCollider.vCenter.x));
		ofFile.write(reinterpret_cast<char*>(&tCollider.vCenter.y), sizeof(tCollider.vCenter.y));
		ofFile.write(reinterpret_cast<char*>(&tCollider.vCenter.z), sizeof(tCollider.vCenter.z));

		ofFile.write(reinterpret_cast<char*>(&tCollider.fRadius), sizeof(tCollider.fRadius));

		if (tCollider.eColliderType == EColliderType::CAPSULE)
		{
			ofFile.write(reinterpret_cast<char*>(&tCollider.vRotationYPR.x), sizeof(tCollider.vRotationYPR.x));
			ofFile.write(reinterpret_cast<char*>(&tCollider.vRotationYPR.y), sizeof(tCollider.vRotationYPR.y));
			ofFile.write(reinterpret_cast<char*>(&tCollider.vRotationYPR.z), sizeof(tCollider.vRotationYPR.z));

			ofFile.write(reinterpret_cast<char*>(&tCollider.fLength), sizeof(tCollider.fLength));
		}
	}

	ofFile.close();
}

int32 FbxBinary::CModel::GetBoneIndex(const wstring& _wstrBoneName)
{
	int32 iBoneIndex = -1;

	auto iter = find_if(m_vecBones.begin(), m_vecBones.end(), [&](shared_ptr<CBone> _spBone) {
		iBoneIndex++;
		return _wstrBoneName == _spBone->GetBoneName();
		});

	if (iter == m_vecBones.end()) return -1;

	return iBoneIndex;
}

FbxBinary::CModel::MaterialInfo FbxBinary::CModel::GetMaterialInfo(int32 _iIndex)
{
	wstring wstrMaterialName = ConvertToWstring(m_pAiScene->mMaterials[_iIndex]->GetName().data);
	if (wstrMaterialName.find(L"Player_Eye") != wstring::npos)
	{
		wstrMaterialName = L"MI_Player_Eye";
	}

	for (auto& tMaterialInfo : m_vecMaterials)
	{
		if (tMaterialInfo.wstrMaterialName == wstrMaterialName)
		{
			return tMaterialInfo;
		}
	}
	MaterialInfo tDebug;
	tDebug.wstrMaterialName = L"없음";
	return tDebug;
}

HRESULT FbxBinary::CModel::InitBones(const aiNode* _pAiNode, int32 _iParentBoneIndex)
{
	shared_ptr<CBone> spBone = CBone::Create(_pAiNode, _iParentBoneIndex);
	CHECKF(spBone != nullptr, L"CModel::InitMeshes Failed");

	m_vecBones.push_back(spBone);
	int32 iBoneIndex = (m_vecBones.size() - 1);
	spBone->SetIndex(iBoneIndex);
	if (_iParentBoneIndex != -1)
	{
		m_vecBones[_iParentBoneIndex]->AddChildIndex(iBoneIndex);
	}

	for (size_t i = 0; i < _pAiNode->mNumChildren; i++)
	{
		InitBones(_pAiNode->mChildren[i], iBoneIndex);
	}

	return S_OK;
}

HRESULT FbxBinary::CModel::InitMeshes()
{
	int32 iNumMeshes = m_pAiScene->mNumMeshes;
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (m_pAiScene->mMeshes[i]->mNumVertices != 0)
		{
			shared_ptr<CMeshBuffer> spMesh = CMeshBuffer::Create(m_eModelType, m_pAiScene->mMeshes[i], shared_from_this());
			CHECKF(spMesh != nullptr, L"CModel::InitMeshes Failed");
			m_vecMeshBuffers.push_back(spMesh);
		}
	}
	return S_OK;
}

HRESULT FbxBinary::CModel::InitAnimations()
{
	int32 iNumAnimations = m_pAiScene->mNumAnimations;
	for (size_t i = 0; i < iNumAnimations; i++)
	{
		shared_ptr<CAnimation> spAnimation = CAnimation::Create(m_pAiScene->mAnimations[i], shared_from_this());
		CHECKF(spAnimation != nullptr, L"CModel::InitAnimation Failed");
		m_umapAnimations.emplace(ConvertToWstring(m_pAiScene->mAnimations[i]->mName.data), spAnimation);
	}

	return S_OK;
}

HRESULT FbxBinary::CModel::InitObjects(const aiNode* _pAiNode, Matrix _matParent)
{
	if (_pAiNode->mNumMeshes > 0)
	{
		ObjectInfo tObjectInfo;

		// 오브젝트 이름
		tObjectInfo.wstrObjectName = ConvertToWstring(_pAiNode->mName.data);
		if (tObjectInfo.wstrObjectName.find(L".") != wstring::npos)
		{
			tObjectInfo.wstrObjectName = tObjectInfo.wstrObjectName.substr(0, tObjectInfo.wstrObjectName.find(L"."));
		}

		// 오브젝트 메쉬
		int32 iNumMeshes = _pAiNode->mNumMeshes;
		for (int32 i = 0; i < iNumMeshes; ++i)
		{
			tObjectInfo.vecMeshNames.push_back(m_vecMeshBuffers[_pAiNode->mMeshes[i]]->GetMeshName());
		}

		// 오브젝트 월드 행렬
		memcpy(&tObjectInfo.matWorld, &_pAiNode->mTransformation, sizeof(Matrix));
		tObjectInfo.matWorld = tObjectInfo.matWorld.Transpose();

		if (_pAiNode->mParent != NULL)
		{
			Matrix matParent;
			memcpy(&matParent, &_pAiNode->mParent->mTransformation, sizeof(Matrix));
			matParent = matParent.Transpose();
			tObjectInfo.matWorld = tObjectInfo.matWorld * matParent;
		}

		m_vecObjects.push_back(tObjectInfo);
	}

	int32 iNumChildren = _pAiNode->mNumChildren;
	for (int32 i = 0; i < iNumChildren; ++i)
	{
		InitObjects(_pAiNode->mChildren[i], _matParent);
	}
	return S_OK;
}

// 파일을 열어서 JSON을 파싱하는 함수
rapidjson::Document ParseJSONFromFile(const std::string& _strFilename) {
	std::ifstream _ifstream(_strFilename);
	if (!_ifstream.is_open()) {
		std::cerr << "Failed to open file: " << _strFilename << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string strJsonContent((std::istreambuf_iterator<char>(_ifstream)), (std::istreambuf_iterator<char>()));
	rapidjson::Document docDoc;
	docDoc.Parse(strJsonContent.c_str());

	if (docDoc.HasParseError()) {
		std::cerr << "JSON parse error: " << docDoc.GetParseError() << std::endl;
		exit(EXIT_FAILURE);
	}

	return docDoc;
}

HRESULT FbxBinary::CModel::InitColliders()
{
	if (std::filesystem::exists(L"../../Resource/Models/PhysicsAssets/" + m_wstrModelName + L".json"))
	{
		// JSON 파일을 파싱하여 문서로 가져옴
		string strJsonPath = WConvertToString(L"../../Resource/Models/PhysicsAssets/" + m_wstrModelName + L".json");
		rapidjson::Document tDoc = ParseJSONFromFile(strJsonPath);

		auto DocArray = tDoc.GetArray();
		for (auto& tValue : DocArray)
		{
			if (tValue.HasMember("Properties"))
			{
				auto& tProperties = tValue.FindMember("Properties")->value;
				if (tProperties.HasMember("AggGeom"))
				{
					auto& tAggGeom = tProperties.FindMember("AggGeom")->value;

					ColliderInfo tColliderInfo;

					if (tAggGeom.HasMember("SphereElems"))
					{
						tColliderInfo.eColliderType = EColliderType::SPHERE;

						auto tElems = tAggGeom.FindMember("SphereElems")->value.GetArray();
						tColliderInfo.vCenter.x = tElems[0].FindMember("Center")->value.FindMember("X")->value.GetFloat();
						tColliderInfo.vCenter.y = tElems[0].FindMember("Center")->value.FindMember("Y")->value.GetFloat();
						tColliderInfo.vCenter.z = tElems[0].FindMember("Center")->value.FindMember("Z")->value.GetFloat();

						tColliderInfo.fRadius = tElems[0].FindMember("Radius")->value.GetFloat();
						tColliderInfo.wstrColliderName = ConvertToWstring(tElems[0].FindMember("Name")->value.GetString());
						tColliderInfo.wstrParentBoneName = ConvertToWstring(tProperties.FindMember("BoneName")->value.GetString());
					}
					else if (tAggGeom.HasMember("SphylElems"))
					{
						tColliderInfo.eColliderType = EColliderType::CAPSULE;

						auto tElems = tAggGeom.FindMember("SphylElems")->value.GetArray();
						tColliderInfo.vCenter.x = tElems[0].FindMember("Center")->value.FindMember("X")->value.GetFloat();
						tColliderInfo.vCenter.y = tElems[0].FindMember("Center")->value.FindMember("Y")->value.GetFloat();
						tColliderInfo.vCenter.z = tElems[0].FindMember("Center")->value.FindMember("Z")->value.GetFloat();

						tColliderInfo.vRotationYPR.x = tElems[0].FindMember("Rotation")->value.FindMember("Yaw")->value.GetFloat();
						tColliderInfo.vRotationYPR.y = tElems[0].FindMember("Rotation")->value.FindMember("Pitch")->value.GetFloat();
						tColliderInfo.vRotationYPR.z = tElems[0].FindMember("Rotation")->value.FindMember("Roll")->value.GetFloat();

						tColliderInfo.fRadius = tElems[0].FindMember("Radius")->value.GetFloat();
						tColliderInfo.fLength = tElems[0].FindMember("Length")->value.GetFloat();
						tColliderInfo.wstrColliderName = ConvertToWstring(tElems[0].FindMember("Name")->value.GetString());
						tColliderInfo.wstrParentBoneName = ConvertToWstring(tProperties.FindMember("BoneName")->value.GetString());
					}

					m_vecColliders.push_back(tColliderInfo);
				}
			}
		}
	}
	return S_OK;
}

HRESULT FbxBinary::CModel::InitMaterials()
{
	// JSON 파일을 파싱하여 문서로 가져옴
	string strJsonPath = WConvertToString(L"../../Resource/Models/Properties/" + m_wstrModelName + L".json");
	rapidjson::Document tDoc = ParseJSONFromFile(strJsonPath);

	auto tDocArray = tDoc.GetArray();
	for (auto& tValue : tDocArray)
	{
		if (tValue.HasMember("Materials"))
		{
			auto tMaterials = tValue.FindMember("Materials")->value.GetArray();
			for (auto& tMaterial : tMaterials)
			{
				string strMaterialPath = tMaterial.FindMember("ObjectPath")->value.GetString();
				strMaterialPath = strMaterialPath.substr(0, strMaterialPath.find("."));

				MaterialInfo tMaterialInfo;

				if (strMaterialPath.find("Pal/Content/Pal/Model/Character/Player/Eye/") != string::npos)
				{
					tMaterialInfo.wstrMaterialName = L"MI_Player_Eye";
					tMaterialInfo.wstrShaderTag = L"Player_Eye";

					tMaterialInfo.umapTextures.insert({ L"Cornea_0",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Cornea_0.png" });
					tMaterialInfo.umapTextures.insert({ L"Cornea_1",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Cornea_1.png" });
					tMaterialInfo.umapTextures.insert({ L"Cornea_2",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Cornea_2.png" });
					tMaterialInfo.umapTextures.insert({ L"Cornea_3",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Cornea_3.png" });

					tMaterialInfo.umapTextures.insert({ L"Highlight_0",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_0.png" });
					tMaterialInfo.umapTextures.insert({ L"Highlight_1",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_1.png" });
					tMaterialInfo.umapTextures.insert({ L"Highlight_2",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_2.png" });
					tMaterialInfo.umapTextures.insert({ L"Highlight_3",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_3.png" });
					tMaterialInfo.umapTextures.insert({ L"Highlight_4",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_4.png" });
					tMaterialInfo.umapTextures.insert({ L"Highlight_5",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_5.png" });

					tMaterialInfo.umapTextures.insert({ L"Pupil_0",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Pupil_0.png" });
					tMaterialInfo.umapTextures.insert({ L"Pupil_1",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Pupil_1.png" });
					tMaterialInfo.umapTextures.insert({ L"Pupil_2",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Pupil_2.png" });
					tMaterialInfo.umapTextures.insert({ L"Pupil_3",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Pupil_3.png" });

					m_vecMaterials.push_back(tMaterialInfo);
					continue;
				}
				rapidjson::Document tMaterialDoc = ParseJSONFromFile("../../Resource/Models/Materials/" + strMaterialPath + ".json");


				tMaterialInfo.wstrMaterialName = ConvertToWstring(strMaterialPath);
				while (true)
				{
					if (tMaterialInfo.wstrMaterialName.find(L"/") == wstring::npos)
						break;

					tMaterialInfo.wstrMaterialName = tMaterialInfo.wstrMaterialName.substr(tMaterialInfo.wstrMaterialName.find(L"/") + 1);
				}

				if (tMaterialDoc.HasMember("Textures"))
				{
					auto tTextures = tMaterialDoc.GetObj().FindMember("Textures")->value.GetObj();
					for (auto& tTexture : tTextures)
					{
						wstring wstrTextureUsage = ConvertToWstring(tTexture.name.GetString());
						wstring wstrTexturePath = ConvertToWstring(tTexture.value.GetString()) + L".png";
						tMaterialInfo.umapTextures.insert({ wstrTextureUsage,wstrTexturePath });
					}
				}
				else
				{
					int32 a = 0;
				}

				auto tParameters = tMaterialDoc.GetObj().FindMember("Parameters")->value.GetObj();
				if (tParameters.HasMember("Scalars"))
				{
					auto tScalars = tParameters.FindMember("Scalars")->value.GetObj();
					for (auto& tScalar : tScalars)
					{
						wstring wstrScalarUsage = ConvertToWstring(tScalar.name.GetString());
						float fScalarValue = tScalar.value.GetFloat();
						tMaterialInfo.umapScalars.insert({ wstrScalarUsage, fScalarValue });
					}
				}
				else
				{
					int32 a = 0;
				}

				if (tParameters.HasMember("Properties"))
				{
					auto tProperties = tParameters.FindMember("Properties")->value.GetObj();

					if (tProperties.HasMember("BasePropertyOverrides"))
					{
						auto tBasePropertyOverrides = tProperties.FindMember("BasePropertyOverrides")->value.GetObj();
						if (tBasePropertyOverrides.HasMember("ShadingModel"))
						{
							tMaterialInfo.wstrShaderTag = ConvertToWstring(tBasePropertyOverrides.FindMember("ShadingModel")->value.GetString());
							tMaterialInfo.wstrShaderTag = tMaterialInfo.wstrShaderTag.substr(tMaterialInfo.wstrShaderTag.find(L"::") + 2);
						}
						else
						{
							tMaterialInfo.wstrShaderTag = L"Default";
						}
					}
				}
				else
				{
					int32 a = 0;
				}

				m_vecMaterials.push_back(tMaterialInfo);
			}
			return S_OK;
		}
		else if (tValue.HasMember("Properties") && tValue.FindMember("Properties")->value.HasMember("StaticMaterials"))
		{
			auto tStaticMaterials = tValue.FindMember("Properties")->value.FindMember("StaticMaterials")->value.GetArray();
			for (auto& tMaterial : tStaticMaterials)
			{
				if (tMaterial.HasMember("MaterialInterface") && tMaterial.FindMember("MaterialInterface")->value.IsObject() && tMaterial.FindMember("MaterialInterface")->value.HasMember("ObjectPath"))
				{
					string strMaterialPath = tMaterial.FindMember("MaterialInterface")->value.FindMember("ObjectPath")->value.GetString();
					strMaterialPath = strMaterialPath.substr(0, strMaterialPath.find("."));

					MaterialInfo tMaterialInfo;

					if (strMaterialPath.find("Pal/Content/Pal/Model/Character/Player/Eye/") != string::npos)
					{
						tMaterialInfo.wstrMaterialName = L"MI_Player_Eye";
						tMaterialInfo.wstrShaderTag = L"Player_Eye";

						tMaterialInfo.umapTextures.insert({ L"Cornea_0",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Cornea_0.png" });
						tMaterialInfo.umapTextures.insert({ L"Cornea_1",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Cornea_1.png" });
						tMaterialInfo.umapTextures.insert({ L"Cornea_2",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Cornea_2.png" });
						tMaterialInfo.umapTextures.insert({ L"Cornea_3",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Cornea_3.png" });

						tMaterialInfo.umapTextures.insert({ L"Highlight_0",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_0.png" });
						tMaterialInfo.umapTextures.insert({ L"Highlight_1",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_1.png" });
						tMaterialInfo.umapTextures.insert({ L"Highlight_2",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_2.png" });
						tMaterialInfo.umapTextures.insert({ L"Highlight_3",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_3.png" });
						tMaterialInfo.umapTextures.insert({ L"Highlight_4",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_4.png" });
						tMaterialInfo.umapTextures.insert({ L"Highlight_5",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Highlight_5.png" });

						tMaterialInfo.umapTextures.insert({ L"Pupil_0",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Pupil_0.png" });
						tMaterialInfo.umapTextures.insert({ L"Pupil_1",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Pupil_1.png" });
						tMaterialInfo.umapTextures.insert({ L"Pupil_2",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Pupil_2.png" });
						tMaterialInfo.umapTextures.insert({ L"Pupil_3",L"../../Resource/Models/Materials/Pal/Content/Pal/Model/Character/Player/Eye/T_Player_Pupil_3.png" });

						m_vecMaterials.push_back(tMaterialInfo);
						continue;
					}

					rapidjson::Document tMaterialDoc = ParseJSONFromFile("../../Resource/Models/Materials/" + strMaterialPath + ".json");


					tMaterialInfo.wstrMaterialName = ConvertToWstring(strMaterialPath);
					while (true)
					{
						if (tMaterialInfo.wstrMaterialName.find(L"/") == wstring::npos)
							break;

						tMaterialInfo.wstrMaterialName = tMaterialInfo.wstrMaterialName.substr(tMaterialInfo.wstrMaterialName.find(L"/") + 1);
					}
					if (tMaterialDoc.HasMember("Textures"))
					{
						auto tTextures = tMaterialDoc.GetObj().FindMember("Textures")->value.GetObj();
						for (auto& tTexture : tTextures)
						{
							wstring wstrTextureUsage = ConvertToWstring(tTexture.name.GetString());
							wstring wstrTexturePath = ConvertToWstring(tTexture.value.GetString()) + L".png";
							tMaterialInfo.umapTextures.insert({ wstrTextureUsage,wstrTexturePath });
						}
					}
					else
					{
						int32 a = 0;
					}

					auto tParameters = tMaterialDoc.GetObj().FindMember("Parameters")->value.GetObj();
					if (tParameters.HasMember("Scalars"))
					{
						auto tScalars = tParameters.FindMember("Scalars")->value.GetObj();
						for (auto& tScalar : tScalars)
						{
							wstring wstrScalarUsage = ConvertToWstring(tScalar.name.GetString());
							float fScalarValue = tScalar.value.GetFloat();
							tMaterialInfo.umapScalars.insert({ wstrScalarUsage, fScalarValue });
						}
					}
					else
					{
						int32 a = 0;
					}

					if (tParameters.HasMember("Properties"))
					{
						auto tProperties = tParameters.FindMember("Properties")->value.GetObj();

						if (tProperties.HasMember("BasePropertyOverrides"))
						{
							auto tBasePropertyOverrides = tProperties.FindMember("BasePropertyOverrides")->value.GetObj();
							if (tBasePropertyOverrides.HasMember("ShadingModel"))
							{
								tMaterialInfo.wstrShaderTag = ConvertToWstring(tBasePropertyOverrides.FindMember("ShadingModel")->value.GetString());
								tMaterialInfo.wstrShaderTag = tMaterialInfo.wstrShaderTag.substr(tMaterialInfo.wstrShaderTag.find(L"::") + 2);
							}
							else
							{
								tMaterialInfo.wstrShaderTag = L"Default";
							}
						}
						else
						{
							tMaterialInfo.wstrShaderTag = L"Default";
						}
					}
					else
					{
						int32 a = 0;
					}

					m_vecMaterials.push_back(tMaterialInfo);
				}
			}
			return S_OK;
		}
	}

	return S_OK;
}

HRESULT FbxBinary::CModel::InitMaterials_Map()
{
	// JSON 파일을 파싱하여 문서로 가져옴
	string strJsonPath = WConvertToString(L"../../Resource/Models/Properties/" + m_wstrModelName + L".json");
	rapidjson::Document tDoc = ParseJSONFromFile(strJsonPath);

	auto tDocObject = tDoc.GetObj();
	for (auto& tValue : tDocObject)
	{
		MaterialInfo tMaterialInfo;
		tMaterialInfo.wstrMaterialName = ConvertToWstring(tValue.name.GetString());

		if (m_eModelType == EModelType::MAP_OBJECT)
		{
			tMaterialInfo.wstrShaderTag = ConvertToWstring(tValue.value.FindMember("shader_name")->value.GetString());

		}
		else
		{
			tMaterialInfo.wstrShaderTag = L"Landscape";
		}

		auto tTextures = tValue.value.FindMember("textures")->value.GetArray();
		for (auto& tTexture : tTextures)
		{
			wstring wstrTextureUsage = ConvertToWstring(tTexture.FindMember("usage")->value.GetString());
			wstring wstrTexturePath = L"../../Resource/Models/Materials/WorldMap/" + ConvertToWstring(tTexture.FindMember("name")->value.GetString()) + L".png";
			tMaterialInfo.umapTextures.insert({ wstrTextureUsage ,wstrTexturePath });
		}
		m_vecMaterials.push_back(tMaterialInfo);
	}

	if (m_eModelType == EModelType::MAP_LANDSCAPE)
	{
		wstring wstrLandscapeID = m_wstrFilePath.substr(m_wstrFilePath.find(L"/Landscape_") + lstrlenW(L"/Landscape_"));
		wstrLandscapeID.replace(wstrLandscapeID.find(L".fbx"), 4, L".png");

		m_vecMaterials[0].umapTextures.insert({ L"Heightmap"					,L"../../Resource/Models/Materials/WorldMap/Heightmap_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_SlopeMaterialA"		,L"../../Resource/Models/Materials/WorldMap/LayerInfo_SlopeMaterialA_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_Cliff"				,L"../../Resource/Models/Materials/WorldMap/LayerInfo_Cliff_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_CoastBeach"			,L"../../Resource/Models/Materials/WorldMap/LayerInfo_CoastBeach_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_DirtAndRock"		,L"../../Resource/Models/Materials/WorldMap/LayerInfo_DirtAndRock_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_DirtDry"			,L"../../Resource/Models/Materials/WorldMap/LayerInfo_DirtDry_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_Foliage_Remover"	,L"../../Resource/Models/Materials/WorldMap/LayerInfo_Foliage_Remover_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_GrassA"				,L"../../Resource/Models/Materials/WorldMap/LayerInfo_GrassA_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_LongGrass"			,L"../../Resource/Models/Materials/WorldMap/LayerInfo_LongGrass_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_NaturalRoadPath"	,L"../../Resource/Models/Materials/WorldMap/LayerInfo_NaturalRoadPath_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_RiverBed"			,L"../../Resource/Models/Materials/WorldMap/LayerInfo_RiverBed_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_SandA"				,L"../../Resource/Models/Materials/WorldMap/LayerInfo_SandA_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"LayerInfo_SandC"				,L"../../Resource/Models/Materials/WorldMap/LayerInfo_SandC_" + wstrLandscapeID });
		m_vecMaterials[0].umapTextures.insert({ L"Voronoi Noise"				,L"../../Resource/Models/Materials/WorldMap/T_Voronoi_Perturbed_4k.png" });
	}

	return S_OK;
}

HRESULT FbxBinary::CModel::InitMaterials_Extern()
{
	wstring wstrFileName = m_wstrFilePath;
	while (true)
	{
		if (wstrFileName.find(L"/") == wstring::npos)
			break;

		wstrFileName = wstrFileName.substr(wstrFileName.find(L"/") + 1);
	}

	wstring wstrDefaultFilePath = L"../../Resource/Models/Materials/Extern/";

	size_t iNumMaterial = m_pAiScene->mNumMaterials;
	m_vecMaterials.resize(iNumMaterial);
	for (size_t i = 0; i < iNumMaterial; ++i)
	{
		wstring wstrMaterialName = ConvertToWstring(m_pAiScene->mMaterials[i]->GetName().C_Str());

		{
			aiString aistrFilePath;
			if (0 == m_pAiScene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &aistrFilePath))
			{
				wstring wstrFilePath = ConvertToWstring(aistrFilePath.C_Str());
				while (true)
				{
					if (wstrFilePath.find(L"\\") == wstring::npos)
						break;

					wstrFilePath = wstrFilePath.substr(wstrFilePath.find(L"\\") + 1);
				}

				m_vecMaterials[i].umapTextures.insert({ L"Base Color",wstrDefaultFilePath + wstrFilePath });
			}
		}

		{
			aiString aistrFilePath;
			if (0 == m_pAiScene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &aistrFilePath))
			{
				wstring wstrFilePath = ConvertToWstring(aistrFilePath.C_Str());
				while (true)
				{
					if (wstrFilePath.find(L"\\") == wstring::npos)
						break;

					wstrFilePath = wstrFilePath.substr(wstrFilePath.find(L"\\") + 1);
				}
				m_vecMaterials[i].umapTextures.insert({ L"Base Normal",wstrDefaultFilePath + wstrFilePath });
			}
		}

		{
			aiString aistrFilePath;
			if (0 == m_pAiScene->mMaterials[i]->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &aistrFilePath))
			{
				wstring wstrFilePath = ConvertToWstring(aistrFilePath.C_Str());
				while (true)
				{
					if (wstrFilePath.find(L"\\") == wstring::npos)
						break;

					wstrFilePath = wstrFilePath.substr(wstrFilePath.find(L"\\") + 1);
				}
				m_vecMaterials[i].umapTextures.insert({ L"SpecularInvRoughness",wstrDefaultFilePath + wstrFilePath });
			}
		}

		//m_vecMaterials[i].wstrMaterialName = L"Extern_" + m_wstrModelName;
		m_vecMaterials[i].wstrMaterialName = wstrMaterialName;
		m_vecMaterials[i].wstrShaderTag = L"Extern";
	}
	return S_OK;
}
