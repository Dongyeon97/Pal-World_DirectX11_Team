#include "Model.h"
#include "Bone.h"
#include "Mesh.h"
#include "Animation.h"

CModel::CModel()
{
}

CModel::~CModel()
{
	for (auto& pAnimation : m_Animations)
		Safe_Delete(pAnimation);

	m_Animations.clear();

	for (auto& pBone : m_Bones)
		Safe_Delete(pBone);

	m_Bones.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Delete(pMesh);

	m_Importer.FreeScene();
}

wstring CModel::Get_Name() const
{
	return m_wstrName;
}

_uint CModel::Get_NumMeshes() const
{
	return m_iNumMeshes;
}

_uint CModel::Get_NumAnim() const
{
	return m_iNumAnimations;
}

_int CModel::Get_BoneIndex(const wstring wstrBoneName) const
{

	_int		iBoneIndex = { -1 };

	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](class CBone* pBone) {

		iBoneIndex++;

		if (wstrBoneName == pBone->Get_BoneName())
			return true;
		return false;
		});

	if (iter == m_Bones.end())
		return -1;

	return iBoneIndex;
}

HRESULT CModel::Initialize_Prototype(TYPE eModelType, const wstring wstrModelFilePath, _bool bFlipNormal)
{
	m_wstrFilePath = wstrModelFilePath;
	m_wstrName = wstrModelFilePath.substr(lstrlen(L"../../../Resource/Fbx"));
	m_wstrName = m_wstrName.substr(0, m_wstrName.find(L".fbx"));

	string strConvertToString = WConvertToString(wstrModelFilePath);

	m_eModelType = eModelType;

	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (TYPE_NONANIM == eModelType)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(strConvertToString.c_str(), iFlag);

	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(bFlipNormal)))
		return E_FAIL;

	if (FAILED(Ready_Materials(wstrModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Ready_Meshes(_bool bFlipNormal)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_eModelType, m_pAIScene->mMeshes[i], this, bFlipNormal);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const wstring wstrModelFilePath)
{
	_char			szDrive[MAX_PATH] = "";
	_char			szDirectory[MAX_PATH] = "";
	string			strConvertToString = WConvertToString(wstrModelFilePath);
	_splitpath_s(strConvertToString.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		wstring* MaterialDesc = new wstring[AI_TEXTURE_TYPE_MAX];

		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			MaterialDesc[j] = L"NULL";
			//pAIMaterial->GetTextureCount(aiTextureType(j));
			aiString			strTextureFilePath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTextureFilePath)))
				continue;

			_char				szFileName[MAX_PATH] = "";
			_char				szExt[MAX_PATH] = "";

			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			_char				szTmp[MAX_PATH] = "";
			strcpy_s(szTmp, szDrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExt);

			_tchar				szFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), szFullPath, MAX_PATH);

			MaterialDesc[j] = szFullPath;
			MaterialDesc[j].replace(MaterialDesc[j].find(L"Fbx"), lstrlen(L"Fbx"), L"Tex");
		}
		m_Materials.push_back(MaterialDesc);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(aiNode* pNode, _int iParentBoneIndex)
{
	CBone* pBone = CBone::Create(pNode, iParentBoneIndex, XMMatrixIdentity());
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	_uint	iParentIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < pNode->mNumChildren; i++)
	{
		Ready_Bones(pNode->mChildren[i], iParentIndex);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

CModel* CModel::Create(TYPE eModelType, const wstring pModelFilePath, _bool bFlipNormal)
{
	CModel* pInstance = new CModel();

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, bFlipNormal)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CModel::ConvertToBin()
{
	_char			szDrive[MAX_PATH] = "";
	_char			szDirectory[MAX_PATH] = "";
	_char			szFileName[MAX_PATH] = "";
	_char			szExt[MAX_PATH] = "";

	string			strConvertToString = WConvertToString(m_wstrFilePath);

	_splitpath_s(strConvertToString.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);

	_char szTmp[MAX_PATH] = "";
	strcpy_s(szTmp, szDrive);
	strcat_s(szTmp, szDirectory);

	wstring wstrTexOutputDir;
	wstring wstrBinOutputDir = wstrTexOutputDir = ConvertToWstring(szTmp);
	wstrBinOutputDir.replace(wstrBinOutputDir.find(L"Fbx"), lstrlen(L"Fbx"), L"Bin");
	wstrTexOutputDir.replace(wstrTexOutputDir.find(L"Fbx"), lstrlen(L"Fbx"), L"Tex");

	if (!filesystem::exists(wstrBinOutputDir)) {
		filesystem::create_directories(wstrBinOutputDir);
	}
	if (!filesystem::exists(wstrTexOutputDir)) {
		filesystem::create_directories(wstrTexOutputDir);
	}

	wstrBinOutputDir += ConvertToWstring(szFileName);
	wstrBinOutputDir += L".bin";

	ofstream file(wstrBinOutputDir, std::ios::binary);

	if (!filesystem::exists(wstrBinOutputDir)) {
		filesystem::create_directories(wstrBinOutputDir);
	}

	_uint iNameLen = m_wstrName.length();
	file.write(reinterpret_cast<_char*>(&iNameLen), sizeof(iNameLen));
	file.write(reinterpret_cast<_char*>(m_wstrName.data()), iNameLen * sizeof(_tchar));

	file.write(reinterpret_cast<_char*>(&m_eModelType), sizeof(m_eModelType));

	_uint iNumBones = m_Bones.size();
	file.write(reinterpret_cast<_char*>(&iNumBones), sizeof(iNumBones));
	for (auto& pBone : m_Bones)
		pBone->ConvertToBin(file);

	file.write(reinterpret_cast<_char*>(&m_iNumMeshes), sizeof(m_iNumMeshes));
	for (auto& pMesh : m_Meshes)
		pMesh->ConvertToBin(file);


	file.write(reinterpret_cast<_char*>(&m_iNumMaterials), sizeof(m_iNumMaterials));
	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			_uint iMaterialLen = m_Materials[i][j].length();
			file.write(reinterpret_cast<_char*>(&iMaterialLen), sizeof(iMaterialLen));
			file.write(reinterpret_cast<_char*>(m_Materials[i][j].data()), iMaterialLen * sizeof(_tchar));
		}
	}

	file.write(reinterpret_cast<_char*>(&m_iNumAnimations), sizeof(m_iNumAnimations));
	for (auto& pAnimation : m_Animations)
		pAnimation->ConvertToBin(file);


	file.close();
}