#include "Mesh.h"
#include "Bone.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	Safe_Delete_Array(m_pVertices);
	Safe_Delete_Array(m_pIndices);
}

void CMesh::Reset_BoneIndices(CModel* pModel)
{
	if (!m_BoneIndices.empty())
		m_BoneIndices.clear();

	for (auto& wstrBoneName : m_BoneNames)
		m_BoneIndices.push_back(pModel->Get_BoneIndex(wstrBoneName));
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eType, const aiMesh* pAIMesh, CModel* pModel, _bool bFlipNormal)
{
	m_wstrName = ConvertToWstring(pAIMesh->mName.data);
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumIndices = pAIMesh->mNumFaces * 3;

#pragma region VERTEX_BUFFER

	HRESULT		hr = eType == CModel::TYPE_NONANIM ? Ready_Vertex_Buffer_NonAnim(pAIMesh, bFlipNormal) : Ready_Vertex_Buffer_Anim(pAIMesh, pModel, bFlipNormal);
	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEX_BUFFER
	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };
	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		m_pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
		m_pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
		m_pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
	}
#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMesh::Ready_Vertex_Buffer_NonAnim(const aiMesh* pAIMesh, _bool bFlipNormal)
{
	m_pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&m_pVertices[i].vPosition,
			XMVector3TransformCoord(XMLoadFloat3(&m_pVertices[i].vPosition), XMMatrixIdentity()));

		memcpy(&m_pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&m_pVertices[i].vNormal,
			XMVector3TransformNormal(XMLoadFloat3(&m_pVertices[i].vNormal), XMMatrixIdentity() * (bFlipNormal ? -1 : 1)));

		memcpy(&m_pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&m_pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}
	return S_OK;
}

HRESULT CMesh::Ready_Vertex_Buffer_Anim(const aiMesh* pAIMesh, CModel* pModel, _bool bFlipNormal)
{
	m_pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&m_pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&m_pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&m_pVertices[i].vNormal,
			XMVector3TransformNormal(XMLoadFloat3(&m_pVertices[i].vNormal), XMMatrixIdentity() * (bFlipNormal ? -1 : 1)));
		memcpy(&m_pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&m_pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	/* 이 메시의 정점들이 상태를 받아와야하는 메시에 영향을 주는 뼈들의 전체 갯수 .*/
	m_iNumBones = pAIMesh->mNumBones;

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		_float4x4	OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_OffsetMatrices.push_back(OffsetMatrix);

		m_BoneNames.push_back(ConvertToWstring(pAIBone->mName.data));
		m_BoneIndices.push_back(pModel->Get_BoneIndex(ConvertToWstring(pAIBone->mName.data)));

		/* 이 메시에게 영향을 주는 i번째 뼈는 몇개의 정점들에게 영향을 준다. */
		for (size_t j = 0; j < pAIBone->mNumWeights; j++)
		{
			/* pAIBone->mWeights[j].mVertexId : 이 뼈가 영향을 주는 j번째 정점의 인덱스 */
			if (0.f == m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x)
			{
				m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.x = i;
				m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y)
			{
				m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.y = i;
				m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z)
			{
				m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.z = i;
				m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w)
			{
				m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.w = i;
				m_pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_BoneIndices.push_back(pModel->Get_BoneIndex(m_wstrName));

		_float4x4		OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrices.push_back(OffsetMatrix);
	}

	return S_OK;
}

CMesh* CMesh::Create(CModel::TYPE eType, const aiMesh* pAIMesh, CModel* pModel, _bool bFlipNormal)
{
	CMesh* pInstance = new CMesh();

	if (FAILED(pInstance->Initialize_Prototype(eType, pAIMesh, pModel, bFlipNormal)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CMesh::ConvertToBin(ofstream& file)
{
	_uint iNameLen = m_wstrName.length();
	file.write(reinterpret_cast<_char*>(&iNameLen), sizeof(iNameLen));
	file.write(reinterpret_cast<_char*>(m_wstrName.data()), iNameLen * sizeof(_tchar));
	file.write(reinterpret_cast<_char*>(&m_iMaterialIndex), sizeof(m_iMaterialIndex));
	file.write(reinterpret_cast<_char*>(&m_iNumVertices), sizeof(m_iNumVertices));
	file.write(reinterpret_cast<_char*>(&m_iNumIndices), sizeof(m_iNumIndices));
	file.write(reinterpret_cast<_char*>(&m_iNumBones), sizeof(m_iNumBones));



	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vPosition.x), sizeof(m_pVertices[i].vPosition.x));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vPosition.y), sizeof(m_pVertices[i].vPosition.y));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vPosition.z), sizeof(m_pVertices[i].vPosition.z));

		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vNormal.x), sizeof(m_pVertices[i].vNormal.x));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vNormal.y), sizeof(m_pVertices[i].vNormal.y));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vNormal.z), sizeof(m_pVertices[i].vNormal.z));

		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vTexcoord.x), sizeof(m_pVertices[i].vTexcoord.x));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vTexcoord.y), sizeof(m_pVertices[i].vTexcoord.y));

		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vTangent.x), sizeof(m_pVertices[i].vTangent.x));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vTangent.y), sizeof(m_pVertices[i].vTangent.y));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vTangent.z), sizeof(m_pVertices[i].vTangent.z));

		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vBlendIndices.x), sizeof(m_pVertices[i].vBlendIndices.x));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vBlendIndices.y), sizeof(m_pVertices[i].vBlendIndices.y));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vBlendIndices.z), sizeof(m_pVertices[i].vBlendIndices.z));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vBlendIndices.w), sizeof(m_pVertices[i].vBlendIndices.w));

		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vBlendWeights.x), sizeof(m_pVertices[i].vBlendWeights.x));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vBlendWeights.y), sizeof(m_pVertices[i].vBlendWeights.y));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vBlendWeights.z), sizeof(m_pVertices[i].vBlendWeights.z));
		file.write(reinterpret_cast<_char*>(&m_pVertices[i].vBlendWeights.w), sizeof(m_pVertices[i].vBlendWeights.w));
	}
	for (size_t i = 0; i < m_iNumIndices; ++i)
	{
		file.write(reinterpret_cast<_char*>(&m_pIndices[i]), sizeof(m_pIndices[i]));
	}

	for (size_t i = 0; i < m_iNumBones; ++i)
	{
		_uint iBoneNameLen = m_BoneNames[i].length();
		file.write(reinterpret_cast<_char*>(&iBoneNameLen), sizeof(iBoneNameLen));
		file.write(reinterpret_cast<_char*>(m_BoneNames[i].data()), iBoneNameLen * sizeof(_tchar));

		file.write(reinterpret_cast<_char*>(&m_BoneIndices[i]), sizeof(m_BoneIndices[i]));
		for (size_t j = 0; j < 4; ++j)
		{
			for (size_t k = 0; k < 4; ++k)
			{
				file.write(reinterpret_cast<_char*>(&m_OffsetMatrices[i].m[j][k]), sizeof(m_OffsetMatrices[i].m[j][k]));
			}
		}
	}
}