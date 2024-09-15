#include "Channel.h"
#include "Model.h"
#include "Bone.h"

CChannel::CChannel()
{
}

CChannel::~CChannel()
{

}

HRESULT CChannel::Initialize(const aiNodeAnim* pAIChannel, class CModel* pModel, _float3 vPivotPos, _bool bApplyRootRotation)
{
	m_wstrName = ConvertToWstring(pAIChannel->mNodeName.data);

	m_iBoneIndex = pModel->Get_BoneIndex(m_wstrName);

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME		KeyFrame = {};

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.Time = pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		if (m_wstrName==L"TransN" || m_wstrName == L"ABone")
		{
			KeyFrame.vPosition.x += vPivotPos.x;
			KeyFrame.vPosition.y += vPivotPos.y;
			KeyFrame.vPosition.z += vPivotPos.z;

			if (!bApplyRootRotation)
			{
				XMVECTOR qRotation = XMQuaternionRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), XMConvertToRadians(-90.0f));
				XMFLOAT4 quaternion;
				XMStoreFloat4(&quaternion, qRotation);

				KeyFrame.vRotation.w = quaternion.w;
				KeyFrame.vRotation.x = quaternion.x;
				KeyFrame.vRotation.y = quaternion.y;
				KeyFrame.vRotation.z = quaternion.z;
			}
		}

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

CChannel* CChannel::Create(const aiNodeAnim* pAIChannel, class CModel* pModel, _float3 vPivotPos, _bool bApplyRootRotation)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel, vPivotPos, bApplyRootRotation)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CChannel::ConvertToBin(ofstream& file)
{
	_uint iNameLen = m_wstrName.length();
	file.write(reinterpret_cast<_char*>(&iNameLen), sizeof(iNameLen));
	file.write(reinterpret_cast<_char*>(m_wstrName.data()), iNameLen * sizeof(_tchar));

	file.write(reinterpret_cast<_char*>(&m_iNumKeyFrames), sizeof(m_iNumKeyFrames));
	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vPosition.x), sizeof(m_KeyFrames[i].vPosition.x));
		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vPosition.y), sizeof(m_KeyFrames[i].vPosition.y));
		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vPosition.z), sizeof(m_KeyFrames[i].vPosition.z));

		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vRotation.x), sizeof(m_KeyFrames[i].vRotation.x));
		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vRotation.y), sizeof(m_KeyFrames[i].vRotation.y));
		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vRotation.z), sizeof(m_KeyFrames[i].vRotation.z));
		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vRotation.w), sizeof(m_KeyFrames[i].vRotation.w));

		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vScale.x), sizeof(m_KeyFrames[i].vScale.x));
		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vScale.y), sizeof(m_KeyFrames[i].vScale.y));
		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].vScale.z), sizeof(m_KeyFrames[i].vScale.z));

		file.write(reinterpret_cast<_char*>(&m_KeyFrames[i].Time), sizeof(m_KeyFrames[i].Time));
	}
	file.write(reinterpret_cast<_char*>(&m_iBoneIndex), sizeof(m_iBoneIndex));
}