
#include "pch.h"
#include "FreeCam.h"

#include "EngineModule.h"
#include "CameraManager.h"
#include "InputManager.h"

#include "CameraComponent.h"

//HRESULT ToolEffect::CFreeCam::Initialize()
//{
//	/* 카메라 컴포넌트 */
//	CCameraComponent::FCameraDesc tCameraDesc;
//	m_wpCameraComponent = CreateComponent<CCameraComponent>(L"FreeCam", EComponentType::CAMERA);
//	m_wpCameraComponent.lock()->SetComponentDesc(make_shared<CCameraComponent::FCameraDesc>(tCameraDesc));
//	m_wpCameraComponent.lock()->SetOnAir();
//	// CEngineModule::GetInstance()->GetCameraManager()->SetCurCamera(L"FreeCam");
//
//	return CGameObject::Initialize();
//}

HRESULT ToolEffect::CFreeCam::BeginPlay()
{
	if (FAILED(CGameObject::BeginPlay())) { return E_FAIL; }

	return S_OK;
}

int32 ToolEffect::CFreeCam::PreTick(float _fDeltaSeconds)
{
	switch (CGameObject::PreTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

int32 ToolEffect::CFreeCam::Tick(float _fDeltaSeconds)
{
	/*
	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyPressed(DIK_S))
	{
		SetLocalPosition(Vector3(GetLocalPosition().value() - Vector3(0.f, 0.f, _fDeltaSeconds)));
	}
	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyPressed(DIK_W))
	{
		SetLocalPosition(Vector3(GetLocalPosition().value() + Vector3(0.f, 0.f, _fDeltaSeconds)));
	}
	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyPressed(DIK_A))
	{
		SetLocalPosition(Vector3(GetLocalPosition().value() - Vector3(_fDeltaSeconds, 0.f, 0.f)));
	}
	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyPressed(DIK_D))
	{
		SetLocalPosition(Vector3(GetLocalPosition().value() + Vector3(_fDeltaSeconds, 0.f, 0.f)));
	}
	*/

	switch (CGameObject::Tick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

int32 ToolEffect::CFreeCam::PostTick(float _fDeltaSeconds)
{
	switch (CGameObject::PostTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

HRESULT ToolEffect::CFreeCam::EndPlay()
{
	if (FAILED(CGameObject::EndPlay())) { return E_FAIL; }

	return S_OK;
}

//void ToolEffect::CFreeCam::Render()
//{
//	CGameObject::Render();
//}

void ToolEffect::CFreeCam::Release()
{
	CGameObject::Release();
}
