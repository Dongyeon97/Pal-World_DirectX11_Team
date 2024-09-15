#pragma once
#include "Fbx_Exporter_Defines.h"

BEGIN(Fbx_Exporter)

class CBone 
{
public:
	CBone();
	virtual ~CBone();
	
public:
	const wstring Get_BoneName() const {
		return m_wstrName;
	}

	void Set_TranslationMatrix(_fmatrix TranslationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TranslationMatrix);
	}

public:
	HRESULT Initialize(const aiNode* pAIBone, _int iParentBoneIndex, _fmatrix PivotMatrix);

private:
	wstring			m_wstrName;
	_float4x4		m_TransformationMatrix; 
	_int			m_iParentBoneIndex = { 0 };


public:
	static CBone* Create(const aiNode* pAIBone, _int iParentBoneIndex, _fmatrix PivotMatrix);
	void ConvertToBin(ofstream& file);
};

END