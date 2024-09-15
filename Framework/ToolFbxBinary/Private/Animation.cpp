#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::~CAnimation()
{

}

HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, class CModel* pModel)
{
	m_wstrName = ConvertToWstring(pAIAnimation->mName.data);

	if (m_wstrName.find(L"_SYS") != wstring::npos)
		m_wstrName = m_wstrName.substr(m_wstrName.find(L"_SYS") + 1, m_wstrName.length() - m_wstrName.find(L"_SYS" - 1));

	if (m_wstrName.find(L"|SYS") != wstring::npos)
		m_wstrName = m_wstrName.substr(m_wstrName.find(L"|SYS") + 1, m_wstrName.length() - m_wstrName.find(L"|SYS" - 1));

	if (m_wstrName.find(L"_BTL") != wstring::npos)
		m_wstrName = m_wstrName.substr(m_wstrName.find(L"_BTL") + 1, m_wstrName.length() - m_wstrName.find(L"_BTL") - 1);

	if (m_wstrName.find(L"|BTL") != wstring::npos)
		m_wstrName = m_wstrName.substr(m_wstrName.find(L"|BTL") + 1, m_wstrName.length() - m_wstrName.find(L"|BTL") - 1);

	/* 내 애니메이션의 전체 길이. */
	m_Duration = pAIAnimation->mDuration;

	/* 초당 재생 속도 .*/
	m_TickPerSecond = 60;

	/* 이 애니메이션을 구동하는데 필요한 뼈의 갯수. */
	m_iNumChannels = pAIAnimation->mNumChannels;

	_float3 vPivotPosition(0, 0, 0);
	if (L"BTL_ATTACK_ENGETU" == m_wstrName)
	{
		vPivotPosition.y =  0.0014261319302022457f;
	}
	if (L"BTL_ATTACK_FLASH_AIR" == m_wstrName)
	{
		vPivotPosition.y =  69.3703842163086f;
	}
	if (L"BTL_ATTACK_GOUKABAKUENNZIN_END" == m_wstrName)
	{
		vPivotPosition.y =  -75.83405303955078f;
	}
	if (L"BTL_ATTACK_GOUKABAKUENNZIN_LOOP" == m_wstrName)
	{
		vPivotPosition.y =  -75.83405303955078f;
	}
	if (L"BTL_ATTACK_HITENSYOUKU_END" == m_wstrName)
	{
		vPivotPosition.y =  40.51364517211914f;
	}
	if (L"BTL_ATTACK_HITENSYOUKU_LOOP" == m_wstrName)
	{
		vPivotPosition.y =  40.513668060302734f;
	}
	if (L"BTL_ATTACK_HOUOUTENKU_END" == m_wstrName)
	{
		vPivotPosition.y =  -83.55825805664062f;
	}
	if (L"BTL_ATTACK_HOUOUTENKU_LOOP" == m_wstrName)
	{
		vPivotPosition.y =  -83.55825805664062f;
	}
	if (L"BTL_ATTACK_MAOUENGEKIHA_LOOP2" == m_wstrName)
	{
		vPivotPosition.y =  -0.000889122427906841f;
	}
	if (L"BTL_ATTACK_NEPPASENPUUZIN_LOOP" == m_wstrName)
	{
		vPivotPosition.y =  2.704205508052837e-05f;
	}
	if (L"BTL_ATTACK_NEPPASENPUUZIN_START_END" == m_wstrName)
	{
		vPivotPosition.y =  1.1533478755154647e-05f;
	}
	if (L"BTL_ATTACK_NEPPASENPUUZIN_START_LOOP" == m_wstrName)
	{
		vPivotPosition.y =  1.1533478755154647e-05f;
	}
	if (L"BTL_ATTACK_NORMAL_1" == m_wstrName)
	{
		vPivotPosition.y =  156.9373779296875f;
	}
	if (L"BTL_ATTACK_NORMAL_8" == m_wstrName)
	{
		vPivotPosition.y =  588.218017578125f;
		m_wstrName = L"BTL_ATTACK_NORMAL_2";
	}
	if (L"BTL_ATTACK_NORMAL_6" == m_wstrName)
	{
		vPivotPosition.y =  874.79052734375f;
		m_wstrName = L"BTL_ATTACK_NORMAL_3";
	}
	if (L"BTL_ATTACK_NORMAL_7" == m_wstrName)
	{
		vPivotPosition.y =  -3.6233248710632324f;
		m_wstrName = L"BTL_ATTACK_NORMAL_4";
	}
	if (L"BTL_ATTACK_NORMAL_AIR_0" == m_wstrName)
	{
		vPivotPosition.y =  0.00010924148955382407f;
	}
	if (L"BTL_ATTACK_NORMAL_AIR_1" == m_wstrName)
	{
		vPivotPosition.y =  226.6702880859375f;
	}
	if (L"BTL_ATTACK_NORMAL_AIR_2" == m_wstrName)
	{
		vPivotPosition.y =  -0.00016114753088913858f;
	}
	if (L"BTL_ATTACK_NORMAL_AIR_3" == m_wstrName)
	{
		vPivotPosition.y =  -0.0001696261460892856f;
	}
	if (L"BTL_ATTACK_NORMAL_AIR_4" == m_wstrName)
	{
		vPivotPosition.y =  -1.578689261805266e-05f;
	}
	if (L"BTL_ATTACK_SENKUKEN" == m_wstrName)
	{
		vPivotPosition.y =  -0.18960638344287872f;
	}
	if (L"BTL_ATTACK_TESSETUZAN_COUNTER" == m_wstrName)
	{
		vPivotPosition.y =  78.00516510009766f;
	}
	if (L"BTL_STEP_LAND_BACK" == m_wstrName)
	{
		vPivotPosition.y =  -45.854827880859375f;
	}

	_bool bApplyRootRotation = true;

	/* 이 애니메이션을 구동하는데 필요한 뼈들의 정보를 생성한다.*/
	/* 뼈들의 정보? : */
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pModel, vPivotPosition, bApplyRootRotation);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, class CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CAnimation::ConvertToBin(ofstream& file)
{
	_uint iNameLen = m_wstrName.length();
	file.write(reinterpret_cast<_char*>(&iNameLen), sizeof(iNameLen));
	file.write(reinterpret_cast<_char*>(m_wstrName.data()), iNameLen * sizeof(_tchar));

	file.write(reinterpret_cast<_char*>(&m_iNumAnimations), sizeof(m_iNumAnimations));
	file.write(reinterpret_cast<_char*>(&m_Duration), sizeof(m_Duration));
	file.write(reinterpret_cast<_char*>(&m_TickPerSecond), sizeof(m_TickPerSecond));
	file.write(reinterpret_cast<_char*>(&m_iNumChannels), sizeof(m_iNumChannels));
	for (auto& pChannel : m_Channels)
		pChannel->ConvertToBin(file);
}