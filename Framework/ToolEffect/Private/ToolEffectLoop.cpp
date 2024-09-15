#include "pch.h"
#include "ToolEffectLoop.h"

#include "EngineModule.h"
#include "ToolEffectModule.h"
#include "World.h"
#include "ImguiManager.h"
#include "PipelineManager.h"

#include "ToolEffectLevel.h"

#include "Inspector.h"

HRESULT ToolEffect::CToolEffectLoop::Initialize()
{
	// ���� ����� �����ϰ� �ʱ�ȭ�մϴ�.
	m_spEngineModule = CEngineModule::GetInstance();
	m_spEngineModule->Initialize(g_hWnd, g_hInst, g_iWindowSizeX, g_iWindowSizeY);

	// �ڵ鷯 �ε� ����
	m_spEngineModule->LoadResources();

	// ����Ʈ�� ����� �����ϰ� �ʱ�ȭ�մϴ�.
	m_spToolEffectModule = CToolEffectModule::GetInstance();
	m_spToolEffectModule->Initialize();

	// ���带 �����ϰ� �ʱ�ȭ�մϴ�.
	shared_ptr<CLevel> spLevel = make_shared<CToolEffectLevel>();
	m_spWorld = CWorld::GetInstance();
	m_spWorld->Initialize(nullptr, spLevel, nullptr);

	// �����쿡 ���� ������ �����մϴ�.
	m_spToolEffectModule->GetInstance()->GetImguiManager()->SetCurLevelToInspector(spLevel);
	m_spToolEffectModule->GetInstance()->GetImguiManager()->SetCurLevelToObjectList(spLevel);
	m_spToolEffectModule->GetInstance()->GetImguiManager()->SetCurLevelToEffectGroupWindow(spLevel);

	return S_OK;
}

HRESULT ToolEffect::CToolEffectLoop::BeginPlay()
{
	m_spWorld->BeginPlay();
	return S_OK;
}

int32 ToolEffect::CToolEffectLoop::Tick()
{
	float fDeltaSeconds = 0.0f;
	if (m_spEngineModule->PreTick(&fDeltaSeconds)) { return -1; }
	if (0.1f <= fDeltaSeconds) { fDeltaSeconds = 0.1f; }

	m_spToolEffectModule->PreTick(fDeltaSeconds);
	m_spToolEffectModule->Tick(fDeltaSeconds);
	m_spWorld->Tick(fDeltaSeconds);

	m_spEngineModule->Tick(fDeltaSeconds);
	m_spEngineModule->PostTick(fDeltaSeconds);
	m_spToolEffectModule->PostTick(fDeltaSeconds);
	return 0;
}

void ToolEffect::CToolEffectLoop::Render()
{
	m_spEngineModule->PreRender();
	m_spToolEffectModule->Render();
	m_spEngineModule->PostRender();
}

void ToolEffect::CToolEffectLoop::Release()
{
	m_spInspector->Release();
	m_spInspector = nullptr;

	m_spWorld->Release();
	m_spWorld = nullptr;

	m_spToolEffectModule->Release();
	m_spToolEffectModule = nullptr;

	m_spEngineModule->Release();
	m_spEngineModule = nullptr;
}