#include "pch.h"
#include "ToolUILoop.h"

#include "EngineModule.h"
#include "ToolUIModule.h"
#include "World.h"
#include "ToolUILevel.h"

#include "Camera.h"
#include "CameraManager.h"

HRESULT ToolUI::CToolUILoop::Initialize()
{
	// 엔진 모듈을 생성하고 초기화합니다.
	m_spEngineModule = CEngineModule::GetInstance();
	m_spEngineModule->Initialize(g_hWnd, g_hInst, g_iWindowSizeX, g_iWindowSizeY);

	m_spEngineModule->LoadResources();

	// 툴UI 모듈을 생성하고 초기화합니다.
	m_spToolUIModule = CToolUIModule::GetInstance();
	m_spToolUIModule->Initialize();

	// 월드를 생성하고 초기화합니다.
	m_spWorld = CWorld::GetInstance();
	m_spWorld->Initialize(nullptr, make_shared<CToolUILevel>(), nullptr);

	FCameraDesc fCameraDesc{};

	shared_ptr<CCamera> m_spCamera = CCamera::Create(make_shared<FCameraDesc>(fCameraDesc));
	//m_spCamera->SetOnAir();
	m_spEngineModule->GetCameraManager()->SetCurCamera(L"CameraComponent");
	m_spWorld->GetCurLevel()->AddGameObject(m_spCamera);
	//m_spToolUILevel = dynamic_pointer_cast<CToolUILevel>(m_spWorld->GetCurLevel());
	//m_spToolUILevel->Initialize();



    return S_OK;
}

HRESULT ToolUI::CToolUILoop::BeginPlay()
{
    m_spWorld->BeginPlay();
    return S_OK;
}

int32 ToolUI::CToolUILoop::Tick()
{
	float fDeltaSeconds = 0.0f;
	if (m_spEngineModule->PreTick(&fDeltaSeconds)) { return -1; }
	if (0.1f <= fDeltaSeconds) { fDeltaSeconds = 0.1f; }
	m_spToolUIModule->PreTick(fDeltaSeconds);

	m_spEngineModule->Tick(fDeltaSeconds);
	m_spToolUIModule->Tick(fDeltaSeconds);
	m_spWorld->Tick(fDeltaSeconds);

	m_spEngineModule->PostTick(fDeltaSeconds);
	m_spToolUIModule->PostTick(fDeltaSeconds);
	return 0;
}

void ToolUI::CToolUILoop::Render()
{
	m_spEngineModule->PreRender();
	m_spToolUIModule->Render();
	m_spEngineModule->PostRender();
}

void ToolUI::CToolUILoop::Release()
{
	m_spWorld->Release();
	m_spWorld = nullptr;

	m_spToolUIModule->Release();
	m_spToolUIModule = nullptr;

	m_spEngineModule->Release();
	m_spEngineModule = nullptr;
}
