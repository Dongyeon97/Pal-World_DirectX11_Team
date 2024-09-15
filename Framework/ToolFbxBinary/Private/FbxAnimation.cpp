#include "pch.h"
#include "FbxAnimation.h"
#include "FbxChannel.h"
#include "FbxBone.h"
#include "FbxModel.h"

shared_ptr<CAnimation> FbxBinary::CAnimation::Create(const aiAnimation* _pAiAnimation, shared_ptr<CModel> _spModel)
{
	shared_ptr<CAnimation> spInstance = make_shared<CAnimation>();

	CHECKF(spInstance != nullptr, L"CAnimation Create Failed");
	spInstance->Initialize(_pAiAnimation, _spModel);

	return spInstance;
}

void FbxBinary::CAnimation::Initialize(const aiAnimation* _pAiAnimation, shared_ptr<CModel> _spModel)
{
	m_wstrAnimName = ConvertToWstring(_pAiAnimation->mName.data);
	m_fDuration = _pAiAnimation->mDuration;
	m_fTickPerSec = 30.f;
	int32 iNumChannel = _pAiAnimation->mNumChannels;
	int32 iMaxKeyFrame = 0;
	for (size_t i = 0; i < iNumChannel; i++)
	{
		shared_ptr<CChannel> spChannel = CChannel::Create(_pAiAnimation->mChannels[i], _spModel);
		CHECKF(spChannel != nullptr, L"CChannel Create Failed");
		m_umapChannels.emplace(_spModel->GetBoneIndex(ConvertToWstring(_pAiAnimation->mChannels[i]->mNodeName.data)), spChannel);
	}
}

void FbxBinary::CAnimation::SaveToDat(ofstream& _ofFile)
{
	wstring wstrAnimationName = m_wstrAnimName;

	while (true)
	{
		if (wstrAnimationName.find(L"|") == wstring::npos)
			break;

		wstrAnimationName = wstrAnimationName.substr(wstrAnimationName.find(L"|") + 1);
	}

	int32 iAnimationNameLen = wstrAnimationName.length();
	_ofFile.write(reinterpret_cast<char*>(&iAnimationNameLen), sizeof(iAnimationNameLen));
	_ofFile.write(reinterpret_cast<char*>(const_cast<wchar_t*>(wstrAnimationName.c_str())), iAnimationNameLen * sizeof(wchar_t));

	_ofFile.write(reinterpret_cast<char*>(&m_fDuration), sizeof(m_fDuration));
	_ofFile.write(reinterpret_cast<char*>(&m_fTickPerSec), sizeof(m_fTickPerSec));

	int32 iNumChannel = m_umapChannels.size();
	_ofFile.write(reinterpret_cast<char*>(&iNumChannel), sizeof(iNumChannel));
	for (auto& Pair : m_umapChannels)
	{
		int32 iBoneIndex = Pair.first;
		_ofFile.write(reinterpret_cast<char*>(&iBoneIndex), sizeof(iBoneIndex));
		Pair.second->SaveToDat(_ofFile);
	}
}