#pragma once
#include "Fbx_Exporter_Defines.h"

BEGIN(Fbx_Exporter)

class CModel
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

	CModel();
	virtual ~CModel();

public:
	wstring		Get_Name() const;
	_uint		Get_NumMeshes() const;
	_uint		Get_NumAnim() const;
	_int		Get_BoneIndex(const wstring wstrBoneName) const;

public:
	HRESULT Initialize_Prototype(TYPE eModelType, const wstring wstrModelFilePath, _bool bFlipNormal);

private:
	const aiScene* m_pAIScene = {};
	Assimp::Importer m_Importer;

private:
	wstring									m_wstrFilePath;
	wstring									m_wstrName;
	TYPE									m_eModelType;

	vector<class CBone*>					m_Bones;

	_uint									m_iNumMeshes = { 0 };
	vector<class CMesh*>					m_Meshes;

	_uint									m_iNumMaterials = { 0 };
	vector<wstring*>						m_Materials;

	_uint									m_iNumAnimations = { 0 };
	vector<class CAnimation*>				m_Animations;

private:
	HRESULT Ready_Bones(aiNode* pNode, _int iParentBoneIndex);
	HRESULT Ready_Meshes(_bool bFlipNormal);
	HRESULT Ready_Materials(const wstring wstrModelFilePath);
	HRESULT Ready_Animations();

public:
	static CModel* Create(TYPE eModelType, const wstring wstrModelFilePath, _bool bFlipNormal);
	void ConvertToBin();
};

END