#include "pch.h"
#include "ToolEffectModule.h"

#include "EngineModule.h"

#include "ImguiManager.h"

IMPLEMENT_SINGLETON(CToolEffectModule)

HRESULT ToolEffect::CToolEffectModule::Initialize()
{
	m_spImguiManager = CImguiManager::Create<CImguiManager>();
	m_spImguiManager->Initialize();

	return S_OK;
}

int32 ToolEffect::CToolEffectModule::PreTick(float _fDeltaSeconds)
{
	m_spImguiManager->UpdateBegin(_fDeltaSeconds);

	return 0;
}

int32 ToolEffect::CToolEffectModule::Tick(float _fDeltaSeconds)
{
	m_spImguiManager->Update(_fDeltaSeconds);

	return 0;
}

int32 ToolEffect::CToolEffectModule::PostTick(float _fDeltaSeconds)
{
	m_spImguiManager->UpdateEnd(_fDeltaSeconds);

	return 0;
}

void ToolEffect::CToolEffectModule::Render()
{
	m_spImguiManager->Render();
}

void ToolEffect::CToolEffectModule::Release()
{
	m_spImguiManager->Release();

	m_spImguiManager = nullptr;

	m_spInstance.reset();
	m_spInstance = nullptr;
}
