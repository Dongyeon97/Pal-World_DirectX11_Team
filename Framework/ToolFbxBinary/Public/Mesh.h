#pragma once
#include "Fbx_Exporter_Defines.h"
#include "Model.h"

/* CMesh : ���� �����ϴ� �ϳ��� �޽ø� ���� ������ �ε����� �����Ѵ�. */
BEGIN(Fbx_Exporter)

class CMesh
{
public:
	CMesh();
	virtual ~CMesh();

public:
	_uint	Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	_uint Get_NumBones() const {
		return m_iNumBones;
	}

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eType, const aiMesh* pAIMesh, CModel* pModel, _bool bFlipNormal);
	virtual HRESULT Initialize(void* pArg);
	void Reset_BoneIndices(CModel* pModel);


private:
	wstring				m_wstrName;

	_uint				m_iMaterialIndex = { 0 };

	_uint				m_iNumVertices = { 0 };
	VTXANIMMESH* m_pVertices = { nullptr };

	_uint				m_iNumIndices = { 0 };
	_uint* m_pIndices = { nullptr };

	/* �޽ú��� �̿��ϰ� �ִ� ����. */
	_uint				m_iNumBones = { 0 };
	vector<wstring>		m_BoneNames;
	vector<_uint>		m_BoneIndices;
	vector<_float4x4>	m_OffsetMatrices;

public:
	HRESULT Ready_Vertex_Buffer_NonAnim(const aiMesh* pAIMesh, _bool bFlipNormal);
	HRESULT Ready_Vertex_Buffer_Anim(const aiMesh* pAIMesh, CModel* pModel, _bool bFlipNormal);

public:
	static CMesh* Create(CModel::TYPE eType, const aiMesh* pAIMesh, CModel* pModel, _bool bFlipNormal);
	void ConvertToBin(ofstream& file);
};

END