#include "pch.h"
#include "ToolMapModule.h"

// Module
#include "EngineModule.h"

// Manager
#include "ImGuiManager.h"

IMPLEMENT_SINGLETON(CToolMapModule)

HRESULT ToolMap::CToolMapModule::Initialize()
{
	m_spImGuiManager = CImGuiManager::GetInstance();
	m_spImGuiManager->Initialize();

	return S_OK;
}

HRESULT ToolMap::CToolMapModule::BeginPlay()
{
	m_spImGuiManager->BeginPlay();
	return S_OK;
}

int32 ToolMap::CToolMapModule::PreTick(float _fDeltaSeconds)
{
	return 0;
}

int32 ToolMap::CToolMapModule::Tick(float _fDeltaSeconds)
{
	m_spImGuiManager->Tick(_fDeltaSeconds);
	return 0;
}

int32 ToolMap::CToolMapModule::PostTick(float _fDeltaSeconds)
{
	return 0;
}

void ToolMap::CToolMapModule::Render()
{
	m_spImGuiManager->Render();
}

void ToolMap::CToolMapModule::Release()
{
	m_spImGuiManager->Release();
	m_spImGuiManager=nullptr;

	m_spInstance = nullptr;
}
