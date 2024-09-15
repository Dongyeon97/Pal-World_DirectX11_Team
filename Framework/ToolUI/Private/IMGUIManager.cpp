#include "pch.h"

#include "IMGUIManager.h"
#include "ToolUIModule.h"
#include "ToolUIManager.h"

#include "EngineModule.h"
#include "DeviceManager.h"
#include "InputManager.h"
#include "CameraManager.h"

#include "TextureHandler.h"
#include "FontHandler.h"

#include "World.h"
#include "Level.h"

#include "ToolUIPanel.h"

#include "RectComponent.h"
#include "ImageComponent.h"
#include "ButtonComponent.h"
#include "SliderComponent.h"
#include "ProgressBarComponent.h"
#include "AnimationImageComponent.h"
#include "TextComponent.h"

#include "StringUtility.h"

IMPLEMENT_SINGLETON(CIMGUIManager)

static float fCurCenter[2];
static float fPosition[3];
static float fMousePosition[2];
static float fCurScale[3];
static float fCurRatio;
static float fCurRectAngle;
static float fCurRectAlpha;
static float fCurColor[4];

// ImageAnimationFrameDesc
static float fAniPosition[3];
static float fAniScale[3];
static float fAniAngle;
static float fAniAlpha;
static float fAniTime;

static float fAniSpeed = 0.0f;

static int iEaseType = 1;

// Cur ImageAnimationFrameDesc
static float fAniCurRelativeCenter[2];
static float fAniCurPosition[3];
static float fAniCurScale[3];
static float fAniCurAngle;
static float fAniCurAlpha;
static float fAniCurTime;
static float fAniCurSpeed = 0.0f;
static Vector4 vCurColor = { 1.0f, 1.0f, 1.0f, 1.0f };

static bool bLoop;
static bool bReset;

static int iCurEaseType = 1;

static int iStartIndex = 0;
static int iEndIndex = 0;

static int iBaseTextureIndex = 0;
static int iMaskTextureIndex = 0;

static int iClickTextureIndex = 0;
static int iHoverTextureIndex = 0;

//rect component desc
static bool bMask;
static bool bAlpha;
static bool bColor;
static bool bPivot;
static bool bRelative;
static bool bInteraction;

// animation image component
static int iCurKeyFrameIndex = -1;
static int iPreKeyFrameIndex = -1;

// 버튼 클릭이미지와 슬라이더 버튼 이미지 리스트 오픈 여부를 저장합니다.
static bool bClickTexture = false;

// text component
static char szText[MAX_PATH] = "";
static wchar_t szWText[MAX_PATH] = L"";
static int iTextType = 1;

// 바꿔줄 텍스트입니다.
static char szCurText[MAX_PATH] = "";
static wchar_t szCurWText[MAX_PATH] = L"";

// image component
static int iImageType = 0;

// 부모 패널을 볼건지 자식 패널을 볼건지 선택해줍니다.
static int iPanelType = 0;
static int iCurrentPanelSelect = 0;

static int iFindComponentType = 0;

static int iCurDepth = 0;

// frame animation component
static int iRow = 0;
static int iColoumn = 0;
static float fNextTime = 0.0f;

static int iCurRow = 0;
static int iCurColoumn = 0;
static float fCurNextTime = 0.0f;

// 보간되는 프로그래스바 컴포넌트
static Vector4 vOtherColor = { 1.0f, 1.0f, 1.0f, 1.0f };
static float fOtherRatio;

static Vector4 vCurOtherColor = { 1.0f, 1.0f, 1.0f, 1.0f };
static float fCurOtherRatio;

HRESULT ToolUI::CIMGUIManager::Initialize()
{
	// Imgui를 초기화 합니다.
	if (FAILED(InitializeIMGUI())) { return E_FAIL; }

	return S_OK;
}

int32 ToolUI::CIMGUIManager::Tick(float _fDeltaSeconds)
{
	// Imgui의 New Frame과 Begin Frame을 호출합니다.
	TickIMGUI();

	//bool bDemo = true;
	//ImGui::ShowDemoWindow(&bDemo);

	CreateBaseIMGUI();

	if (nullptr != CToolUIManager::GetInstance()->GetTopPanel())
	{
		CToolUIManager::GetInstance()->UIMouseHover();
		CToolUIManager::GetInstance()->UIMouseDown();
		CToolUIManager::GetInstance()->UIMousePress();
		CToolUIManager::GetInstance()->UIMouseUp();

		if (0 == iFindComponentType)
		{
			CToolUIManager::GetInstance()->FindComponent();
			CToolUIManager::GetInstance()->MoveComponent();
			CToolUIManager::GetInstance()->ResetComponent();
		}
	}

	//if (nullptr != CToolUIManager::GetInstance()->GetCurPanel().lock())
	//{
	//	shared_ptr<CRectComponent> spRectComponent = CToolUIManager::GetInstance()->GetCurPanel().lock()->GetFindSelectRectComponent();
	//	//spRectComponent->CheckClickInRect();
	//	if (nullptr != spRectComponent)
	//	{
	//		spRectComponent->IsDragged();
	//	}
	//}


	return 0;
}

void ToolUI::CIMGUIManager::Render()
{
	// Imgui를 랜더 합니다.
	RenderIMGUI();
}

void ToolUI::CIMGUIManager::Release()
{
	// Imgui를 해제 해줍니다.
	// Imgui를 해제 해줍니다.
	ReleaseIMGUI();

	m_spInstance = nullptr;
}

HRESULT ToolUI::CIMGUIManager::InitializeIMGUI()
{
	// Imgui를 띄우기 위해, 장치를 받아옵니다.
	ComPtr<ID3D11Device> pDevice = CEngineModule::GetInstance()->GetDeviceManager()->GetDevice();
	ComPtr<ID3D11DeviceContext> pDeviceContext = CEngineModule::GetInstance()->GetDeviceManager()->GetDeviceContext();

	// Setup ImGui Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.Fonts->AddFontFromFileTTF("../../Resource/Fonts/NotoSansKR-Regular.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesKorean());
	// Setup Dear ImGui style
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	if (!ImGui_ImplWin32_Init(g_hWnd)) { return E_FAIL; }
	if (!ImGui_ImplDX11_Init(pDevice.Get(), pDeviceContext.Get())) { return E_FAIL; }
  
	return S_OK;
}

void ToolUI::CIMGUIManager::TickIMGUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ToolUI::CIMGUIManager::RenderIMGUI()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ToolUI::CIMGUIManager::ReleaseIMGUI()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void ToolUI::CIMGUIManager::CreateBaseIMGUI()
{
	static bool bOpen = true;
	 if (ImGui::Begin("UI", &bOpen))
	 {
		 if (ImGui::BeginTabBar("UI", ImGuiTabBarFlags_None))
		 {
			 if (ImGui::BeginTabItem("Add Component"))
			 {
#pragma region 패널 생성

				 static int iProjectionType = 0;
				 // 패널의 Projection Type을 지정합니다.
				 if (ImGui::RadioButton("ORTHOGONAL", &iProjectionType, 0)) {}
				 ImGui::SameLine();
				 if (ImGui::RadioButton("PERSPECTIVE", &iProjectionType, 1)) {}

				 ImGui::Dummy(ImVec2(0, 10));

				 // 패널의 이름을 입력합니다.
				 static char szPanel[MAX_PATH] = "";
				 ImGui::SetNextItemWidth(140);
				 ImGui::InputText("Panel Name", szPanel, sizeof(szPanel));
				 std::wstring wstrwide(szPanel, szPanel + MAX_PATH);

				 ImGui::Dummy(ImVec2(0, 10));

				 if (ImGui::Button("Create"))
				 {
					 //이때 같은 이름이 있는지 검사합니다.
					 weak_ptr<CToolUIPanel> wpPanel = CToolUIManager::GetInstance()->FindPanel(wstrwide);
					 if (nullptr != wpPanel.lock())
					 {
						 CHECKF(nullptr == wpPanel.lock(), L"GameObject name is same!");
						 //return;
					 }

					 //패널의 이름이 입력되었을 때 패널을 만들어줍니다.			 
					 if (strcmp(szPanel, ""))
					 {
						 //현재 레벨에 패널 게임오브젝트를 만들어줍니다.
						 shared_ptr<CToolUIPanel> spPanel = CToolUIPanel::Create(make_shared<CToolUIPanel::FUIPanelDesc>());
						 CWorld::GetInstance()->GetCurLevel()->AddGameObject(spPanel);
						 spPanel->SetUIProjectionType(static_cast<EProjectionType>(iProjectionType));
						 CToolUIManager::GetInstance()->AddPanel(wstrwide, spPanel);
						 shared_ptr<CPanel> spParentPanel = dynamic_pointer_cast<CPanel>(spPanel);
						 CToolUIManager::GetInstance()->AddParentPanel(wstrwide, spParentPanel);
					 }
				 }

				 ImGui::SameLine();

				 if (ImGui::Button("Load"))
				 {
					 CToolUIManager::GetInstance()->LoadPanel(wstrwide);
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 if (0 != CToolUIManager::GetInstance()->GetParentsUIPanels().size())
				 {
					 ImGui::Separator();

					 ImGui::Dummy(ImVec2(0, 10));
					 // 부모 패널의 리스트 박스를 보여줍니다.
					 if (ImGui::BeginListBox("Parent Panel List", ImVec2(140, 90)))
					 {
						 static int iCurParentPanelIndex = -1;
						 int32 i = 0;
						 for (auto& pair : CToolUIManager::GetInstance()->GetParentsUIPanels())
						 {
							 string strName = CStringUtility::ToString(pair.first);
							 const bool bSelect = iCurParentPanelIndex == i;
							 if (ImGui::Selectable(strName.c_str(), bSelect))
							 {
								 m_wstrSaveParentPanelName = pair.first;
								 iCurParentPanelIndex = i;
								 if (bSelect)
								 {
									 ImGui::SetItemDefaultFocus();
								 }
							 }
							 i++;
						 }
						 ImGui::EndListBox();
					 }

					 ImGui::SetNextItemWidth(140);
					 static char szChildPanel[MAX_PATH] = "";
					 // 자식 패널의 이름을 설정합니다.
					 ImGui::InputText("Child Panel Name", szChildPanel, sizeof(szChildPanel));
					 std::wstring wstrChildPanel(szChildPanel, szChildPanel + MAX_PATH);

					 if (ImGui::Button("Child Create"))
					 {
						 //// 자식 패널을 생성합니다.
						 //shared_ptr<CPanel> spParentPanel = CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName);						 //if (nullptr != wpPanel.lock())
						 //if(nullptr != spParentPanel)
						 //{
							//// ////이때 같은 이름이 있는지 검사합니다.
							//// //CHECKF(nullptr == wpPanel.lock(), L"GameObject name is same!");
							//// //return;
							// for (auto& iter : spParentPanel->GetVecChildGameObjects())
							// {
							//	 if (wstrChildPanel == iter->GetGameObjectName())
							//	 {
							//		 CHECKF(wstrChildPanel != iter->GetGameObjectName(), L"GameObject name is same!");
							//	 }
							// }
						 //}

						 //패널의 이름이 입력되었을 때 패널을 만들어줍니다.			 
						 if (strcmp(szChildPanel, ""))
						 { 
							 shared_ptr<CPanel> spChildPanel = CPanel::Create(make_shared<CPanel::FPanelDesc>());//CToolUIPanel::Create(make_shared<CToolUIPanel::FUIPanelDesc>());
							 spChildPanel->SetUIProjectionType(static_cast<EProjectionType>(iProjectionType));
							 if (nullptr != spChildPanel)
							 {
								 // 부모 패널을 찾아서 가져와줍니다.
								 shared_ptr<CPanel> spParentPanel = CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName);
								 if (nullptr != spParentPanel)
								 {
									 if (nullptr == spParentPanel->FindChildGameObject(wstrChildPanel))
									 {
										 spChildPanel->SetGameObjectName(wstrChildPanel);
										 //선택한 부모 패널의 child gameobjects에 넣어줍니다.
										 spParentPanel->AddChildGameObject(spChildPanel);
										 if (nullptr != spParentPanel->GetPivotComponent())
										 {
											 spChildPanel->SetPivotComponent(spParentPanel->GetPivotComponent());
										 }
										 //전체 패널 map에도 추가해줍니다.
										 shared_ptr<CToolUIPanel> spToolPanel = static_pointer_cast<CToolUIPanel>(spChildPanel);
										 CToolUIManager::GetInstance()->AddPanel(wstrChildPanel, spToolPanel);
									 }
									 else
									 {
										 CHECKF(nullptr == spParentPanel->FindChildGameObject(wstrChildPanel), L"GameObject name is same!");
									 }
								 }
							 }
						 }
					 }

					 ImGui::SameLine();

					 if (ImGui::Button("Save Panel"))
					 {
						 shared_ptr<CPanel> spParentPanel = CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName);
						 if (nullptr != spParentPanel)
						 {
							 CToolUIManager::GetInstance()->SavePanelsJson(spParentPanel);
						 }
					 }

					 ImGui::Dummy(ImVec2(0, 10));
				 }

#pragma endregion 패널 생성

				 ImGui::SeparatorText("Current Panel");

				 if (ImGui::BeginListBox("Panel List", ImVec2(140, 50)))
				 {
					 //shared_ptr<CPanel> spPanel = dynamic_pointer_cast<CPanel>(CToolUIManager::GetInstance()->GetCurPanel().lock());
					 static int iCurParIndex = -1;
					 int i = 0;
					 for (auto& pair : CToolUIManager::GetInstance()->GetParentsUIPanels())
					 {
						 string strName = CStringUtility::ToString(pair.first);
						 const bool bSelect = iCurParIndex == i;
						 if (ImGui::Selectable(strName.c_str(), bSelect))
						 {
							 CToolUIManager::GetInstance()->SetCurPanel(pair.first);
							 CToolUIManager::GetInstance()->SetTopPanel(static_pointer_cast<CPanel>(CToolUIManager::GetInstance()->GetCurPanel().lock()));

							 iCurParIndex = i;
							 if (bSelect)
							 {
								 ImGui::SetItemDefaultFocus();
							 }
						 }
						 i++;
					 }

					 ImGui::EndListBox();
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 if (ImGui::RadioButton("Parent", &iCurrentPanelSelect, 0))
				 {
					 CToolUIManager::GetInstance()->SelectActiveChildPanel(0 );
				 }
				 ImGui::SameLine();
				 if (ImGui::RadioButton("Child", &iCurrentPanelSelect, 1)) {}

				 if (1 == iCurrentPanelSelect)
				 {
					 if (ImGui::BeginListBox("Childs", ImVec2(140, 50)))
					 {
						 shared_ptr<CPanel> spParentPanel = static_pointer_cast<CPanel>(CToolUIManager::GetInstance()->GetCurPanel().lock());
						 if (nullptr != spParentPanel)
						 {
							 static int iCurChildPanelIndex = -1;
							 int i = 0;
							 for (auto& iter : spParentPanel->GetVecChildGameObjects())
							 {
								 string strName = CStringUtility::ToString(iter->GetGameObjectName());
								 
								 const bool bSelect = iCurChildPanelIndex == i;
								 if (ImGui::Selectable(strName.c_str(), bSelect))
								 {
									 // 자식이 선택된 경우 자식 패널을 현재 패널로 설정할 수 있도록합니다.
									 CToolUIManager::GetInstance()->SetChildPanel(iter->GetGameObjectName());

									 iCurChildPanelIndex = i;
									 if (bSelect)
									 {
										 ImGui::SetItemDefaultFocus();
									 }
								 }
								 i++;
							 }
						 }

						 ImGui::EndListBox();
					 }

					 static int iChildSelect = 0;
					 if (ImGui::RadioButton("All", &iChildSelect, 0))
					 {
						 CToolUIManager::GetInstance()->SelectActiveChildPanel(iChildSelect);
					 }
					 ImGui::SameLine();
					 if (ImGui::RadioButton("This", &iChildSelect, 1))
					 {
						 CToolUIManager::GetInstance()->SelectActiveChildPanel(iChildSelect);
					 }
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 ImGui::Separator();

				 ImGui::Dummy(ImVec2(0, 10));

				 ImGui::SetNextItemWidth(140);
				 // 적용될 렌더타입을 설정합니다
				 static int iRenderType = 6;
				 if (ImGui::Combo("RenderTpye", &iRenderType, "NONE\0RENDER_PRE\0RENDER_NBLEND\0RENDER_LBLEND\0RENDER_PBR\0RENDER_BLEND\0RENDER_UI\0RENDER_POST\0RENDER_DEBUG\0")) {}

				 ImGui::Dummy(ImVec2(0, 10));
				 m_wstrSaveShaderName = L"FX_UIOrthogonal_PS";

				 static int iPassIndex = 0;
				 static int iComboPassIndex = 0;
				 if (ImGui::Combo("Render PassIndex", &iComboPassIndex, "STATIC\0CIRCLEPROGRESS\0LINEAR_HORIZONTAL_PROGRESSBAR\00LINEAR_VERTICAL_PROGRESSBAR\0IMAGE_ANIMATION\0LINEAR_INTERPOLTIONPROGRESS\0FRAME_ANIMATION\0MASK_IMAGE\0POINT_IMAGEANIMATION\0CIRCLE_INTERPOLATIONPROGRESS\0RED_IMAGE\0"))
				 {
					 switch (iComboPassIndex)
					 {
					 case 0:
						 iPassIndex = 0;
						 break;
					 case 1:
						 iPassIndex = 2;
						 break;
					 case 2:
						 iPassIndex = 3;
						 break;
					 case 3:
						 iPassIndex = 4;
						 break;
					 case 4:
						 iPassIndex = 5;
						 break;
					 case 5:
						 iPassIndex = 6;
						 break;
					 case 6:
						 iPassIndex = 7;
						 break;
					 case 7:
						 iPassIndex = 8;
						 break;
					 case 8:
						 iPassIndex = 9;
						 break;
					 case 9:
						 iPassIndex = 10;
						 break;
					 case 10:
						 iPassIndex = 11;
						 break;
					 }
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 static int iUIType = 0;
				 if (ImGui::Combo("UIType", &iUIType, "STATIC_IMAGE\0DYNAMIC_IMAGE\0SLIDER\0BUTTON\0TEXT\0LINEAR_HORIZONTAL_PROGRESSBAR\0LINEA_VERTICAL_PROGRESSBAR\0CIRCULAR_PROGRESSBAR\0LINEAR_INTERPOLATION_PROGRESSBAR\0FRAME_IMAGE\00CIRCLE_INTERPOLATION_PROGRESSBAR\0")) {}

				 if (4 == iUIType)
				 {
					 bColor = true;
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 // 적용될 텍스처파일을 찾습니다.
				 const std::map<std::wstring, ComPtr<ID3D11ShaderResourceView>>& mapUITexture = CTextureHandler::GetInstance()->GetUITextures();

				 if (ImGui::BeginListBox("UI Texture"))
				 {
					 for (auto& pair : mapUITexture)
					 {
						 string strID = "Base" + to_string(iBaseTextureIndex);

						 if (ImGui::ImageButton(strID.c_str(), pair.second.Get(), ImVec2(35.0f, 35.0f)))
						 {
							 // 선택된 이름을 텍스처파일 이름에 저장합니다.
							 m_wstrSaveTextureName = pair.first;
						 }

						 iBaseTextureIndex++;

						 if (0 != iBaseTextureIndex % 3)
						 {
							 ImGui::SameLine();
						 }
					 }

					 iBaseTextureIndex = 0;

					 ImGui::EndListBox();
				 }
				 ImGui::Text(CStringUtility::ToString(m_wstrSaveTextureName).c_str());

				 ImGui::Dummy(ImVec2(0, 10));

				 ImGui::Checkbox("Mask", &bMask);
				 if (bMask)
				 {
					 if (ImGui::BeginListBox("Mask Texture"))
					 {
						 for (auto& pair : mapUITexture)
						 {
							 string strID = "Mask" + to_string(iMaskTextureIndex);

							 if (ImGui::ImageButton(strID.c_str(), pair.second.Get(), ImVec2(35.0f, 35.0f)))
							 {
								 // 선택된 이름을 텍스처파일 이름에 저장합니다.
								 m_wstrSaveMaskTextureName = pair.first;
							 }

							 iMaskTextureIndex++;

							 if (0 != iMaskTextureIndex % 3)
							 {
								 ImGui::SameLine();
							 }
						 }

						 iMaskTextureIndex = 0;

						 ImGui::EndListBox();
					 }
					 ImGui::Text(CStringUtility::ToString(m_wstrSaveMaskTextureName).c_str());
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 ImGui::Checkbox("Color", &bColor);
				 if(bColor)
				 {
					 static bool alpha_preview = true;
					 static bool alpha_half_preview = false;
					 static bool drag_and_drop = true;
					 static bool options_menu = true;
					 static bool hdr = false;
					 ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
					 static Vector4 myColor = { 1.0f, 1.0f, 1.0f, 1.0f };
					 if (ImGui::ColorEdit4("Color", (float*)&myColor, ImGuiColorEditFlags_Float | misc_flags))
					 {
						 m_vSaveColor = myColor;
					 }
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 ImGui::Separator();

				 ImGui::Dummy(ImVec2(0, 10));

				 if (ImGui::TreeNode("Image"))
				 {
					 // 이미지의 타입을 지정합니다.
					 if (ImGui::Combo("UIType", &iImageType, "DEFAULT\0INVENTORY\0ITEM\0")) {}

					 ImGui::TreePop();
				 }

				 if (ImGui::TreeNode("Button"))
				 {
					 bClickTexture = true;
					 if (ImGui::BeginListBox("Hover Texture"))
					 {
						 for (auto& pair : mapUITexture)
						 {
							 string strID = "Hover" + to_string(iHoverTextureIndex);

							 if (ImGui::ImageButton(strID.c_str(), pair.second.Get(), ImVec2(35.0f, 35.0f)))
							 {
								 // 선택된 이름을 텍스처파일 이름에 저장합니다.
								 m_wstrSaveTextureHoverName = pair.first;
							 }

							 iHoverTextureIndex++;

							 if (0 != iHoverTextureIndex % 3)
							 {
								 ImGui::SameLine();
							 }
						 }

						 iHoverTextureIndex = 0;

						 ImGui::EndListBox();
					 }
					 ImGui::Text(CStringUtility::ToString(m_wstrSaveTextureHoverName).c_str());
					 ImGui::Dummy(ImVec2(0, 10));
					 ImGui::TreePop();
				 }
				 else
					 bClickTexture = false;
				 
				 // Slider Button 크기를 지정합니다.
				 static float fSliderCenter[2] = {};
				 static float fSliderScale[3] = { 20.0f, 20.0f, 1.f };
				 if (ImGui::TreeNode("Slider"))
				 {
					 bClickTexture = true;
					 if (ImGui::InputFloat2("SliderCenter", fSliderCenter)) {}
					 if(ImGui::InputFloat3("SliderScale", fSliderScale)) {}

					 ImGui::Dummy(ImVec2(0, 10));
					 ImGui::TreePop();
				 }

				 if (bClickTexture)
				 {
					 if (ImGui::BeginListBox("Click Texture"))
					 {
						 for (auto& pair : mapUITexture)
						 {
							 string strID = "Click" + to_string(iClickTextureIndex);

							 if (ImGui::ImageButton(strID.c_str(), pair.second.Get(), ImVec2(35.0f, 35.0f)))
							 {
								 // 선택된 이름을 텍스처파일 이름에 저장합니다.
								 m_wstrSaveTextureClickName = pair.first;
							 }

							 iClickTextureIndex++;

							 if (0 != iClickTextureIndex % 3)
							 {
								 ImGui::SameLine();
							 }
						 }

						 iClickTextureIndex = 0;

						 ImGui::EndListBox();
					 }
					 ImGui::Text(CStringUtility::ToString(m_wstrSaveTextureClickName).c_str());
				 }
				 
				 if (ImGui::TreeNode("ProgressBar"))
				 {
					 ImGui::InputFloat("OtherRatio", &fOtherRatio);

					 static bool alpha_preview = true;
					 static bool alpha_half_preview = false;
					 static bool drag_and_drop = true;
					 static bool options_menu = true;
					 static bool hdr = false;
					 ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
					 static Vector4 myColor = { 1.0f, 1.0f, 1.0f, 1.0f };
					 if (ImGui::ColorEdit4("OtherColor", (float*)&vOtherColor, ImGuiColorEditFlags_Float | misc_flags))
					 {
					 }
					 //ImGui::Text("LinearProgressBar");
					 //ImGui::SameLine();
					 //static int iSelectXY = 0;
					 //if (ImGui::RadioButton("X", &iSelectXY, 0)) {}
					 //ImGui::SameLine();
					 //if(ImGui::RadioButton("Y", &iSelectXY, 1)) {}
				 
					 ImGui::Dummy(ImVec2(0, 10));
					 ImGui::TreePop();
				 }

				 if (ImGui::TreeNode("Image Animation"))
				 {
					 // 처음 추가하는 구조체입니다.
					 ImGui::InputFloat3("Position", fAniPosition);
					 ImGui::InputFloat3("Scale", fAniScale);
					 ImGui::InputFloat("Angle", &fAniAngle);
					 ImGui::InputFloat("Alpha", &fAniAlpha);
					 ImGui::InputFloat("Time", &fAniTime);
					 ImGui::InputFloat("Speed", &fAniSpeed);

					 ImGui::Checkbox("Loop", &bLoop);
					 if (ImGui::Combo("EaseType", &iEaseType, "EASE_NONE\0EASE_LINEAR\0EASE_IN_SINE\0EASE_OUT_SINE\0EASE_INOUT_SINE\0EASE_IN_SQUARE\0EASE_OUT_SQUARE\0EASE_INOUT_SQUARE\0EASE_IN_CIRC\0EASE_OUT_CIRC\0EASE_INOUT_CIRC\0EASE_IN_BACK\0EASE_OUT_BACK\0EASE_INOUT_BACK\0EASE_IN_ELASTIC\0EASE_OUT_ELASTIC\0EASE_INOUT_ELASTIC\0EASE_IN_BOUNCE\0EASE_OUT_BOUNCE\0EASE_INOUT_BOUNCE")) {}

					 if (9 == iUIType)
					 {
						 ImGui::InputInt("Row", &iRow);
						 ImGui::InputInt("Coloumn", &iColoumn);

						 ImGui::InputFloat("NextTime", &fNextTime);
					 }

					 ImGui::TreePop();
				 }

				 if (ImGui::TreeNode("Text"))
				 {
					 if (ImGui::Combo("Text Tpye", &iTextType, "LEFT\0CENTER\0RIGHT\0")) {}

					 ImGui::Dummy(ImVec2(0, 10));

					 static int iCurFontIndex = -1;
					 int i = 0;
					 const std::unordered_map<std::wstring, std::shared_ptr<CFont>>& mapFonts = CFontHandler::GetInstance()->GetFonts();
					 // 폰트 리스트를 보여줍니다.
					 if (ImGui::BeginListBox("Font List", ImVec2(140, 90)))
					 {
						 for (auto& pair : mapFonts)
						 {
							 string strName = CStringUtility::ToString(pair.first);
							 const bool bSelect = iCurFontIndex == i;
							 if (ImGui::Selectable(strName.c_str(), bSelect))
							 {
								 m_wstrSaveFontFileName = pair.first;
								 iCurFontIndex = i;

								 if (bSelect)
								 {
									 ImGui::SetItemDefaultFocus();
								 }
							 }
							 i++;
						 }

						 ImGui::EndListBox();
					 }

					 // 텍스트를 입력받습니다.
					 ImGui::InputText("Text", szText, sizeof(szText));

					 int szLength = MultiByteToWideChar(CP_UTF8, 0, szText, -1, nullptr, 0);
					 MultiByteToWideChar(CP_UTF8, 0, szText, -1, szWText, szLength);

					 ImGui::TreePop();
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 ImGui::Separator();

				 ImGui::Dummy(ImVec2(0, 10));

				 // 이 컴포넌트가 기준좌표가 될 건지 설정합니다.
				 ImGui::Checkbox("Pivot", &bPivot);

				 // 이 컴포넌트가 상대좌표로할건지 절대좌표로 할건지 설정합니다.
				 ImGui::Checkbox("Relative", &bRelative);

				 // 이 컴포넌트가 상호작용을 하는지 안하는 지 체크합니다.
				 ImGui::Checkbox("Interection", &bInteraction);

				 // 컴포넌트의 중점을 설정합니다.
				 static float fCenter[3];
				 if (ImGui::InputFloat3("Center", fCenter)) {}

				 // 컴포넌트의 크기를 설정합니다.
				 static float fScale[3] = { 0.0f, 0.0f, 1.0f };
				 if (ImGui::InputFloat3("Scale", fScale)) {}

				 static int iDepth = 0;
				 if (ImGui::InputInt("Depth", &iDepth)) {}

				 static float fRectAngle = 0.0f;
				 if (ImGui::InputFloat("Angle Rect", &fRectAngle)) {}


				 static float fRectAlpha = 1.0f;
				 ImGui::Checkbox("Alpha", &bAlpha);
				 if (bAlpha)
				 {
					 if (ImGui::InputFloat("Alpha Rect", &fRectAlpha)) {}
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 ImGui::SetNextItemWidth(140);
				 // 컴포넌트의 이름을 입력받습니다.
				 static char szComponetName[MAX_PATH] = "";
				 ImGui::InputText("ComponentName", szComponetName, sizeof(szComponetName));
				 std::wstring wstrComponentName(szComponetName, szComponetName + MAX_PATH);

				 static char szSliderName[MAX_PATH] = "";
				 if (EUIType::SLIDER == static_cast<EUIType>(iUIType))
				 {
					 ImGui::SetNextItemWidth(140);
					 ImGui::InputText("SliderButtonName", szSliderName, sizeof(szSliderName)); 
				 }
				 std::wstring wstrSliderName(szSliderName, szSliderName + MAX_PATH);

				 // 저장한 이름에 해당하는 패널에 컴포넌트를 추가해줍니다.
				 if (ImGui::Button("Add"))
				 {
					 weak_ptr<CToolUIPanel> wpPanel;
					 if (0 == iCurrentPanelSelect)
					 {
						 wpPanel = CToolUIManager::GetInstance()->GetCurPanel();
					 }
					 else if (1 == iCurrentPanelSelect)
					 {
						 wpPanel =  static_pointer_cast<CToolUIPanel>(CToolUIManager::GetInstance()->GetChildPanel().lock());
					 }
 
					 if (nullptr != wpPanel.lock())
					 {
						 weak_ptr<CRectComponent> wpFindComponent = wpPanel.lock()->FindComponent<CRectComponent>(wstrComponentName);

						 if (strcmp(szComponetName, "") || nullptr == wpFindComponent.lock())
						 {
							 if (EUIType::STATIC_IMAGE == static_cast<EUIType>(iUIType))
							 {
								 CImageComponent::FImageComponentDesc tImageDsec;

								 tImageDsec.m_wpOwnerGameObject = wpPanel;
								 tImageDsec.m_eComponentType = EComponentType::RECT;
								 tImageDsec.m_wstrComponentName = wstrComponentName;

								 tImageDsec.m_iPassIndex = iPassIndex;
								 tImageDsec.m_eUIType = static_cast<EUIType>(iUIType);
								 tImageDsec.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 tImageDsec.m_wstrShaderFileName = m_wstrSaveShaderName;
								 tImageDsec.m_wstrTextureFileName = m_wstrSaveTextureName;
								 tImageDsec.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 tImageDsec.m_iDepth = iDepth;
								 tImageDsec.m_vColor = m_vSaveColor;
								 tImageDsec.m_bColor = bColor;
								 tImageDsec.m_bAlpha = bAlpha;
								 tImageDsec.m_bMask = bMask;
								 tImageDsec.m_fRectAngle = fRectAngle;
								 tImageDsec.m_fAlpha = fRectAlpha;
								 tImageDsec.m_bInteraction = bInteraction;

								 tImageDsec.m_eImageType = static_cast<EImageType>(iImageType);

								 shared_ptr<CImageComponent> spRectComponent = CImageComponent::Create(make_shared<CImageComponent::FImageComponentDesc>(tImageDsec));
								 wpPanel.lock()->AddComponent(wstrComponentName, spRectComponent);

								 // 직교인지 원근인지에 따라서 값을 다르게 넣어줍니다.
								 if (EProjectionType::ORTHOGONAL == wpPanel.lock()->GetUIProjectionType())
								 {
									 spRectComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spRectComponent->IsRelativeCenter(bRelative);
									 if (bRelative)
									 {
										 spRectComponent->CenterToRelativeCenter();
									 }
								 }
								 else if (EProjectionType::PERSPECTIVE == wpPanel.lock()->GetUIProjectionType())
								 {
									 spRectComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spRectComponent->CenterToRelativeCenter();
									 spRectComponent->IsTool(true);
									 //spRectComponent->SetRectTransform(ETransformType::POSITION, Vector4(fCenter[0], fCenter[1], fCenter[2], 1.0f));
									 //spRectComponent->WorldToRelativeCenter();
								 }

								 spRectComponent->SetRectScale(Vector3(fScale[0], fScale[1], fScale[2]));

								 // 피벗을 체크했을 경우 패널의 기준 피벗컴포넌트로 넣어줍니다.
								 if (bPivot)
								 {
									 wpPanel.lock()->SetPivotComponent(spRectComponent);
								 }
							 }
							 else if (EUIType::BUTTON == static_cast<EUIType>(iUIType))
							 {
								 CButtonComponent::FButtonComponentDesc tButtonDesc;
								 tButtonDesc.m_wpOwnerGameObject = wpPanel;
								 tButtonDesc.m_eComponentType = EComponentType::RECT;
								 tButtonDesc.m_wstrComponentName = wstrComponentName;
								 tButtonDesc.m_iDepth = iDepth;
								 tButtonDesc.m_vColor = m_vSaveColor;

								 tButtonDesc.m_iPassIndex = iPassIndex;
								 tButtonDesc.m_eUIType = static_cast<EUIType>(iUIType);
								 tButtonDesc.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 tButtonDesc.m_wstrShaderFileName = m_wstrSaveShaderName;
								 tButtonDesc.m_wstrTextureFileNameButtonStatic = m_wstrSaveTextureName;
								 tButtonDesc.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 tButtonDesc.m_wstrTextureFileNameButtonHover = m_wstrSaveTextureHoverName;
								 tButtonDesc.m_wstrTextureFileNameButtonClick = m_wstrSaveTextureClickName;
								 tButtonDesc.m_bColor = bColor;
								 tButtonDesc.m_bAlpha = bAlpha;
								 tButtonDesc.m_bMask = bMask;
								 tButtonDesc.m_fRectAngle = fRectAngle;
								 tButtonDesc.m_fAlpha = fRectAlpha;
								 tButtonDesc.m_bInteraction = bInteraction;

								 shared_ptr<CButtonComponent> spButtonComponent = CButtonComponent::Create(make_shared<CButtonComponent::FButtonComponentDesc>(tButtonDesc));
								 wpPanel.lock()->AddComponent(wstrComponentName, spButtonComponent);

								 spButtonComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
								 spButtonComponent->SetRectScale(Vector3(fScale[0], fScale[1], fScale[2]));

								 // 피벗을 체크했을 경우 패널의 기준 피벗컴포넌트로 넣어줍니다.
								 if (bPivot)
								 {
									 wpPanel.lock()->SetPivotComponent(spButtonComponent);
								 }
							 }
							 else if (EUIType::SLIDER == static_cast<EUIType>(iUIType))
							 {
								 // 슬라이더는 이미지와 슬라이더 두 개의 컴포넌트를 만들고 연결해줍니다.
								 CImageComponent::FImageComponentDesc tImageDsec;

								 tImageDsec.m_wpOwnerGameObject = wpPanel;
								 tImageDsec.m_eComponentType = EComponentType::RECT;
								 tImageDsec.m_wstrComponentName = wstrComponentName;
								 tImageDsec.m_iPassIndex = iPassIndex;
								 tImageDsec.m_eUIType = static_cast<EUIType>(EUIType::STATIC_IMAGE);
								 tImageDsec.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 tImageDsec.m_wstrShaderFileName = m_wstrSaveShaderName;
								 tImageDsec.m_wstrTextureFileName = m_wstrSaveTextureName;
								 tImageDsec.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 tImageDsec.m_iDepth = iDepth;
								 tImageDsec.m_vColor = m_vSaveColor;
								 tImageDsec.m_bColor = bColor;
								 tImageDsec.m_bAlpha = bAlpha;
								 tImageDsec.m_bMask = bMask;
								 tImageDsec.m_fAlpha = fRectAlpha;
								 tImageDsec.m_bInteraction = bInteraction;

								 shared_ptr<CImageComponent> spImageComponent = CImageComponent::Create(make_shared<CImageComponent::FImageComponentDesc>(tImageDsec));
								 wpPanel.lock()->AddComponent(wstrComponentName, spImageComponent);

								 spImageComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
								 spImageComponent->SetRectScale(Vector3(fScale[0], fScale[1], fScale[2]));

								 CSliderComponent::FSliderComponentDesc tSliderDesc;

								 tSliderDesc.m_wpOwnerGameObject = wpPanel;
								 tSliderDesc.m_eComponentType = EComponentType::RECT;
								 tSliderDesc.m_wstrComponentName = wstrSliderName;

								 tSliderDesc.m_iPassIndex = iPassIndex + 1;
								 tSliderDesc.m_eUIType = static_cast<EUIType>(iUIType);
								 tSliderDesc.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 tSliderDesc.m_wstrShaderFileName = m_wstrSaveShaderName;
								 tSliderDesc.m_wstrTextureSliderButtonFileName = m_wstrSaveTextureClickName;
								 tSliderDesc.m_wpBackGroundSliderComponent = spImageComponent;
								 tSliderDesc.m_iDepth = iDepth;
								 tSliderDesc.m_vColor = m_vSaveColor;
								 tSliderDesc.m_bColor = bColor;
								 tSliderDesc.m_bAlpha = bAlpha;
								 tSliderDesc.m_bMask = bMask;
								 tSliderDesc.m_fAlpha = fRectAlpha;
								 tSliderDesc.m_bInteraction = bInteraction;

								 shared_ptr<CSliderComponent> spSliderComponent = CSliderComponent::Create(make_shared<CSliderComponent::FSliderComponentDesc>(tSliderDesc));
								 wpPanel.lock()->AddComponent(wstrSliderName, spSliderComponent);

								 spSliderComponent->SetCenterPosition(Vector2(fSliderCenter[0], fSliderCenter[1]));
								 spSliderComponent->SetRectScale(Vector3(fSliderScale[0], fSliderScale[1], fSliderScale[2]));
							 }
							 else if (EUIType::CIRCULAR_PROGRESSBAR == static_cast<EUIType>(iUIType))
							 {
								 // 프로그래스 바 컴포넌트를 생성합니다.
								 CProgressBarComponent::FProgressBarComponentDesc fProgressDesc;
								 fProgressDesc.m_wpOwnerGameObject = wpPanel;
								 fProgressDesc.m_eComponentType = EComponentType::RECT;
								 fProgressDesc.m_wstrComponentName = wstrComponentName;

								 fProgressDesc.m_iPassIndex = iPassIndex;
								 fProgressDesc.m_eUIType = static_cast<EUIType>(iUIType);
								 fProgressDesc.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 fProgressDesc.m_wstrShaderFileName = m_wstrSaveShaderName;
								 fProgressDesc.m_wstrTextureFileName = m_wstrSaveTextureName;
								 fProgressDesc.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 fProgressDesc.m_iDepth = iDepth;
								 fProgressDesc.m_vColor = m_vSaveColor;
								 fProgressDesc.m_bColor = bColor;
								 fProgressDesc.m_bAlpha = bAlpha;
								 fProgressDesc.m_bMask = bMask;
								 fProgressDesc.m_fRectAngle = fRectAngle;
								 fProgressDesc.m_fAlpha = fRectAlpha;
								 fProgressDesc.m_bInteraction = bInteraction;

								 shared_ptr<CProgressBarComponent> spProgressComponent = CProgressBarComponent::Create(make_shared<CProgressBarComponent::FProgressBarComponentDesc>(fProgressDesc));
								 wpPanel.lock()->AddComponent(wstrComponentName, spProgressComponent);

								 // 직교인지 원근인지에 따라서 값을 다르게 넣어줍니다.
								 if (EProjectionType::ORTHOGONAL == wpPanel.lock()->GetUIProjectionType())
								 {
									 spProgressComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spProgressComponent->IsRelativeCenter(bRelative);
									 if (bRelative)
									 {
										 spProgressComponent->CenterToRelativeCenter();
									 }
								 }
								 else if (EProjectionType::PERSPECTIVE == wpPanel.lock()->GetUIProjectionType())
								 {
									 spProgressComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spProgressComponent->CenterToRelativeCenter();
									 spProgressComponent->IsTool(true);
									 //spProgressComponent->SetRectTransform(ETransformType::POSITION, Vector4(fCenter[0], fCenter[1], fCenter[2], 1.0f));
								 }
								 spProgressComponent->SetRectScale(Vector3(fScale[0], fScale[1], fScale[2]));

								 // 피벗을 체크했을 경우 패널의 기준 피벗컴포넌트로 넣어줍니다.
								 if (bPivot)
								 {
									 wpPanel.lock()->SetPivotComponent(spProgressComponent);
								 }
							 }
							 else if (EUIType::LINEAR_HORIZONTAL_PROGRESSBAR == static_cast<EUIType>(iUIType) || EUIType::LINEAR_VERTICAL_PROGRESSBAR == static_cast<EUIType>(iUIType))
							 {
								 // 프로그래스 바 컴포넌트를 생성합니다.
								 CProgressBarComponent::FProgressBarComponentDesc fProgressDesc;
								 fProgressDesc.m_wpOwnerGameObject = wpPanel;
								 fProgressDesc.m_eComponentType = EComponentType::RECT;
								 fProgressDesc.m_wstrComponentName = wstrComponentName;

								 fProgressDesc.m_iPassIndex = iPassIndex;
								 fProgressDesc.m_eUIType = static_cast<EUIType>(iUIType);
								 fProgressDesc.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 fProgressDesc.m_wstrShaderFileName = m_wstrSaveShaderName;
								 fProgressDesc.m_wstrTextureFileName = m_wstrSaveTextureName;
								 fProgressDesc.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 fProgressDesc.m_iDepth = iDepth;
								 fProgressDesc.m_vColor = m_vSaveColor;
								 fProgressDesc.m_bColor = bColor;
								 fProgressDesc.m_bAlpha = bAlpha;
								 fProgressDesc.m_bMask = bMask;
								 fProgressDesc.m_fRectAngle = fRectAngle;
								 fProgressDesc.m_fAlpha = fRectAlpha;
								 fProgressDesc.m_bInteraction = bInteraction;

								 shared_ptr<CProgressBarComponent> spProgressComponent = CProgressBarComponent::Create(make_shared<CProgressBarComponent::FProgressBarComponentDesc>(fProgressDesc));
								 wpPanel.lock()->AddComponent(wstrComponentName, spProgressComponent);

								 // 직교인지 원근인지에 따라서 값을 다르게 넣어줍니다.
								 if (EProjectionType::ORTHOGONAL == wpPanel.lock()->GetUIProjectionType())
								 {
									 spProgressComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spProgressComponent->IsRelativeCenter(bRelative);
									 if (bRelative)
									 {
										 spProgressComponent->CenterToRelativeCenter();
									 }
								 }
								 else if (EProjectionType::PERSPECTIVE == wpPanel.lock()->GetUIProjectionType())
								 {
									 spProgressComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spProgressComponent->CenterToRelativeCenter();
									 spProgressComponent->IsTool(true);
									 //spProgressComponent->SetRectTransform(ETransformType::POSITION, Vector4(fCenter[0], fCenter[1], fCenter[2], 1.0f));
								 }

								 spProgressComponent->SetRectScale(Vector3(fScale[0], fScale[1], fScale[2]));

								 // 피벗을 체크했을 경우 패널의 기준 피벗컴포넌트로 넣어줍니다.
								 if (bPivot)
								 {
									 wpPanel.lock()->SetPivotComponent(spProgressComponent);
								 }
							 }
							 else if (EUIType::DYNAMIC_IMAGE == static_cast<EUIType>(iUIType))
							 {
								 CAnimationImageComponent::FAnimationImageComponentDesc fAnimationDesc;

								 fAnimationDesc.m_wpOwnerGameObject = wpPanel;
								 fAnimationDesc.m_eComponentType = EComponentType::RECT;
								 fAnimationDesc.m_wstrComponentName = wstrComponentName;

								 fAnimationDesc.m_iPassIndex = iPassIndex;
								 fAnimationDesc.m_eUIType = static_cast<EUIType>(iUIType);
								 fAnimationDesc.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 fAnimationDesc.m_wstrShaderFileName = m_wstrSaveShaderName;
								 fAnimationDesc.m_wstrTextureFileName = m_wstrSaveTextureName;
								 fAnimationDesc.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 fAnimationDesc.m_iDepth = iDepth;
								 fAnimationDesc.m_vColor = m_vSaveColor;

								 fAnimationDesc.m_bLoop = bLoop;
								 fAnimationDesc.m_bTool = true;
								 fAnimationDesc.m_bRative = bRelative;
								 fAnimationDesc.m_bColor = bColor;
								 fAnimationDesc.m_bAlpha = bAlpha;
								 fAnimationDesc.m_bMask = bMask;
								 fAnimationDesc.m_bInteraction = bInteraction;

								 // 기본 정보 이외에 추가할 키프레임을 만듭니다.
								 FImageAnimationFrameDesc fAnimationFrameDesc;
								 fAnimationFrameDesc.vPosition = Vector3(fAniPosition[0], fAniPosition[1], fAniPosition[2]);
								 fAnimationFrameDesc.vScale = Vector3(fAniScale[0], fAniScale[1], fAniScale[2]);

								 fAnimationFrameDesc.fAlpha = fAniAlpha;
								 fAnimationFrameDesc.fTime = fAniTime;
								 fAnimationFrameDesc.fSpeed = fAniSpeed;
								 fAnimationFrameDesc.fAngle = fAniAngle;
								 fAnimationFrameDesc.vColor = m_vSaveColor;

								 fAnimationDesc.m_fImageAnimationDesc = fAnimationFrameDesc;

								 // 정보들을 바탕으로 객체를 생성합니다.
								 shared_ptr<CAnimationImageComponent> spComponent = CAnimationImageComponent::Create(make_shared<CAnimationImageComponent::FAnimationImageComponentDesc>(fAnimationDesc));
								 wpPanel.lock()->AddComponent(wstrComponentName, spComponent);
								 // 피벗을 체크했을 경우 패널의 기준 피벗컴포넌트로 넣어줍니다.
								 if (bPivot)
								 {
									 wpPanel.lock()->SetPivotComponent(spComponent);
								 }
								 spComponent->SetEasingType(static_cast<EEasingType>(iEaseType));
							 }
							 else if (EUIType::TEXT == static_cast<EUIType>(iUIType))
							 {
								 CTextComponent::FTextComponentDesc fTextComponentDesc;

								 fTextComponentDesc.m_wpOwnerGameObject = wpPanel;
								 fTextComponentDesc.m_eComponentType = EComponentType::RECT;
								 fTextComponentDesc.m_wstrComponentName = wstrComponentName;

								 fTextComponentDesc.m_iPassIndex = iPassIndex;
								 fTextComponentDesc.m_eUIType = static_cast<EUIType>(iUIType);
								 fTextComponentDesc.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 //tImageDsec.m_wstrShaderFileName = m_wstrSaveShaderName;
								 //tImageDsec.m_wstrTextureFileName = m_wstrSaveTextureName;
								 //tImageDsec.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 fTextComponentDesc.m_iDepth = iDepth;
								 fTextComponentDesc.m_vColor = m_vSaveColor;
								 //Vector4(1.0f, 1.0f, 1.0f, 1.0f);
								 fTextComponentDesc.m_bColor = bColor;
								 //fTextComponentDesc.m_bAlpha = bAlpha;
								 //fTextComponentDesc.m_bMask = bMask;
								 fTextComponentDesc.m_fRectAngle = fRectAngle;
								 //fTextComponentDesc.m_fAlpha = fRectAlpha;
								 std::wstring wstrText = std::wstring(szWText, szWText + wcslen(szWText));
								 fTextComponentDesc.m_wstrText = wstrText;
								 fTextComponentDesc.m_wstrFontFileName = m_wstrSaveFontFileName;
								 fTextComponentDesc.m_eTextType = static_cast<ETextType>(iTextType);
								 fTextComponentDesc.m_bInteraction = bInteraction;

								 shared_ptr<CTextComponent> spRectComponent = CTextComponent::Create(make_shared<CTextComponent::FTextComponentDesc>(fTextComponentDesc));
								 wpPanel.lock()->AddComponent(wstrComponentName, spRectComponent);

								 // 직교인지 원근인지에 따라서 값을 다르게 넣어줍니다.
								 if (EProjectionType::ORTHOGONAL == wpPanel.lock()->GetUIProjectionType())
								 {
									 spRectComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spRectComponent->IsRelativeCenter(bRelative);
									 if (bRelative)
									 {
										 spRectComponent->CenterToRelativeCenter();
									 }
								 }
								 else if (EProjectionType::PERSPECTIVE == wpPanel.lock()->GetUIProjectionType())
								 {
									 spRectComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spRectComponent->CenterToRelativeCenter();
									 spRectComponent->IsTool(true);
									 //spRectComponent->SetRectTransform(ETransformType::POSITION, Vector4(fCenter[0], fCenter[1], fCenter[2], 1.0f));
								 }

								 spRectComponent->SetRectScale(Vector3(fScale[0], fScale[1], fScale[2]));

								 // 피벗을 체크했을 경우 패널의 기준 피벗컴포넌트로 넣어줍니다.
								 if (bPivot)
								 {
									 wpPanel.lock()->SetPivotComponent(spRectComponent);
								 }
							 }
							 else if (EUIType::FRAME_IMAGE == static_cast<EUIType>(iUIType))
							 {
								 CAnimationImageComponent::FAnimationImageComponentDesc fAnimationDesc;

								 fAnimationDesc.m_wpOwnerGameObject = wpPanel;
								 fAnimationDesc.m_eComponentType = EComponentType::RECT;
								 fAnimationDesc.m_wstrComponentName = wstrComponentName;

								 fAnimationDesc.m_iPassIndex = iPassIndex;
								 fAnimationDesc.m_eUIType = static_cast<EUIType>(iUIType);
								 fAnimationDesc.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 fAnimationDesc.m_wstrShaderFileName = m_wstrSaveShaderName;
								 fAnimationDesc.m_wstrTextureFileName = m_wstrSaveTextureName;
								 fAnimationDesc.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 fAnimationDesc.m_iDepth = iDepth;
								 fAnimationDesc.m_vColor = m_vSaveColor;

								 fAnimationDesc.m_bLoop = bLoop;
								 fAnimationDesc.m_bTool = true;
								 fAnimationDesc.m_bRative = bRelative;
								 fAnimationDesc.m_bColor = bColor;
								 fAnimationDesc.m_bAlpha = bAlpha;
								 fAnimationDesc.m_bMask = bMask;
								 fAnimationDesc.m_bInteraction = bInteraction;

								 // 정보들을 바탕으로 객체를 생성합니다.
								 shared_ptr<CAnimationImageComponent> spComponent = CAnimationImageComponent::Create(make_shared<CAnimationImageComponent::FAnimationImageComponentDesc>(fAnimationDesc));
								 
								 spComponent->SetRowCoulmn(iRow, iColoumn, fNextTime);
								 
								 wpPanel.lock()->AddComponent(wstrComponentName, spComponent);
								 // 피벗을 체크했을 경우 패널의 기준 피벗컴포넌트로 넣어줍니다.
								 if (bPivot)
								 {
									 wpPanel.lock()->SetPivotComponent(spComponent);
								 }
								 spComponent->SetEasingType(static_cast<EEasingType>(iEaseType));

								 // 직교인지 원근인지에 따라서 값을 다르게 넣어줍니다.
								 if (EProjectionType::ORTHOGONAL == wpPanel.lock()->GetUIProjectionType())
								 {
									 spComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spComponent->IsRelativeCenter(bRelative);
									 if (bRelative)
									 {
										 spComponent->CenterToRelativeCenter();
									 }
								 }
								 else if (EProjectionType::PERSPECTIVE == wpPanel.lock()->GetUIProjectionType())
								 {
									 spComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spComponent->CenterToRelativeCenter();
									 spComponent->IsTool(true);
									 //spProgressComponent->SetRectTransform(ETransformType::POSITION, Vector4(fCenter[0], fCenter[1], fCenter[2], 1.0f));
								 }

								 spComponent->SetRectScale(Vector3(fScale[0], fScale[1], fScale[2]));
							 }
							 else if (EUIType::LINEAR_INTERPOLATION_PROGRESSBAR == static_cast<EUIType>(iUIType) || EUIType::CIRCLE_INTERPOLATION_PROGRESSBAR == static_cast<EUIType>(iUIType))
							 {
								 // 프로그래스 바 컴포넌트를 생성합니다.
								 CProgressBarComponent::FProgressBarComponentDesc fProgressDesc;
								 fProgressDesc.m_wpOwnerGameObject = wpPanel;
								 fProgressDesc.m_eComponentType = EComponentType::RECT;
								 fProgressDesc.m_wstrComponentName = wstrComponentName;

								 fProgressDesc.m_iPassIndex = iPassIndex;
								 fProgressDesc.m_eUIType = static_cast<EUIType>(iUIType);
								 fProgressDesc.m_eRenderType = static_cast<ERenderType>(iRenderType);
								 fProgressDesc.m_wstrShaderFileName = m_wstrSaveShaderName;
								 fProgressDesc.m_wstrTextureFileName = m_wstrSaveTextureName;
								 fProgressDesc.m_wstrMaskTextureFileName = m_wstrSaveMaskTextureName;
								 fProgressDesc.m_iDepth = iDepth;
								 fProgressDesc.m_vColor = m_vSaveColor;
								 fProgressDesc.m_bColor = bColor;
								 fProgressDesc.m_bAlpha = bAlpha;
								 fProgressDesc.m_bMask = bMask;
								 fProgressDesc.m_fRectAngle = fRectAngle;
								 fProgressDesc.m_fAlpha = fRectAlpha;
								 fProgressDesc.m_bInteraction = bInteraction;

								 shared_ptr<CProgressBarComponent> spProgressComponent = CProgressBarComponent::Create(make_shared<CProgressBarComponent::FProgressBarComponentDesc>(fProgressDesc));
								 wpPanel.lock()->AddComponent(wstrComponentName, spProgressComponent);

								 spProgressComponent->SetOtherRatio(fOtherRatio);
								 spProgressComponent->SetOtherColor(vOtherColor);

								 // 직교인지 원근인지에 따라서 값을 다르게 넣어줍니다.
								 if (EProjectionType::ORTHOGONAL == wpPanel.lock()->GetUIProjectionType())
								 {
									 spProgressComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spProgressComponent->IsRelativeCenter(bRelative);
									 if (bRelative)
									 {
										 spProgressComponent->CenterToRelativeCenter();
									 }
								 }
								 else if (EProjectionType::PERSPECTIVE == wpPanel.lock()->GetUIProjectionType())
								 {
									 spProgressComponent->SetCenterPosition(Vector2(fCenter[0], fCenter[1]));
									 spProgressComponent->CenterToRelativeCenter();
									 spProgressComponent->IsTool(true);
									 //spProgressComponent->SetRectTransform(ETransformType::POSITION, Vector4(fCenter[0], fCenter[1], fCenter[2], 1.0f));
								 }

								 spProgressComponent->SetRectScale(Vector3(fScale[0], fScale[1], fScale[2]));

								 // 피벗을 체크했을 경우 패널의 기준 피벗컴포넌트로 넣어줍니다.
								 if (bPivot)
								 {
									 wpPanel.lock()->SetPivotComponent(spProgressComponent);
								 }
							 }
						 }
					 }
				 }

				 ImGui::Dummy(ImVec2(0.0f, 10.0f));

				 ImGui::Separator();

				 ImGui::EndTabItem();
			 }

			 if (ImGui::BeginTabItem("Current Component"))
			 {
				 ImGui::Dummy(ImVec2(0, 10));

				 shared_ptr<CRectComponent> spCurrentComponent = CToolUIManager::GetInstance()->GetCurComponent().lock();
				 
				 if (spCurrentComponent != nullptr)
				 {
					 Vector2 vCenter = spCurrentComponent->GetCenterPosition();

					 Vector2 vRelativeCenter = spCurrentComponent->GetScreenRelativeCenter();
					 static float fRelativeCenter[2];
					 fRelativeCenter[0] = vRelativeCenter.x;
					 fRelativeCenter[1] = vRelativeCenter.y;
					 //fRelativeCenter[2] = vRelativeCenter.z;
					 
					 ImGui::InputFloat2("RelativeCenter", fRelativeCenter);

					 fCurCenter[0] = vCenter.x;
					 fCurCenter[1] = vCenter.y;
					 ImGui::InputFloat2("Center Cur", fCurCenter);
					 spCurrentComponent->SetCenterPosition(Vector2(fCurCenter[0], fCurCenter[1]));
					 if (EProjectionType::PERSPECTIVE == static_pointer_cast<CPanel>(spCurrentComponent->GetOwnerGameObject())->GetUIProjectionType() && spCurrentComponent != static_pointer_cast<CPanel>(spCurrentComponent->GetOwnerGameObject())->GetPivotComponent())
					 {
						 spCurrentComponent->CenterToRelativeCenter();
					 }
					 else if (EProjectionType::ORTHOGONAL == static_pointer_cast<CPanel>(spCurrentComponent->GetOwnerGameObject())->GetUIProjectionType() && spCurrentComponent != static_pointer_cast<CPanel>(spCurrentComponent->GetOwnerGameObject())->GetPivotComponent())
					 {
						 spCurrentComponent->CenterToRelativeCenter();
					 }

					 Vector3 vCurScale = spCurrentComponent->GetRectScale();
					 fCurScale[0] = vCurScale.x;
					 fCurScale[1] = vCurScale.y;
					 fCurScale[2] = vCurScale.z;
					 ImGui::InputFloat3("Scale Cur", fCurScale);
					 spCurrentComponent->SetRectScale(Vector3(fCurScale[0], fCurScale[1], fCurScale[2]));

					 /*static float fPosition[3] = */
					 Vector4 vPosition = spCurrentComponent->GetRectTransform(ETransformType::POSITION);
					 fPosition[0] = vPosition.x;
					 fPosition[1] = vPosition.y;
					 fPosition[2] = vPosition.z;
					 ImGui::InputFloat3("World Pos", fPosition);
					 //spCurrentComponent->SetRectPosition(Vector3(fPosition[0], fPosition[1], fPosition[2]));
					 //if (spCurrentComponent != static_pointer_cast<CPanel>(spCurrentComponent->GetOwnerGameObject())->GetPivotComponent())
					 //{
						// spCurrentComponent->WorldToRelativeCenter();
					 //}

					 float fRatio = spCurrentComponent->GetRatio();
					 fCurRatio = fRatio;
					 ImGui::SliderFloat("Ratio", &fCurRatio, 0.0f, 1.0f);
					 spCurrentComponent->SetRatio(fCurRatio);

					 float fRectAngle = spCurrentComponent->GetRectAngle();
					 fCurRectAngle = fRectAngle;
					 ImGui::InputFloat("Angle Cur", &fCurRectAngle);
					 spCurrentComponent->SetRectAngle(fCurRectAngle);

					 float fRectAlpha = spCurrentComponent->GetAlpha();
					 fCurRectAlpha = fRectAlpha;
					 ImGui::InputFloat("Alpha Cur", &fCurRectAlpha);
					 spCurrentComponent->SetAlpha(fCurRectAlpha);

					 int iDepth = spCurrentComponent->GetDepth();
					 iCurDepth = iDepth;
					 ImGui::InputInt("Depth Cur", &iCurDepth);
					 spCurrentComponent->SetDepth(iCurDepth);

					 if (spCurrentComponent->GetBoolColor())
					 {
						 Vector4 vComponentColor = spCurrentComponent->GetColor();
						 fCurColor[0] = vComponentColor.x;
						 fCurColor[1] = vComponentColor.y;
						 fCurColor[2] = vComponentColor.z;
						 fCurColor[3] = vComponentColor.w;

						 ImGui::ColorEdit4("Color Cur Image", fCurColor, ImGuiColorEditFlags_Float);
						 spCurrentComponent->SetColor(Vector4(fCurColor[0], fCurColor[1], fCurColor[2], fCurColor[3]));
					 }

					 ImGui::Dummy(ImVec2(0, 10));

					 if (EUIType::DYNAMIC_IMAGE == spCurrentComponent->GetUIType())
					 {
						 iCurEaseType = static_cast<int>(static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->GetEasingType());

						 if (ImGui::Combo("EaseType Cur", &iCurEaseType, "EASE_NONE\0EASE_LINEAR\0EASE_IN_SINE\0EASE_OUT_SINE\0EASE_INOUT_SINE\0EASE_IN_SQUARE\0EASE_OUT_SQUARE\0EASE_INOUT_SQUARE\0EASE_IN_CIRC\0EASE_OUT_CIRC\0EASE_INOUT_CIRC\0EASE_IN_BACK\0EASE_OUT_BACK\0EASE_INOUT_BACK\0EASE_IN_ELASTIC\0EASE_OUT_ELASTIC\0EASE_INOUT_ELASTIC\0EASE_IN_BOUNCE\0EASE_OUT_BOUNCE\0EASE_INOUT_BOUNCE")) {}

						 static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SetEasingType(static_cast<EEasingType>(iCurEaseType));

						 ImGui::Dummy(ImVec2(0, 10));

						 iStartIndex = static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->GetStartIndex();
						 ImGui::InputInt("Set StartIndex", &iStartIndex);
						 static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SetStartIndex(iStartIndex);

						 iEndIndex = static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->GetEndIndex();
						 ImGui::InputInt("Set EndIndex", &iEndIndex);
						 static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SetEndIndex(iEndIndex);

						 ImGui::Dummy(ImVec2(0, 10));

						 //iCurKeyFrameIndex = -1;
						 iPreKeyFrameIndex = -1;
						 std::vector<pair<float, FImageAnimationFrameDesc>>* vecKeyFrame = dynamic_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->GetKeyFrames();
						 
						 ImGui::Text("KeyFrame Index");
						 ImGui::SameLine();
						 ImGui::Text(to_string(vecKeyFrame->size()).c_str());

						 // 현재 컴포넌트의 키프레임 목록을 보여줍니다.
						 if (ImGui::BeginListBox("KeyFrames"))
						 {
							 for (int32 i = 0; i < vecKeyFrame->size(); i++)
							 {
								 const bool bSelect = (iCurKeyFrameIndex == i);
								 if (ImGui::Selectable(std::to_string((*vecKeyFrame)[i].first).c_str(), bSelect))
								 {
									 iCurKeyFrameIndex = i;
									 if (iCurKeyFrameIndex != iPreKeyFrameIndex)
									 {
										 pair<float, FImageAnimationFrameDesc> prKeyFrame = dynamic_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->FindKeyFrame(iCurKeyFrameIndex);
										 Vector2 vCenter;
										 if (EProjectionType::PERSPECTIVE == CToolUIManager::GetInstance()->GetCurPanel().lock()->GetUIProjectionType())
										 {
											 vCenter.x = prKeyFrame.second.vPosition.x;
											 vCenter.y = prKeyFrame.second.vPosition.y;
										 }
										 else if (EProjectionType::ORTHOGONAL == CToolUIManager::GetInstance()->GetCurPanel().lock()->GetUIProjectionType())
										 {
											 vCenter.x = prKeyFrame.second.vPosition.x;
											 vCenter.y = prKeyFrame.second.vPosition.y;
											 //vCenter.x = prKeyFrame.second.vPosition.x + 1280.f * 0.5f;
											 //vCenter.y = -prKeyFrame.second.vPosition.y + 720.f * 0.5f;
										 }

										 fAniCurPosition[0] = vCenter.x;
										 fAniCurPosition[1] = vCenter.y;
										 fAniCurPosition[2] = prKeyFrame.second.vPosition.z;

										 fAniCurScale[0] = prKeyFrame.second.vScale.x;
										 fAniCurScale[1] = prKeyFrame.second.vScale.y;
										 fAniCurScale[2] = prKeyFrame.second.vScale.z;

										 fAniCurTime = prKeyFrame.second.fTime;
										 fAniCurAlpha = prKeyFrame.second.fAlpha;
										 fAniCurAngle = prKeyFrame.second.fAngle;
										 fAniCurSpeed = prKeyFrame.second.fSpeed;
										 vCurColor = prKeyFrame.second.vColor;

										 fAniCurRelativeCenter[0] = prKeyFrame.second.vRelativeCenter.x;
										 fAniCurRelativeCenter[1] = prKeyFrame.second.vRelativeCenter.y;

										 iPreKeyFrameIndex = iCurKeyFrameIndex;
									 }
								 }

								 if (bSelect)
								 {
									 ImGui::SetItemDefaultFocus();
								 }
							 }

							 ImGui::EndListBox();
						 }

						 ImGui::Text("Current Index");
						 ImGui::SameLine();
						 ImGui::Text(to_string(iCurKeyFrameIndex).c_str());

						 ImGui::Dummy(ImVec2(0, 10));

						 // 이미지 애니메이션 컴포넌트의 경우 키프레임의 추가, 삭제 기능을 넣습니다.
						 // 구조체에 필요한 값들을 받아 구조체로 저장합니다.
						 // 키프레임이 선택되었을 경우 구조체의 정보를 불러와서 Imgui칸을 채워줍니다.

						 ImGui::InputFloat2("RelativeCenter", fAniCurRelativeCenter);
						 ImGui::InputFloat3("Position", fAniCurPosition);
						 ImGui::InputFloat3("Image Scale", fAniCurScale);
						 ImGui::InputFloat("Angle", &fAniCurAngle);
						 ImGui::InputFloat("Alpha", &fAniCurAlpha);
						 ImGui::InputFloat("Time", &fAniCurTime);
						 ImGui::InputFloat("Speed", &fAniCurSpeed);
						 ImGui::ColorEdit4("Color Image", (float*)&vCurColor, ImGuiColorEditFlags_Float);

						 FImageAnimationFrameDesc fAnimationDesc;
						 fAnimationDesc.vPosition = Vector3(fAniCurPosition[0], fAniCurPosition[1], fAniCurPosition[2]);
						 fAnimationDesc.vScale = Vector3(fAniCurScale[0], fAniCurScale[1], fAniCurScale[2]);
						 fAnimationDesc.fAngle = fAniCurAngle;

						 fAnimationDesc.fAlpha = fAniCurAlpha;
						 fAnimationDesc.fTime = fAniCurTime;
						 fAnimationDesc.fSpeed = fAniCurSpeed;
						 fAnimationDesc.vColor = vCurColor;

						 ImGui::Dummy(ImVec2(0, 10));

						 if (ImGui::Button("Add"))
						 {
							 // 구조체를 넣어줍니다.
							 dynamic_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->AddKeyFrame(fAnimationDesc);
						 }
						 ImGui::SameLine();
						 if (ImGui::Button("Modify"))
						 {
							 // 구조체를 넣어줍니다.
							 dynamic_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->ReplaceKeyFrame(fAnimationDesc);
						 }
						 ImGui::SameLine();
						 if (ImGui::Button("Delete"))
						 {
							 if (iCurKeyFrameIndex >= 0)
							 {
								 dynamic_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SubtractKeyFrame(iCurKeyFrameIndex);
							 }
						 }

						 ImGui::Dummy(ImVec2(0, 10));

						 if (ImGui::Button("Position ReSet"))
						 {
							 dynamic_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->ResetWorldPosition(Vector2(fCurCenter[0], fCurCenter[1]));
						 }

						 ImGui::Checkbox("ReSet", &bReset);
						 if (bReset)
						 {
							 static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SetStartReset(bReset);
						 }

						 //dynamic_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SetReset(bReset);
					 }

					 if (EUIType::TEXT == spCurrentComponent->GetUIType())
					 {
						 // 텍스트를 입력받습니다.
						 ImGui::InputText("Cur Text", szCurText, sizeof(szCurText));

						 int szLength = MultiByteToWideChar(CP_UTF8, 0, szCurText, -1, nullptr, 0);
						 MultiByteToWideChar(CP_UTF8, 0, szCurText, -1, szCurWText, szLength);

						 static_pointer_cast<CTextComponent>(spCurrentComponent)->SetText(std::wstring(szCurWText, szCurWText + wcslen(szCurWText)));
					 }

					 if (EUIType::FRAME_IMAGE == spCurrentComponent->GetUIType())
					 {
						 iCurRow = static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->GetRow();
						 ImGui::InputInt("Cur Row", &iCurRow);
						 static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SetRow(iCurRow);

						 iCurColoumn = static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->GetColoumn();
						 ImGui::InputInt("Cur Coloumn", &iCurColoumn);
						 static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SetColumn(iCurColoumn);

						 fCurNextTime = static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->GetNextTime();
						 ImGui::InputFloat("Cur NextTime", &fCurNextTime);
						 static_pointer_cast<CAnimationImageComponent>(spCurrentComponent)->SetNextTime(fCurNextTime);

					 }

					 if (EUIType::LINEAR_INTERPOLATION_PROGRESSBAR == spCurrentComponent->GetUIType() || EUIType::CIRCLE_INTERPOLATION_PROGRESSBAR == spCurrentComponent->GetUIType())
					 {
						 fCurOtherRatio = static_pointer_cast<CProgressBarComponent>(spCurrentComponent)->GetOtherRatio();
						 ImGui::InputFloat("Cur Other Ratio", &fCurOtherRatio);
						 static_pointer_cast<CProgressBarComponent>(spCurrentComponent)->SetOtherRatio(fCurOtherRatio);

						 vCurOtherColor = static_pointer_cast<CProgressBarComponent>(spCurrentComponent)->GetOtherColor();
						 ImGui::ColorEdit4("Cur Other Color", (float*)&vCurOtherColor, ImGuiColorEditFlags_Float);
						 static_pointer_cast<CProgressBarComponent>(spCurrentComponent)->SetOtherColor(vCurOtherColor);
					 }
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 pair<int32, int32> prMouse = CEngineModule::GetInstance()->GetInputManager()->GetCursorScreenPosition();
				 
				 fMousePosition[0] = prMouse.first;
				 fMousePosition[1] = prMouse.second;
				 ImGui::InputFloat2("Mouse", fMousePosition);

				 ImGui::Dummy(ImVec2(0, 10));
				 ImGui::SeparatorText("Panel List");

				 // 부모 패널 리스트를 보여줍니다.
				 if (ImGui::BeginListBox("Parent Panel List", ImVec2(140, 90)))
				 {
					 static int iCurSelectIndex = -1;
					 int i = 0;
					 for (auto& pair : CToolUIManager::GetInstance()->GetParentsUIPanels())
					 {
						 string strName = CStringUtility::ToString(pair.first);

						 const bool bSelect = iCurSelectIndex == i;
						 if (ImGui::Selectable(strName.c_str(), bSelect))
						 {
							 m_wstrSaveParentPanelName = pair.first;

							 iCurSelectIndex = i;

							 if (bSelect)
							 {
								 ImGui::SetItemDefaultFocus();
							 }
						 }
						 i++;
					 }

					 ImGui::EndListBox();
				 }
				 // 부모 패널이 선택되었다면 그 자식 패널의 리스트를 보여줍니다.
				 shared_ptr<CPanel> spParentPanel = CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName);
				 CToolUIManager::GetInstance()->SetCurPanel(m_wstrSaveParentPanelName);
				 if (nullptr != spParentPanel)
				 {
					 if (ImGui::BeginListBox("Child Panel List", ImVec2(140, 90)))
					 {
						 static int iCurChildIndex = -1;
						 int i = 0;
						 for (auto& iter : spParentPanel->GetVecChildGameObjects())
						 {
							 string strName = CStringUtility::ToString(iter->GetGameObjectName());
							 const bool bSelect = iCurChildIndex == i;
							 if (ImGui::Selectable(strName.c_str(), bSelect))
							 {
								 // 자식 패널의 이름을 저장합니다.
								 m_wstrSaveChildPanelName = iter->GetGameObjectName();
								 CToolUIManager::GetInstance()->SetChildPanel(m_wstrSaveChildPanelName);
								 iCurChildIndex = i;
								 if (bSelect)
								 {
									 ImGui::SetItemDefaultFocus();
								 }
							 }
							 i++;
						 }

						 ImGui::EndListBox();
					 }
				 }
				 ImGui::Dummy(ImVec2(0, 10));
				 ImGui::SeparatorText("Component List");
				 ImGui::Dummy(ImVec2(0, 10));

				 // 패널의 Projection Type을 지정합니다.
				 if (ImGui::RadioButton("PARENT", &iPanelType, 0)) {}
				 ImGui::SameLine();
				 if (ImGui::RadioButton("CHILD", &iPanelType, 1)) {}

				 ImGui::Dummy(ImVec2(0, 10));

				 if (nullptr != CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName))
				 {
					 if (ImGui::RadioButton("Click", &iFindComponentType, 0)) {}
					 ImGui::SameLine();
					 if (ImGui::RadioButton("List", &iFindComponentType, 1)) {}

					 // 최종 선택된 패널의 컴포넌트 리스트를 보여줍니다.
					 if (ImGui::BeginListBox("Cur Component List", ImVec2(140, 90)))
					 {
						 shared_ptr<CPanel> spPanel;
						 if (0 == iPanelType)
						 {
							 spPanel = CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName);
						 }
						 else
						 {
							 spPanel = dynamic_pointer_cast<CPanel>(CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName)->FindChildGameObject(m_wstrSaveChildPanelName));
						 }

						 static int iCurComponentIndex = -1;
						 int32 i = 0;
						 if (nullptr != spPanel)
						 {
							 for (auto& pair : spPanel->GetMapComponents())
							 {
								 if (L"TransformComponent" == pair.first) { continue; }
								 string strName = CStringUtility::ToString(pair.first);
								 const bool bSelect = iCurComponentIndex == i;
								 if (ImGui::Selectable(strName.c_str(), bSelect))
								 {
									 // 현재 컴포넌트로 지정해줍니다.
									 if (1 == iFindComponentType)
									 {
										 shared_ptr<CRectComponent> spRectComponent = dynamic_pointer_cast<CRectComponent>(pair.second);
										 CToolUIManager::GetInstance()->SetCurComponent(spRectComponent);
									 }

									 iCurComponentIndex = i;

									 if (bSelect)
									 {
										 ImGui::SetItemDefaultFocus();
									 }
								 }
								 i++;
							 }

						 }

						 ImGui::EndListBox();
					 }
				 }

				 if (ImGui::Button("Component Delete"))
				 {
					 shared_ptr<CPanel> spPanel;
					 if (0 == iPanelType)
					 {
						 spPanel = CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName);
					 }
					 else
					 {
						 spPanel = dynamic_pointer_cast<CPanel>(CToolUIManager::GetInstance()->FindParentsPanel(m_wstrSaveParentPanelName)->FindChildGameObject(m_wstrSaveChildPanelName));
					 }

					 if (nullptr != CToolUIManager::GetInstance()->GetCurComponent().lock())
					 {
						 spPanel->RemoveComponent(CToolUIManager::GetInstance()->GetCurComponent().lock()->GetComponentName());
					 }
				 }

				 ImGui::Dummy(ImVec2(0, 10));

				 ImGui::EndTabItem();
			 }

			 ImGui::EndTabBar();
		 }
	 }

	 ImGui::End();
}

void ToolUI::CIMGUIManager::ChangeTransformIMGUI()
{
	ImGui::Begin("Transform");

	// 내가 선택한 컴포넌트의 월드 매트릭스를 불러오고 고칩니다.


	ImGui::End();
}

std::shared_ptr<CRectComponent> ToolUI::CIMGUIManager::GetFindSelectComponent()
{
	// 현재 활성화 된 Panel의 Component들 중 마우스 좌표와 충돌하는 컴포넌트를 찾아줍니다.
	weak_ptr<CToolUIPanel> wpPanel = CToolUIManager::GetInstance()->GetCurPanel();

	std::shared_ptr<CRectComponent> spComponent = wpPanel.lock()->GetFindSelectRectComponent();

	return spComponent;
}
