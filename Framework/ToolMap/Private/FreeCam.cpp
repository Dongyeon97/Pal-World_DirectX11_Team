#include "pch.h"
#include "FreeCam.h"

#include "EngineModule.h"
#include "CameraManager.h"
#include "InputManager.h"

#include "CameraComponent.h"

HRESULT ToolMap::CFreeCam::Initialize()
{
	return CCamera::Initialize();
}

HRESULT ToolMap::CFreeCam::BeginPlay()
{
	if (FAILED(CCamera::BeginPlay())) { return E_FAIL; }

	return S_OK;
}

int32 ToolMap::CFreeCam::PreTick(float _fDeltaSeconds)
{
	switch (CCamera::PreTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return int32();
}

int32 ToolMap::CFreeCam::Tick(float _fDeltaSeconds)
{
	/*if (CEngineModule::GetInstance()->GetInputManager()->IsKeyPressed(DIK_Z))
	{
		SetLocalPosition(Vector3(0.f, 0.f, GetLocalPosition().value().z - _fDeltaSeconds));
	}
	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyPressed(DIK_X))
	{
		SetLocalPosition(Vector3(0.f, 0.f, GetLocalPosition().value().z + _fDeltaSeconds));
	}
	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyPressed(DIK_C))
	{
		SetLocalPosition(Vector3(0.f, GetLocalPosition().value().y + _fDeltaSeconds, 0.f));
	}
	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyPressed(DIK_V))
	{
		SetLocalPosition(Vector3(0.f, GetLocalPosition().value().y - _fDeltaSeconds, 0.f));
	}*/


	switch (CCamera::Tick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return S_OK;
}

int32 ToolMap::CFreeCam::PostTick(float _fDeltaSeconds)
{
	switch (CCamera::PostTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return S_OK;
}

HRESULT ToolMap::CFreeCam::EndPlay()
{
	if (FAILED(CCamera::EndPlay())) { return E_FAIL; }

	return S_OK;
}

void ToolMap::CFreeCam::Render()
{
	CCamera::Render();
}

void ToolMap::CFreeCam::Release()
{
	CCamera::Release();
}
