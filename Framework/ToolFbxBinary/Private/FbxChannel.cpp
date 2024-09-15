#include "pch.h"
#include "FbxChannel.h"
#include "FbxBone.h"
#include "FbxModel.h"

std::shared_ptr<CChannel> FbxBinary::CChannel::Create(const aiNodeAnim* _pAiChannel, shared_ptr<CModel> _spModel)
{
	shared_ptr<CChannel> pInstance = make_shared<CChannel>();
	CHECKF(pInstance != nullptr, L"CChannel Create Failed");
	pInstance->Initialize(_pAiChannel, _spModel);
	return pInstance;
}

void FbxBinary::CChannel::Initialize(const aiNodeAnim* _pAiChannel, shared_ptr<CModel> _spModel)
{
	m_wstrChannelName = ConvertToWstring(_pAiChannel->mNodeName.data);
	m_iBoneIndex = _spModel->GetBoneIndex(m_wstrChannelName);

	uint32 iNumKeyFrames = max(_pAiChannel->mNumScalingKeys, _pAiChannel->mNumRotationKeys);
	iNumKeyFrames = max(iNumKeyFrames, _pAiChannel->mNumPositionKeys);

	Vector3		vScale;
	Quaternion	qRotation;
	Vector3		vPosition;
	bool		bApplyPosition = true;

	if (_spModel->GetModelPath().find(L"Player_Female") != wstring::npos && (m_wstrChannelName == L"eyes_l" || m_wstrChannelName == L"eyes_r"))
	{
		bApplyPosition = false;
	}

	for (size_t i = 0; i < iNumKeyFrames; i++)
	{
		KEYFRAME		KeyFrame = {};
		if (_pAiChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &_pAiChannel->mScalingKeys[i].mValue, sizeof(Vector3));
			KeyFrame.fTime = _pAiChannel->mScalingKeys[i].mTime;
		}
		if (_pAiChannel->mNumRotationKeys > i)
		{
			qRotation.x = _pAiChannel->mRotationKeys[i].mValue.x;
			qRotation.y = _pAiChannel->mRotationKeys[i].mValue.y;
			qRotation.z = _pAiChannel->mRotationKeys[i].mValue.z;
			qRotation.w = _pAiChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = _pAiChannel->mRotationKeys[i].mTime;
		}
		if (_pAiChannel->mNumPositionKeys > i)
		{
			memcpy(&vPosition, &_pAiChannel->mPositionKeys[i].mValue, sizeof(Vector3));
			KeyFrame.fTime = _pAiChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.qRotation = qRotation;
		KeyFrame.vPosition = bApplyPosition ? vPosition : Vector3(0, 0, 0);

		m_vecKeyFrames.push_back(KeyFrame);
	}
}

void FbxBinary::CChannel::SaveToDat(ofstream& _ofFile)
{
	int32 iChannelNameLen = m_wstrChannelName.length();
	_ofFile.write(reinterpret_cast<char*>(&iChannelNameLen), sizeof(iChannelNameLen));
	_ofFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(m_wstrChannelName.c_str())), iChannelNameLen * sizeof(wchar_t));

	_ofFile.write(reinterpret_cast<char*>(&m_iBoneIndex), sizeof(m_iBoneIndex));
	int32 iKeyFrameSize = m_vecKeyFrames.size();
	_ofFile.write(reinterpret_cast<char*>(&iKeyFrameSize), sizeof(iKeyFrameSize));
	for (size_t i = 0; i < iKeyFrameSize; ++i)
	{
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].fTime), sizeof(m_vecKeyFrames[i].fTime));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].vPosition.x), sizeof(m_vecKeyFrames[i].vPosition.x));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].vPosition.y), sizeof(m_vecKeyFrames[i].vPosition.y));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].vPosition.z), sizeof(m_vecKeyFrames[i].vPosition.z));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].qRotation.x), sizeof(m_vecKeyFrames[i].qRotation.x));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].qRotation.y), sizeof(m_vecKeyFrames[i].qRotation.y));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].qRotation.z), sizeof(m_vecKeyFrames[i].qRotation.z));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].qRotation.w), sizeof(m_vecKeyFrames[i].qRotation.w));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].vScale.x), sizeof(m_vecKeyFrames[i].vScale.x));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].vScale.y), sizeof(m_vecKeyFrames[i].vScale.y));
		_ofFile.write(reinterpret_cast<char*>(&m_vecKeyFrames[i].vScale.z), sizeof(m_vecKeyFrames[i].vScale.z));
	}
}