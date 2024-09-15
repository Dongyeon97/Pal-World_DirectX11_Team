#pragma once
#include "Fbx_Exporter_Defines.h"

BEGIN(Fbx_Exporter)

class CChannel
{
public:
	typedef struct
	{
		_float3		vScale, vPosition;
		_float4		vRotation;
		_double		Time;
	}KEYFRAME;

	CChannel();
	virtual ~CChannel();

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, class CModel* pModel, _float3 vPivotPos, _bool bApplyRootRotation);
	void Invalidate_TransformationMatrix(_double TrackPosition, _uint* pCurrentKeyFrame, const vector<class CBone*>& Bones);
	KEYFRAME Get_KeyFrame(_double TrackPosition);
	wstring Get_Name() const {
		return m_wstrName;
	}
	_uint Get_BoneIndex() const {
		return m_iBoneIndex;
	}

private:
	wstring				m_wstrName;
	_uint				m_iNumKeyFrames = { 0 };
	vector<KEYFRAME>	m_KeyFrames;
	_uint				m_iBoneIndex = { 0 };

public:
	static CChannel* Create(const aiNodeAnim* pAIChannel, class CModel* pModel, _float3 vPivotPos, _bool bApplyRootRotation);
	void ConvertToBin(ofstream& file);
};

END