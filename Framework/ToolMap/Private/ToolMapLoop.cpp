#include "pch.h"
#include "ToolMapLoop.h"

// Module
#include "EngineModule.h"
#include "ToolMapModule.h"

// Manager
#include "ImguiManager.h"

// World, Level
#include "World.h"
#include "ToolMapLevel.h"

HRESULT ToolMap::CToolMapLoop::Initialize()
{
	m_spEngineModule = CEngineModule::GetInstance();
	m_spEngineModule->Initialize(g_hWnd, g_hInst, g_iWindowSizeX, g_iWindowSizeY);
	m_spEngineModule->LoadResources();

	m_spToolMapModule = CToolMapModule::GetInstance();
	m_spToolMapModule->Initialize();

	// 월드를 생성하고 초기화합니다.
	m_spWorld = CWorld::GetInstance();
	m_spWorld->Initialize(nullptr,make_shared<CToolMapLevel>(),nullptr);

	return S_OK;
}

HRESULT ToolMap::CToolMapLoop::BeginPlay()
{
	m_spWorld->BeginPlay();
	m_spToolMapModule->BeginPlay();
	//m_spToolMapModule m_spWorld->GetCurLevel();
	return S_OK;
}

int32 ToolMap::CToolMapLoop::Tick()
{
	float fDeltaSeconds = 0.0f;
	if (m_spEngineModule->PreTick(&fDeltaSeconds)) { return -1; }
	if (0.1f <= fDeltaSeconds) { fDeltaSeconds = 0.1f; }
	m_spToolMapModule->PreTick(fDeltaSeconds);

	m_spEngineModule->Tick(fDeltaSeconds);
	m_spToolMapModule->Tick(fDeltaSeconds);
	m_spWorld->Tick(fDeltaSeconds);

	m_spEngineModule->PostTick(fDeltaSeconds);
	m_spToolMapModule->PostTick(fDeltaSeconds);
	return 0;
}

void ToolMap::CToolMapLoop::Render()
{
	m_spEngineModule->PreRender();
	m_spToolMapModule->Render();
	m_spEngineModule->PostRender();
}

void ToolMap::CToolMapLoop::Release()
{
	m_spWorld->Release();
	m_spWorld = nullptr;

	m_spToolMapModule->Release();
	m_spToolMapModule = nullptr;

	m_spEngineModule->Release();
	m_spEngineModule = nullptr;
}
