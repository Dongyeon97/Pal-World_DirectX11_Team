#include "Bone.h"

CBone::CBone()
{
}
CBone::~CBone()
{

}

HRESULT CBone::Initialize(const aiNode* pAIBone, _int iParentBoneIndex, _fmatrix PivotMatrix)
{
	m_wstrName = ConvertToWstring(pAIBone->mName.data);

	memcpy(&m_TransformationMatrix, &pAIBone->mTransformation, sizeof(_float4x4));

	if (-1 != iParentBoneIndex)
		XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	else
		XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)) * PivotMatrix);

	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}


CBone* CBone::Create(const aiNode* pAIBone, _int iParentBoneIndex, _fmatrix PivotMatrix)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAIBone, iParentBoneIndex, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CBone::ConvertToBin(ofstream& file)
{
	_uint iNameLen = m_wstrName.length();
	file.write(reinterpret_cast<_char*>(&iNameLen), sizeof(iNameLen));
	file.write(reinterpret_cast<_char*>(m_wstrName.data()), iNameLen * sizeof(_tchar));

	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			file.write(reinterpret_cast<_char*>(&m_TransformationMatrix.m[i][j]), sizeof(m_TransformationMatrix.m[i][j]));
		}
	}

	file.write(reinterpret_cast<_char*>(&m_iParentBoneIndex), sizeof(m_iParentBoneIndex));
}
