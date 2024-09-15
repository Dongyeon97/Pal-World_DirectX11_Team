#include "pch.h"
#include "FbxBone.h"

shared_ptr<CBone> FbxBinary::CBone::Create(const aiNode* _pAiNode, int32 _iParentBoneIndex)
{
	shared_ptr<CBone> spInstance = make_shared<CBone>();

	CHECKF(spInstance != nullptr, L"CBone Create Failed");
	spInstance->Initialize(_pAiNode, _iParentBoneIndex);

	return spInstance;
}

void FbxBinary::CBone::Initialize(const aiNode* _pAiNode, int32 _iParentBoneIndex)
{
	m_wstrBoneName = ConvertToWstring(_pAiNode->mName.data);
	memcpy(&m_matTransformation, &_pAiNode->mTransformation, sizeof(Matrix));
	m_matTransformation = m_matTransformation.Transpose();
	m_iParentBoneIndex = _iParentBoneIndex;
}

void FbxBinary::CBone::SaveToDat(ofstream& _ofFile)
{
	int32 iBoneNameLen = m_wstrBoneName.length();
	_ofFile.write(reinterpret_cast<char*>(&iBoneNameLen), sizeof(iBoneNameLen));
	_ofFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(m_wstrBoneName.c_str())), iBoneNameLen * sizeof(wchar_t));

	_ofFile.write(reinterpret_cast<char*>(&m_iParentBoneIndex), sizeof(m_iParentBoneIndex));
	_ofFile.write(reinterpret_cast<char*>(&m_iMyIndex), sizeof(m_iMyIndex));

	int32 iChildBoneSize = m_lstChildBoneIndices.size();
	_ofFile.write(reinterpret_cast<char*>(&iChildBoneSize), sizeof(iChildBoneSize));
	for (auto& iChildBoneIndex : m_lstChildBoneIndices)
	{
		_ofFile.write(reinterpret_cast<char*>(&iChildBoneIndex), sizeof(iChildBoneIndex));
	}

	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			_ofFile.write(reinterpret_cast<char*>(&m_matTransformation.m[i][j]), sizeof(m_matTransformation.m[i][j]));
		}
	}
}
