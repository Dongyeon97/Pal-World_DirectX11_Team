#include "pch.h"
#include "ToolUIModule.h"

#include "IMGUIManager.h"
#include "ToolUIManager.h"

IMPLEMENT_SINGLETON(CToolUIModule)

HRESULT ToolUI::CToolUIModule::Initialize()
{
    // 매니저를 생성합니다.
    m_spIMGUIManager = CIMGUIManager::GetInstance();
    m_spToolUIManager = CToolUIManager::GetInstance();

    // 매니저를 초기화 합니다.
    m_spIMGUIManager->Initialize();
    m_spToolUIManager->Initialize();

    return S_OK;
}

int32 ToolUI::CToolUIModule::PreTick(float _fDeltaSeconds)
{
    return 0;
}

int32 ToolUI::CToolUIModule::Tick(float _fDeltaSeconds)
{
    m_spIMGUIManager->Tick(_fDeltaSeconds);
    m_spToolUIManager->Tick(_fDeltaSeconds);

    return 0;
}

int32 ToolUI::CToolUIModule::PostTick(float _fDeltaSeconds)
{
    return 0;
}

void ToolUI::CToolUIModule::Render()
{
    m_spIMGUIManager->Render();
    m_spToolUIManager->Render();
}

void ToolUI::CToolUIModule::Release()
{
    m_spIMGUIManager->Release();
    m_spIMGUIManager = nullptr;

    m_spToolUIManager->Release();
    m_spToolUIManager = nullptr;

    m_spInstance = nullptr;
}
