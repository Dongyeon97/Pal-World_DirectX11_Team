#include "pch.h"
#include "ToolEffectLevel.h"

#include "EngineModule.h"
#include "ToolEffectModule.h"
#include "InputManager.h"
#include "ImguiManager.h"

#include "FreeCam.h"
#include "Camera.h"

#include "ToolAnimObject.h"

HRESULT ToolEffect::CToolEffectLevel::Initialize()
{
	FCameraDesc tCameraObjectDesc;
	m_spCamera = CCamera::Create(make_shared<FCameraDesc>(tCameraObjectDesc));
	CHECKF(m_spCamera, L"SampleLevel : Initialize Failed");
	AddGameObject(m_spCamera);

	//FToolAnimObjectDesc tObjDesc;
	//tObjDesc.m_matPivot = Matrix::CreateScale(0.0001f) * Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(180.f)));
	//tObjDesc.m_wstrGameObjectName = L"Anim Object";
	//tObjDesc.m_wstrAnimModelPath = L"../../Resource/Models/Dat/Anim/Kitsunebi.dat";

	//shared_ptr<CToolAnimObject> spAnimObj = CToolAnimObject::Create(make_shared<FToolAnimObjectDesc>(tObjDesc));

	//spAnimObj->SetActive(true);
	//AddGameObject(spAnimObj);

	m_wpInputManager = CEngineModule::GetInstance()->GetInputManager();

	return CLevel::Initialize();
}

HRESULT ToolEffect::CToolEffectLevel::BeginPlay()
{
	ENSURE(m_spCamera->SetOnAir());
	m_spCamera->LockCameraViewDirection(true);
	return CLevel::BeginPlay();
}

int32 ToolEffect::CToolEffectLevel::PreTick(float _fDeltaSeconds)
{
	LockCameraViewDir();

	return CLevel::PreTick(_fDeltaSeconds);
}

int32 ToolEffect::CToolEffectLevel::Tick(float _fDeltaSeconds)
{
	return CLevel::Tick(_fDeltaSeconds);
}

int32 ToolEffect::CToolEffectLevel::PostTick(float _fDeltaSeconds)
{
	return CLevel::PostTick(_fDeltaSeconds);
}

void ToolEffect::CToolEffectLevel::Release()
{
	CLevel::Release();
}

HRESULT ToolEffect::CToolEffectLevel::LockCameraViewDir()
{
	// Lock Camera
	if (!m_bLockCameraViewDir && m_wpInputManager.lock()->IsKeyDown(DIK_F2))
	{
		m_bLockCameraViewDir = !m_bLockCameraViewDir;
		ENSURE(m_spCamera->LockCameraViewDirection(m_bLockCameraViewDir));
	}
	else if (m_bLockCameraViewDir && m_wpInputManager.lock()->IsKeyDown(DIK_F2))
	{
		m_bLockCameraViewDir = !m_bLockCameraViewDir;
		ENSURE(m_spCamera->LockCameraViewDirection(m_bLockCameraViewDir));
	}

	return S_OK;
}
