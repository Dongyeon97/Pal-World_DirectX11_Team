#pragma once
#include "Fbx_Exporter_Defines.h"

BEGIN(Fbx_Exporter)

class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

public:
	HRESULT		Initialize(const aiAnimation* pAIAnimation, class CModel* pModel);

	wstring Get_Name() const {
		return m_wstrName;
	}

private:
	wstring					m_wstrName;
	_uint					m_iNumAnimations = { 0 };
	_double					m_Duration = { 0.0 };
	_double					m_TickPerSecond = { 0.0 };

	_uint					m_iNumChannels = { 0 };
	vector<class CChannel*>	m_Channels = {};

public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, class CModel* pModel);
	void ConvertToBin(ofstream& file);
};

END