#include "pch.h"
#include "ImguiManager.h"

#include "EngineModule.h"
#include "DeviceManager.h"
#include "InputManager.h"

#include "Inspector.h"
#include "ObjectList.h"
#include "EffectGroupWindow.h"

#include "ToolEffectLevel.h"

IMPLEMENT_SINGLETON(CImguiManager)

HRESULT ToolEffect::CImguiManager::Initialize()
{
	m_pDevice = CEngineModule::GetInstance()->GetDeviceManager()->GetDevice();
	m_pDeviceContext = CEngineModule::GetInstance()->GetDeviceManager()->GetDeviceContext();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// io.DisplaySize = ImVec2((float)g_iWindowSizeX, (float)g_iWindowSizeY);
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get());

	m_spInspector = make_shared<CInspector>();
	m_spInspector->Initialize();
	m_spInspector->BeginPlay();

	m_spObjectList = make_shared<CObjectList>();
	m_spObjectList->Initialize();
	m_spObjectList->BeginPlay();

	m_spEffectGroupWindow = make_shared<CEffectGroupWindow>();
	m_spEffectGroupWindow->Initialize();
	m_spEffectGroupWindow->BeginPlay();

	// ============================================================

	m_spInspector->SetObjectList(m_spObjectList);
	m_spInspector->SetEffectGroupWindow(m_spEffectGroupWindow);

	m_spObjectList->SetInspector(m_spInspector);
	m_spObjectList->SetEffectGroupWindow(m_spEffectGroupWindow);

	m_spEffectGroupWindow->SetInspector(m_spInspector);
	m_spEffectGroupWindow->SetObjectList(m_spObjectList);

	return S_OK;
}

void ToolEffect::CImguiManager::UpdateBegin(float _fDeltaSeconds)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	m_spInspector->PreTick(_fDeltaSeconds);
	m_spObjectList->PreTick(_fDeltaSeconds);
	m_spEffectGroupWindow->PreTick(_fDeltaSeconds);
}

void ToolEffect::CImguiManager::Update(float _fDeltaSeconds)
{
	m_spInspector->Tick(_fDeltaSeconds);
	m_spObjectList->Tick(_fDeltaSeconds);
	m_spEffectGroupWindow->Tick(_fDeltaSeconds);
}

void ToolEffect::CImguiManager::UpdateEnd(float _fDeltaSeconds)
{
	m_spInspector->PostTick(_fDeltaSeconds);
	m_spObjectList->PostTick(_fDeltaSeconds);
	m_spEffectGroupWindow->PostTick(_fDeltaSeconds);
}

void ToolEffect::CImguiManager::Render()
{
	m_spInspector->Render();
	m_spObjectList->Render();
	m_spEffectGroupWindow->Render();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags/* & ImGuiConfigFlags_ViewportsEnable*/)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ToolEffect::CImguiManager::Release()
{
	m_spInspector->Release();
	m_spInspector = nullptr;

	m_spObjectList->Release();
	m_spObjectList = nullptr;

	m_spEffectGroupWindow->Release();
	m_spEffectGroupWindow = nullptr;

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	m_spInstance.reset();
	m_spInstance = nullptr;
}

void ToolEffect::CImguiManager::SetCurLevelToInspector(shared_ptr<CLevel> _spLevel)
{
	CHECKF(m_spInspector, L"Failed To SetCurLevel : CImguiManager");
	m_spInspector->SetCurLevel(dynamic_pointer_cast<CToolEffectLevel>(_spLevel));
}

void ToolEffect::CImguiManager::SetCurLevelToObjectList(shared_ptr<CLevel> _spLevel)
{
	CHECKF(m_spObjectList, L"Failed To SetCurLevel : CImguiManager");
	m_spObjectList->SetCurLevel(dynamic_pointer_cast<CToolEffectLevel>(_spLevel));
}

void ToolEffect::CImguiManager::SetCurLevelToEffectGroupWindow(shared_ptr<CLevel> _spLevel)
{
	CHECKF(m_spObjectList, L"Failed To SetCurLevel : CImguiManager");
	m_spEffectGroupWindow->SetCurLevel(dynamic_pointer_cast<CToolEffectLevel>(_spLevel));
}

void ToolEffect::CImguiManager::SetMouseHold()
{
	if (CEngineModule::GetInstance()->GetInputManager()->IsKeyDown(DIK_F2))
	{
		if (!CEngineModule::GetInstance()->GetInputManager()->GetCursorLocked())
		{
			CEngineModule::GetInstance()->GetInputManager()->LockCursor(true);
		}
		else
		{
			CEngineModule::GetInstance()->GetInputManager()->LockCursor(false);
		}
	}
}
