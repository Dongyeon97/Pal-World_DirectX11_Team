#include "pch.h"
#include "FbxMesh.h"
#include "FbxBone.h"
#include "FbxModel.h"

shared_ptr<CMeshBuffer> FbxBinary::CMeshBuffer::Create(EModelType _eModelType, const aiMesh* _pAiMesh, shared_ptr<CModel>(_pModel))
{
	shared_ptr<CMeshBuffer> spInstance = make_shared<CMeshBuffer>();

	CHECKF(spInstance != nullptr, L"CAnimation Create Failed");
	spInstance->Initialize(_eModelType, _pAiMesh, _pModel);

	return spInstance;
}

HRESULT FbxBinary::CMeshBuffer::Initialize(EModelType _eModelType, const aiMesh* _pAiMesh, shared_ptr<CModel>(_spModel))
{
	m_wstrMeshName = ConvertToWstring(_pAiMesh->mName.data);
	if (m_wstrMeshName.find(L".") != wstring::npos)
	{
		m_wstrMeshName = m_wstrMeshName.substr(0, m_wstrMeshName.find(L"."));
	}
	m_iMaterialIndex = _pAiMesh->mMaterialIndex;

	m_iNumVertices = _pAiMesh->mNumVertices;
	m_iNumIndices = _pAiMesh->mNumFaces * 3;

	m_wstrMeshName += L"_VTX" + to_wstring(m_iNumVertices);
	m_wstrMeshName += L"_IDX" + to_wstring(m_iNumIndices);


	if (!filesystem::exists(L"../../Resource/Models/Meshes/" + m_wstrMeshName + L".dat"))
	{
		for (auto& wstrReservedMesh : CModel::m_vecReservedMeshes)
		{
			if (wstrReservedMesh == m_wstrMeshName)
			{
				return S_OK;
			}
		}

		CModel::m_vecReservedMeshes.push_back(m_wstrMeshName);
		m_bNeedToCreateMeshDat = true;


#pragma region VERTEX_BUFFER
		switch (_eModelType)
		{
		case EModelType::ANIM:
			InitAnimMesh(_pAiMesh, _spModel);
			break;
		case EModelType::NONANIM:
		case EModelType::MAP_OBJECT:
		case EModelType::EXTERN:
		case EModelType::MAP_DEBUG:
			InitNonAnimMesh(_pAiMesh);
			break;
		case EModelType::MAP_LANDSCAPE:
			InitLandscapeMesh(_pAiMesh);
			break;
		case EModelType::MAP_OBJECT_COLLISION:
			InitCollisionMesh(_pAiMesh);
			break;
		}
#pragma endregion

#pragma region INDEX_BUFFER
		m_vecIndexBuffer.resize(m_iNumIndices);

		int32		iNumIndices = { 0 };
		for (size_t i = 0; i < _pAiMesh->mNumFaces; i++)
		{
			m_vecIndexBuffer[iNumIndices++] = _pAiMesh->mFaces[i].mIndices[0];
			m_vecIndexBuffer[iNumIndices++] = _pAiMesh->mFaces[i].mIndices[1];
			m_vecIndexBuffer[iNumIndices++] = _pAiMesh->mFaces[i].mIndices[2];
		}
#pragma endregion
	}

	return S_OK;
}

void FbxBinary::CMeshBuffer::SaveToDat(ofstream& _ofFile, shared_ptr<CModel>(_pModel))
{
	if (m_bNeedToCreateMeshDat)
	{
		ofstream ofMeshDatFile(L"../../Resource/Models/Meshes/" + m_wstrMeshName + L".dat", std::ios::binary);

		ofMeshDatFile.write(reinterpret_cast<char*>(&m_iNumVertices), sizeof(m_iNumVertices));
		ofMeshDatFile.write(reinterpret_cast<char*>(&m_iNumIndices), sizeof(m_iNumIndices));

		int32 iBoneSize = m_vecBoneIndices.size();
		ofMeshDatFile.write(reinterpret_cast<char*>(&iBoneSize), sizeof(iBoneSize));
		for (size_t i = 0; i < iBoneSize; ++i)
		{
			int32 iBoneNameLen = m_vecBoneNames[i].length();
			ofMeshDatFile.write(reinterpret_cast<char*>(&iBoneNameLen), sizeof(iBoneNameLen));
			ofMeshDatFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(m_vecBoneNames[i].c_str())), iBoneNameLen * sizeof(wchar_t));
			ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecBoneIndices[i]), sizeof(m_vecBoneIndices[i]));
		}

		int32 iOffsetSize = m_vecOffsetMatrices.size();
		ofMeshDatFile.write(reinterpret_cast<char*>(&iOffsetSize), sizeof(iOffsetSize));
		for (size_t i = 0; i < iBoneSize; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				for (size_t k = 0; k < 4; ++k)
				{
					ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecOffsetMatrices[i].m[j][k]), sizeof(m_vecOffsetMatrices[i].m[j][k]));
				}
			}
		}

		/* AnimModel */
		if (!m_vecAnimBurffer.empty())
		{
			for (size_t i = 0; i < m_iNumVertices; ++i)
			{
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vPosition.x), sizeof(m_vecAnimBurffer[i].m_vPosition.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vPosition.y), sizeof(m_vecAnimBurffer[i].m_vPosition.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vPosition.z), sizeof(m_vecAnimBurffer[i].m_vPosition.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vNormal.x), sizeof(m_vecAnimBurffer[i].m_vNormal.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vNormal.y), sizeof(m_vecAnimBurffer[i].m_vNormal.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vNormal.z), sizeof(m_vecAnimBurffer[i].m_vNormal.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vTexCoord2.x), sizeof(m_vecAnimBurffer[i].m_vTexCoord2.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vTexCoord2.y), sizeof(m_vecAnimBurffer[i].m_vTexCoord2.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vTangent.x), sizeof(m_vecAnimBurffer[i].m_vTangent.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vTangent.y), sizeof(m_vecAnimBurffer[i].m_vTangent.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vTangent.z), sizeof(m_vecAnimBurffer[i].m_vTangent.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vBlendIndices.x), sizeof(m_vecAnimBurffer[i].m_vBlendIndices.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vBlendIndices.y), sizeof(m_vecAnimBurffer[i].m_vBlendIndices.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vBlendIndices.z), sizeof(m_vecAnimBurffer[i].m_vBlendIndices.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vBlendIndices.w), sizeof(m_vecAnimBurffer[i].m_vBlendIndices.w));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vBlendWeights.x), sizeof(m_vecAnimBurffer[i].m_vBlendWeights.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vBlendWeights.y), sizeof(m_vecAnimBurffer[i].m_vBlendWeights.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vBlendWeights.z), sizeof(m_vecAnimBurffer[i].m_vBlendWeights.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecAnimBurffer[i].m_vBlendWeights.w), sizeof(m_vecAnimBurffer[i].m_vBlendWeights.w));
			}
		}
		/* NoneAnimModel */
		else if (!m_vecNonAnimBurffer.empty())
		{
			for (size_t i = 0; i < m_iNumVertices; ++i)
			{
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vPosition.x), sizeof(m_vecNonAnimBurffer[i].m_vPosition.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vPosition.y), sizeof(m_vecNonAnimBurffer[i].m_vPosition.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vPosition.z), sizeof(m_vecNonAnimBurffer[i].m_vPosition.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vNormal.x), sizeof(m_vecNonAnimBurffer[i].m_vNormal.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vNormal.y), sizeof(m_vecNonAnimBurffer[i].m_vNormal.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vNormal.z), sizeof(m_vecNonAnimBurffer[i].m_vNormal.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vTexCoord2.x), sizeof(m_vecNonAnimBurffer[i].m_vTexCoord2.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vTexCoord2.y), sizeof(m_vecNonAnimBurffer[i].m_vTexCoord2.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vTangent.x), sizeof(m_vecNonAnimBurffer[i].m_vTangent.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vTangent.y), sizeof(m_vecNonAnimBurffer[i].m_vTangent.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecNonAnimBurffer[i].m_vTangent.z), sizeof(m_vecNonAnimBurffer[i].m_vTangent.z));
			}
		}
		/* Landscape */
		else if (!m_vecLandscapeBurffer.empty())
		{
			for (size_t i = 0; i < m_iNumVertices; ++i)
			{
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vPosition.x), sizeof(m_vecLandscapeBurffer[i].m_vPosition.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vPosition.y), sizeof(m_vecLandscapeBurffer[i].m_vPosition.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vPosition.z), sizeof(m_vecLandscapeBurffer[i].m_vPosition.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vNormal.x), sizeof(m_vecLandscapeBurffer[i].m_vNormal.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vNormal.y), sizeof(m_vecLandscapeBurffer[i].m_vNormal.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vNormal.z), sizeof(m_vecLandscapeBurffer[i].m_vNormal.z));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vTexCoord21.x), sizeof(m_vecLandscapeBurffer[i].m_vTexCoord21.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vTexCoord21.y), sizeof(m_vecLandscapeBurffer[i].m_vTexCoord21.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vTexCoord22.x), sizeof(m_vecLandscapeBurffer[i].m_vTexCoord22.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vTexCoord22.y), sizeof(m_vecLandscapeBurffer[i].m_vTexCoord22.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vTangent.x), sizeof(m_vecLandscapeBurffer[i].m_vTangent.x));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vTangent.y), sizeof(m_vecLandscapeBurffer[i].m_vTangent.y));
				ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecLandscapeBurffer[i].m_vTangent.z), sizeof(m_vecLandscapeBurffer[i].m_vTangent.z));
			}
		}

		for (size_t i = 0; i < m_iNumIndices; ++i)
		{
			ofMeshDatFile.write(reinterpret_cast<char*>(&m_vecIndexBuffer[i]), sizeof(m_vecIndexBuffer[i]));
		}

		if (_pModel->GetNumMaterial() > 0)
		{
			CModel::MaterialInfo tMaterialInfo = _pModel->GetMaterialInfo(m_iMaterialIndex);

			if (tMaterialInfo.wstrMaterialName == L"¾øÀ½")
			{
				int32 a = 0;
			}

			int32 iMaterialNameLen = tMaterialInfo.wstrMaterialName.length();
			ofMeshDatFile.write(reinterpret_cast<char*>(&iMaterialNameLen), sizeof(iMaterialNameLen));
			ofMeshDatFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(tMaterialInfo.wstrMaterialName.c_str())), iMaterialNameLen * sizeof(wchar_t));

			int32 iShaderTagLen = tMaterialInfo.wstrShaderTag.length();
			ofMeshDatFile.write(reinterpret_cast<char*>(&iShaderTagLen), sizeof(iShaderTagLen));
			ofMeshDatFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(tMaterialInfo.wstrShaderTag.c_str())), iShaderTagLen * sizeof(wchar_t));

			int32 iNumTextures = tMaterialInfo.umapTextures.size();
			ofMeshDatFile.write(reinterpret_cast<char*>(&iNumTextures), sizeof(iNumTextures));
			for (auto& tTexture : tMaterialInfo.umapTextures)
			{
				wstring wstrTextureUsage = tTexture.first;
				wstring wstrTexturePath = tTexture.second;

				int32 iTextureUsageLen = wstrTextureUsage.length();
				ofMeshDatFile.write(reinterpret_cast<char*>(&iTextureUsageLen), sizeof(iTextureUsageLen));
				ofMeshDatFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(wstrTextureUsage.c_str())), iTextureUsageLen * sizeof(wchar_t));

				int32 iTexturePathLen = wstrTexturePath.length();
				ofMeshDatFile.write(reinterpret_cast<char*>(&iTexturePathLen), sizeof(iTexturePathLen));
				ofMeshDatFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(wstrTexturePath.c_str())), iTexturePathLen * sizeof(wchar_t));
			}

			int32 iNumScalars = tMaterialInfo.umapScalars.size();
			ofMeshDatFile.write(reinterpret_cast<char*>(&iNumScalars), sizeof(iNumScalars));
			for (auto& tScalar : tMaterialInfo.umapScalars)
			{
				wstring wstrScalarUsage = tScalar.first;
				float fScalar = tScalar.second;

				int32 iScalarUsageLen = wstrScalarUsage.length();
				ofMeshDatFile.write(reinterpret_cast<char*>(&iScalarUsageLen), sizeof(iScalarUsageLen));
				ofMeshDatFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(wstrScalarUsage.c_str())), iScalarUsageLen * sizeof(wchar_t));

				ofMeshDatFile.write(reinterpret_cast<char*>(&fScalar), sizeof(fScalar));
			}
		}

		ofMeshDatFile.close();
	}
}

void FbxBinary::CMeshBuffer::InitAnimMesh(const aiMesh* _pAiMesh, shared_ptr<CModel> _spModel)
{
	m_vecAnimBurffer.resize(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_vecAnimBurffer[i].m_vPosition, &_pAiMesh->mVertices[i], sizeof(XMFLOAT3));
		memcpy(&m_vecAnimBurffer[i].m_vNormal, &_pAiMesh->mNormals[i], sizeof(XMFLOAT3));
		memcpy(&m_vecAnimBurffer[i].m_vTexCoord2, &_pAiMesh->mTextureCoords[0][i], sizeof(XMFLOAT2));
		memcpy(&m_vecAnimBurffer[i].m_vTangent, &_pAiMesh->mTangents[i], sizeof(XMFLOAT3));
	}

	int32 iNumBones = _pAiMesh->mNumBones;

	for (size_t i = 0; i < iNumBones; i++)
	{
		aiBone* pAIBone = _pAiMesh->mBones[i];

		if (_spModel->GetBoneIndex(ConvertToWstring(pAIBone->mName.data)) != -1)
		{
			Matrix OffsetMatrix = {};

			memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(Matrix));

			OffsetMatrix = OffsetMatrix.Transpose();

			m_vecOffsetMatrices.push_back(OffsetMatrix);

			m_vecBoneNames.push_back(ConvertToWstring(pAIBone->mName.data));
			m_vecBoneIndices.push_back(_spModel->GetBoneIndex(ConvertToWstring(pAIBone->mName.data)));

			for (size_t j = 0; j < pAIBone->mNumWeights; j++)
			{
				if (pAIBone->mWeights[j].mVertexId < m_iNumVertices)
				{
					if (0.f == m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendWeights.x)
					{
						m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendIndices.x = i;
						m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendWeights.x = pAIBone->mWeights[j].mWeight;
					}
					else if (0.f == m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendWeights.y)
					{
						m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendIndices.y = i;
						m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendWeights.y = pAIBone->mWeights[j].mWeight;
					}
					else if (0.f == m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendWeights.z)
					{
						m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendIndices.z = i;
						m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendWeights.z = pAIBone->mWeights[j].mWeight;
					}
					else if (0.f == m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendWeights.w)
					{
						m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendIndices.w = i;
						m_vecAnimBurffer[pAIBone->mWeights[j].mVertexId].m_vBlendWeights.w = pAIBone->mWeights[j].mWeight;
					}
				}
			}
		}
	}

	if (0 == iNumBones)
	{
		iNumBones = 1;
		m_vecBoneNames.push_back(ConvertToWstring(_pAiMesh->mName.data));
		m_vecBoneIndices.push_back(_spModel->GetBoneIndex(ConvertToWstring(_pAiMesh->mName.data)));
		m_vecOffsetMatrices.push_back(Matrix::Identity);
	}
}

void FbxBinary::CMeshBuffer::InitNonAnimMesh(const aiMesh* _pAiMesh)
{
	m_vecNonAnimBurffer.resize(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_vecNonAnimBurffer[i].m_vPosition, &_pAiMesh->mVertices[i], sizeof(XMFLOAT3));
		memcpy(&m_vecNonAnimBurffer[i].m_vNormal, &_pAiMesh->mNormals[i], sizeof(XMFLOAT3));
		memcpy(&m_vecNonAnimBurffer[i].m_vTexCoord2, &_pAiMesh->mTextureCoords[0][i], sizeof(XMFLOAT2));
		memcpy(&m_vecNonAnimBurffer[i].m_vTangent, &_pAiMesh->mTangents[i], sizeof(XMFLOAT3));
	}
}

void FbxBinary::CMeshBuffer::InitLandscapeMesh(const aiMesh* _pAiMesh)
{
	m_vecLandscapeBurffer.resize(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_vecLandscapeBurffer[i].m_vPosition, &_pAiMesh->mVertices[i], sizeof(XMFLOAT3));
		memcpy(&m_vecLandscapeBurffer[i].m_vNormal, &_pAiMesh->mNormals[i], sizeof(XMFLOAT3));
		memcpy(&m_vecLandscapeBurffer[i].m_vTexCoord21, &_pAiMesh->mTextureCoords[0][i], sizeof(XMFLOAT2));
		memcpy(&m_vecLandscapeBurffer[i].m_vTexCoord22, &_pAiMesh->mTextureCoords[1][i], sizeof(XMFLOAT2));
		memcpy(&m_vecLandscapeBurffer[i].m_vTangent, &_pAiMesh->mTangents[i], sizeof(XMFLOAT3));
	}
}

void FbxBinary::CMeshBuffer::InitCollisionMesh(const aiMesh* _pAiMesh)
{
	m_vecNonAnimBurffer.resize(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_vecNonAnimBurffer[i].m_vPosition, &_pAiMesh->mVertices[i], sizeof(XMFLOAT3));
	}
}