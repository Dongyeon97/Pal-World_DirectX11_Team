#include "pch.h"
#include "ImGuiWindow.h"

// Module
#include "EngineModule.h"

// World, Level
#include "World.h"
#include "Level.h"
#include "ToolMapLevel.h"

// Handler
#include "ModelHandler.h"

// Manager
#include "CameraManager.h"
#include "PickManager.h"
#include "InputManager.h"

// GameObject
#include "ToolMapObject.h"

// Component
#include "CameraComponent.h"

// Utility
#include "StringUtility.h"

std::shared_ptr<CImGuiWindow> ToolMap::CImGuiWindow::Create()
{
	const std::shared_ptr<CImGuiWindow> spInstance = make_shared<CImGuiWindow>();
	ENSURE(spInstance->Initialize());
	return spInstance;
}

HRESULT ToolMap::CImGuiWindow::Initialize()
{
	LoadMapMeshResources();
	m_wpCameraManager = CEngineModule::GetInstance()->GetCameraManager();
	m_wpPickManager = CEngineModule::GetInstance()->GetPickManager();
	m_wpInputManager = CEngineModule::GetInstance()->GetInputManager();

	return S_OK;
}

HRESULT ToolMap::CImGuiWindow::BeginPlay()
{
	m_wpToolMapLevel = dynamic_pointer_cast<CToolMapLevel>(CWorld::GetInstance()->GetCurLevel());
	return S_OK;
}

int32 ToolMap::CImGuiWindow::Tick()
{
	// Install ImGui 생성
	ImGui::Begin("Install", nullptr, 0);
	TickInstall();
	ImGui::End();

	// Function ImGui 생성
	ImGui::Begin("Function", nullptr, 0);
	TickFunction();
	ImGui::End();

	return 0;
}

void ToolMap::CImGuiWindow::Release()
{
	m_vecGridLandscapeModelFilePaths.clear();
	m_vecGridEnvModelFilePaths.clear();
	m_vecPlaceableTreeModelFilePaths.clear();
	m_vecPlaceableRockModelFilePaths.clear();
	m_vecPlaceableWoodModelFilePaths.clear();
	m_vecPlaceableStoneModelFilePaths.clear();
	m_vecPlaceablePalModelFilePaths.clear();
	m_vecPlaceableArchitectureModelFilePaths.clear();
	m_vecPlaceableSpotLightModelFilePaths.clear();
	m_vecPlaceablePointLightModelFilePaths.clear();
	m_vecPlaceableDirectionalLightModelFilePaths.clear();
	m_vecPlaceableCameraModelFilePaths.clear();

	for (auto& iter : m_umapGridMapListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapTreeListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapRockListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapWoodListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapStoneListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapPalListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapArchitectureListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapSpotLightListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapPointLightListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapDirectionalLightListBox)
	{
		iter.second.second = nullptr;
	}
	for (auto& iter : m_umapCameraListBox)
	{
		iter.second.second = nullptr;
	}
}

/* Install */
void ToolMap::CImGuiWindow::TickInstall()
{
	/* 카메라 위치 */
	{
		Vector3 vCurCameraPos = m_wpCameraManager.lock()->GetCurCameraWorldPosition();
		m_arrCurCameraPos = { vCurCameraPos.x, vCurCameraPos.y, vCurCameraPos.z };
		m_bLockCameraViewDir = m_wpToolMapLevel.lock()->IsCameraLocked();

		ImGui::SeparatorText("Camera");
		ImGui::Checkbox("Press '1' to Lock CamView", &m_bLockCameraViewDir);
		ImGui::InputFloat3("Camera Pos", m_arrCurCameraPos.data());
	}

	/* 피킹한 마우스 위치 */
	{
		Vector3 vPickingPos = m_wpPickManager.lock()->GetPickingPos();
		m_arrPickedPos = { vPickingPos.x, vPickingPos.y, vPickingPos.z };
		m_bPickMode = m_wpToolMapLevel.lock()->GetPickMode();

		ImGui::SeparatorText("Picking");
		ImGui::Checkbox("Press 'P'->Pick Mode ", &m_bPickMode);
		ImGui::Checkbox("Press 'C'->Fast Picking Mode ", &m_bFastPickingMode);
		ImGui::InputFloat3("Picked Pos", m_arrPickedPos.data());
	}

	/* 설치할 오브젝트입니다. */
	{
		ImGui::SeparatorText("Map ObjectType");
		if (ImGui::BeginTabBar("MapObject"))
		{
			InstallGridMapBar();
			InstallObjectBar();
			InstallPalBar();
			InstallLightBar();
			InstallMapCameraBar();

			ImGui::EndTabBar();
		}
	}

	/* Create Object */
	{
		if ((ImGui::Button("Create", ImVec2(ImGui::GetWindowWidth() - 16, 30))))
		{
			AddObjectToFunctionTab();
			CreateObject(m_wstrModelFilePath);
		}
		if (m_bFastPickingMode && m_wpInputManager.lock()->IsKeyDown(DIK_R))
		{
			AddObjectToFunctionTab();
			CreateObject(m_wstrModelFilePath);
		}
	}

	/* Save & Load */
	{
		ImGui::SeparatorText("Save & Load");
		ImGui::InputTextWithHint("File Name", "MapObject.dat", m_chSaveLoadFileName, IM_ARRAYSIZE(m_chSaveLoadFileName));

		m_strSaveLoadFileName = m_chSaveLoadFileName;
		m_wstrSaveLoadFileName = CStringUtility::ToWideString(m_strSaveLoadFileName);

		if (ImGui::Button("Save", ImVec2(ImGui::GetWindowWidth() * 0.5f - 12.f, 30)))
		{
			//SaveObjectData();
			//SaveDataInto64Json();
			SaveDataInto512Json();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load", ImVec2(ImGui::GetWindowWidth() * 0.5f - 12.f, 30)))
		{
			//LoadObjectData();
			//LoadDataFrom64Json();
			LoadDataFrom512Json();
			//Load_Test_ObjectDataForCache();
		}
	}
}

void ToolMap::CImGuiWindow::InstallGridMapBar()
{
	if (ImGui::BeginTabItem("GridMap"))
	{
		if (ImGui::BeginListBox("GridMap", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int32 iIndex = 0; iIndex < m_vecGridLandscapeModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecGridLandscapeModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);

				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"GridMap";
				}
			}

			ImGui::EndListBox();
		}

		ImGui::EndTabItem();
	}
}

void ToolMap::CImGuiWindow::InstallObjectBar()
{
	if (ImGui::BeginTabItem("Object"))
	{
		if (ImGui::BeginListBox("Object", ImVec2(-FLT_MIN, 170)))
		{
			for (int32 iIndex = 0; iIndex < m_vecPlaceableTreeModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceableTreeModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"Tree";
				}
			}

			for (int32 iIndex = 0; iIndex < m_vecPlaceableRockModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceableRockModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"Rock";
				}
			}

			for (int32 iIndex = 0; iIndex < m_vecPlaceableWoodModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceableWoodModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"Wood";
				}
			}

			for (int32 iIndex = 0; iIndex < m_vecPlaceableStoneModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceableStoneModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"Stone";
				}
			}

			for (int32 iIndex = 0; iIndex < m_vecPlaceableArchitectureModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceableArchitectureModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"Architecture";
				}
			}

			ImGui::EndListBox();
		}

		ImGui::EndTabItem();
	}
}

void ToolMap::CImGuiWindow::InstallPalBar()
{
	if (ImGui::BeginTabItem("Pal"))
	{
		if (ImGui::BeginListBox("Pal", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int32 iIndex = 0; iIndex < m_vecPlaceablePalModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceablePalModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"Pal";
				}
			}

			ImGui::EndListBox();
		}

		ImGui::EndTabItem();
	}
}

void ToolMap::CImGuiWindow::InstallLightBar()
{
	// Light
	if (ImGui::BeginTabItem("Light"))
	{
		if (ImGui::BeginListBox("Light", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int32 iIndex = 0; iIndex < m_vecPlaceableSpotLightModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceableSpotLightModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"SpotLight";
				}
			}

			for (int32 iIndex = 0; iIndex < m_vecPlaceablePointLightModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceablePointLightModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"PointLight";
				}
			}

			for (int32 iIndex = 0; iIndex < m_vecPlaceableDirectionalLightModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceableDirectionalLightModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"DirectionalLight";
				}
			}

			ImGui::EndListBox();
		}

		ImGui::EndTabItem();
	}
}

void ToolMap::CImGuiWindow::InstallMapCameraBar()
{
	if (ImGui::BeginTabItem("Camera"))
	{
		if (ImGui::BeginListBox("Camera", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int32 iIndex = 0; iIndex < m_vecPlaceableCameraModelFilePaths.size(); iIndex++)
			{
				const wstring& wstrLabel = ExtractFileNameFromFilePath(iIndex, m_vecPlaceableCameraModelFilePaths);
				const string& strLabel = CStringUtility::ToString(wstrLabel);
				const bool bIsSelected = (m_wstrModelFileName == wstrLabel);
				if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
				{
					m_iSelectedFileIndex = iIndex;
					m_wstrModelFileName = wstrLabel;
					m_wstrObjectName = L"Camera";
				}
			}

			ImGui::EndListBox();
		}

		ImGui::EndTabItem();
	}
}


/* Function */
void ToolMap::CImGuiWindow::TickFunction()
{
	/* 설치한 오브젝트입니다. */
	if (ImGui::BeginTabBar("MapFunction", ImGuiTabBarFlags_None))
	{
		FunctionGridMapBar();
		FunctionObjectBar();
		FunctionPalBar();
		FunctionLightBar();
		FunctionMapCameraBar();

		ImGui::EndTabBar();
	}
}

void ToolMap::CImGuiWindow::FunctionGridMapBar()
{
	if (ImGui::BeginTabItem("GridMap"))
	{
		/* 오른쪽 Window안에서의 왼쪽 패널 */
		ImGui::BeginChild("Left Panel", ImVec2(150, 0), true);
		{
			if (ImGui::BeginListBox("GridMap", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 17)))
			{
				if (m_umapGridMapListBox.size() != 0)
				{
					for (auto iter = m_umapGridMapListBox.begin(); iter != m_umapGridMapListBox.end(); ++iter)
					{
						const wstring& wstrLabel = iter->first;
						const string& strLabel = CStringUtility::ToString(wstrLabel);
						const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

						if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
						{
							m_wstrSelectedListBoxObjectName = wstrLabel;
						}
					}
				}

				ImGui::EndListBox();
			}

			ImGui::EndChild();
		}

		ImGui::SameLine();

		/* 오른쪽 Window안에서의 오른쪽 패널 */
		ImGui::BeginGroup();
		{
			ImGui::BeginChild("View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			{
				// 선택한 객체의 인덱스
				ImGui::SeparatorText("Index");
				ImGui::Text("Selected GridMap Index : ");

				// 옥트리 인덱스
				ImGui::SeparatorText("Octree");
				ImGui::InputInt3("Index", m_arrOctreeIndex.data(), ImGuiInputTextFlags_ReadOnly);

				EditTransform(m_iGameObjectID);

				ImGui::SeparatorText("Delete");
				if (ImGui::Button("Delete Lastest", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteLatestObject();
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete All", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteAllObject();
				}

				ImGui::EndChild();
			}

			ImGui::EndGroup();
		}

		ImGui::EndTabItem();
	}
}

void ToolMap::CImGuiWindow::FunctionObjectBar()
{
	if (ImGui::BeginTabItem("Object"))
	{
		/* 오른쪽 Window안에서의 왼쪽 패널 */
		ImGui::BeginChild("Left Panel", ImVec2(150, 0), true);
		{
			if (ImGui::BeginListBox("MapObject", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 17)))
			{
				if (m_umapTreeListBox.size() != 0)
				{
					for (auto iter = m_umapTreeListBox.begin(); iter != m_umapTreeListBox.end(); ++iter)
					{
						const wstring& wstrLabel = iter->first;
						const string& strLabel = CStringUtility::ToString(wstrLabel);
						const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

						if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
						{
							m_wstrSelectedListBoxObjectName = wstrLabel;
						}
					}
				}

				if (m_umapRockListBox.size() != 0)
				{
					for (auto iter = m_umapRockListBox.begin(); iter != m_umapRockListBox.end(); ++iter)
					{
						const wstring& wstrLabel = iter->first;
						const string& strLabel = CStringUtility::ToString(wstrLabel);
						const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

						if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
						{
							m_wstrSelectedListBoxObjectName = wstrLabel;
						}
					}
				}

				if (m_umapWoodListBox.size() != 0)
				{
					for (auto iter = m_umapWoodListBox.begin(); iter != m_umapWoodListBox.end(); ++iter)
					{
						const wstring& wstrLabel = iter->first;
						const string& strLabel = CStringUtility::ToString(wstrLabel);
						const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

						if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
						{
							m_wstrSelectedListBoxObjectName = wstrLabel;
						}
					}
				}

				if (m_umapStoneListBox.size() != 0)
				{
					for (auto iter = m_umapStoneListBox.begin(); iter != m_umapStoneListBox.end(); ++iter)
					{
						const wstring& wstrLabel = iter->first;
						const string& strLabel = CStringUtility::ToString(wstrLabel);
						const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

						if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
						{
							m_wstrSelectedListBoxObjectName = wstrLabel;
						}
					}
				}

				if (m_umapArchitectureListBox.size() != 0)
				{
					for (auto iter = m_umapArchitectureListBox.begin(); iter != m_umapArchitectureListBox.end(); ++iter)
					{
						const wstring& wstrLabel = iter->first;
						const string& strLabel = CStringUtility::ToString(wstrLabel);
						const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

						if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
						{
							m_wstrSelectedListBoxObjectName = wstrLabel;
						}
					}
				}

				ImGui::EndListBox();
			}

			ImGui::EndChild();
		}

		ImGui::SameLine();

		/* 오른쪽 Window안에서의 오른쪽 패널 */
		ImGui::BeginGroup();
		{
			ImGui::BeginChild("View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			{
				// 선택한 객체의 인덱스
				ImGui::SeparatorText("Index");
				ImGui::Text("Selected MapObject Index : ");

				// 옥트리 인덱스
				m_arrOctreeIndex[0] = m_arrOctreeIndex[0] * 0.5f;
				m_arrOctreeIndex[1] = m_arrOctreeIndex[1] * 0.5f;
				m_arrOctreeIndex[2] = m_arrOctreeIndex[2] * 0.5f;
				ImGui::SeparatorText("Octree");
				ImGui::InputInt3("Index", m_arrOctreeIndex.data(), ImGuiInputTextFlags_ReadOnly);

				EditTransform(m_iGameObjectID);

				ImGui::SeparatorText("Delete");
				if (ImGui::Button("Delete Lastest", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteLatestObject();
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete All", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteAllObject();
				}

				ImGui::EndChild();
			}

			ImGui::EndGroup();
		}

		ImGui::EndTabItem();
	}
}

void ToolMap::CImGuiWindow::FunctionPalBar()
{
	if (ImGui::BeginTabItem("Pal"))
	{
		/* 왼쪽 패널 */
		static int selected = 0;
		{
			ImGui::BeginChild("Left Panel", ImVec2(150, 0), true);
			{
				if (ImGui::BeginListBox("Pal", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 17)))
				{
					if (m_umapPalListBox.size() != 0)
					{
						for (auto iter = m_umapPalListBox.begin(); iter != m_umapPalListBox.end(); ++iter)
						{
							const wstring& wstrLabel = iter->first;
							const string& strLabel = CStringUtility::ToString(wstrLabel);
							const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

							if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
							{
								m_wstrSelectedListBoxObjectName = wstrLabel;
							}
						}
					}
					ImGui::EndListBox();
				}
				ImGui::EndChild();
			}
		}
		/* 오른쪽 패널 */
		ImGui::SameLine();

		ImGui::BeginGroup();
		{
			ImGui::BeginChild("View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			{
				// 선택한 객체의 인덱스
				ImGui::SeparatorText("Index");
				ImGui::Text("Selected Pal Index : ");

				// 옥트리 인덱스
				ImGui::SeparatorText("Octree");
				ImGui::InputInt3("Index", m_arrOctreeIndex.data(), ImGuiInputTextFlags_ReadOnly);

				EditTransform(m_iGameObjectID);

				ImGui::SeparatorText("Delete");
				if (ImGui::Button("Delete Lastest", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteLatestObject();
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete All", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteAllObject();
				}


				ImGui::EndChild();
			}

			ImGui::EndGroup();
		}

		ImGui::EndTabItem();
	}
}

void ToolMap::CImGuiWindow::FunctionLightBar()
{
	if (ImGui::BeginTabItem("Light"))
	{
		/* 왼쪽 패널 */
		static int selected = 0;
		{
			ImGui::BeginChild("Left Panel", ImVec2(150, 0), true);
			{
				if (ImGui::BeginListBox("Light", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 17)))
				{
					if (m_umapSpotLightListBox.size() != 0)
					{
						for (auto iter = m_umapSpotLightListBox.begin(); iter != m_umapSpotLightListBox.end(); ++iter)
						{
							const wstring& wstrLabel = iter->first;
							const string& strLabel = CStringUtility::ToString(wstrLabel);
							const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

							if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
							{
								m_wstrSelectedListBoxObjectName = wstrLabel;
							}
						}
					}

					if (m_umapPointLightListBox.size() != 0)
					{
						for (auto iter = m_umapPointLightListBox.begin(); iter != m_umapPointLightListBox.end(); ++iter)
						{
							const wstring& wstrLabel = iter->first;
							const string& strLabel = CStringUtility::ToString(wstrLabel);
							const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

							if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
							{
								m_wstrSelectedListBoxObjectName = wstrLabel;
							}
						}
					}

					if (m_umapDirectionalLightListBox.size() != 0)
					{
						for (auto iter = m_umapDirectionalLightListBox.begin(); iter != m_umapDirectionalLightListBox.end(); ++iter)
						{
							const wstring& wstrLabel = iter->first;
							const string& strLabel = CStringUtility::ToString(wstrLabel);
							const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

							if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
							{
								m_wstrSelectedListBoxObjectName = wstrLabel;
							}
						}
					}

					ImGui::EndListBox();
				}
				ImGui::EndChild();
			}
		}
		/* 오른쪽 패널 */
		ImGui::SameLine();

		ImGui::BeginGroup();
		{
			ImGui::BeginChild("View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			{
				// 선택한 객체의 인덱스
				ImGui::SeparatorText("Index");
				ImGui::Text("Selected Light Index : ");

				// 옥트리 인덱스
				ImGui::SeparatorText("Octree");
				ImGui::InputInt3("Index", m_arrOctreeIndex.data(), ImGuiInputTextFlags_ReadOnly);

				EditTransform(m_iGameObjectID);

				ImGui::SeparatorText("Delete");
				if (ImGui::Button("Delete Lastest", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteLatestObject();
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete All", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteAllObject();
				}

				ImGui::EndChild();
			}

			ImGui::EndGroup();
		}

		ImGui::EndTabItem();
	}
}

void ToolMap::CImGuiWindow::FunctionMapCameraBar()
{
	if (ImGui::BeginTabItem("Camera"))
	{
		/* 왼쪽 패널 */
		static int selected = 0;
		{
			ImGui::BeginChild("Left Panel", ImVec2(150, 0), true);
			{
				if (ImGui::BeginListBox("Camera", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 17)))
				{
					if (m_umapCameraListBox.size() != 0)
					{
						for (auto iter = m_umapCameraListBox.begin(); iter != m_umapCameraListBox.end(); ++iter)
						{
							const wstring& wstrLabel = iter->first;
							const string& strLabel = CStringUtility::ToString(wstrLabel);
							const bool bIsSelected = (m_wstrSelectedListBoxObjectName == wstrLabel);

							if (ImGui::Selectable(strLabel.c_str(), bIsSelected))
							{
								m_wstrSelectedListBoxObjectName = wstrLabel;
							}
						}
					}
					ImGui::EndListBox();
				}
				ImGui::EndChild();
			}
		}
		/* 오른쪽 패널 */
		ImGui::SameLine();

		ImGui::BeginGroup();
		{
			ImGui::BeginChild("View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			{
				// 선택한 객체의 인덱스
				ImGui::SeparatorText("Index");
				ImGui::Text("Selected Camera Index : ");

				// 옥트리 인덱스
				ImGui::SeparatorText("Octree");
				ImGui::InputInt3("Index", m_arrOctreeIndex.data(), ImGuiInputTextFlags_ReadOnly);

				EditTransform(m_iGameObjectID);

				ImGui::SeparatorText("Delete");
				if (ImGui::Button("Delete Lastest", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteLatestObject();
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete All", ImVec2(ImGui::GetWindowSize().x * 0.5f, 30)))
				{
					DeleteAllObject();
				}

				ImGui::EndChild();
			}

			ImGui::EndGroup();
		}

		ImGui::EndTabItem();
	}
}


void ToolMap::CImGuiWindow::AddObjectToFunctionTab()
{
	if (m_wstrObjectName == L"GridMap")
	{
		m_wstrModelFilePath = m_vecGridLandscapeModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iGridMapCount);
		m_iGridMapCount++;
		m_umapGridMapListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"Tree")
	{
		m_wstrModelFilePath = m_vecPlaceableTreeModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iTreeCount);
		m_iTreeCount++;
		m_umapTreeListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"Rock")
	{
		m_wstrModelFilePath = m_vecPlaceableRockModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iRockCount);
		m_iRockCount++;
		m_umapRockListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"Branch")
	{
		m_wstrModelFilePath = m_vecPlaceableWoodModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iWoodCount);
		m_iWoodCount++;
		m_umapWoodListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"Stone")
	{
		m_wstrModelFilePath = m_vecPlaceableStoneModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iStoneCount);
		m_iStoneCount++;
		m_umapStoneListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"Pal")
	{
		m_wstrModelFilePath = m_vecPlaceablePalModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iPalCount);
		m_iPalCount++;
		m_umapPalListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"Architecture")
	{
		m_wstrModelFilePath = m_vecPlaceableArchitectureModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iArchitectureCount);
		m_iArchitectureCount++;
		m_umapArchitectureListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"SpotLight")
	{
		m_wstrModelFilePath = m_vecPlaceableSpotLightModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iSpotLightCount);
		m_iSpotLightCount++;
		m_umapSpotLightListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"PointLight")
	{
		m_wstrModelFilePath = m_vecPlaceablePointLightModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iPointLightCount);
		m_iPointLightCount++;
		m_umapPointLightListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"DirectionalLight")
	{
		m_wstrModelFilePath = m_vecPlaceableDirectionalLightModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iDirectionalLightCount);
		m_iDirectionalLightCount++;
		m_umapDirectionalLightListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
	else if (m_wstrObjectName == L"Camera")
	{
		m_wstrModelFilePath = m_vecPlaceableCameraModelFilePaths[m_iSelectedFileIndex];
		m_wstrSelectedListBoxObjectName = m_wstrObjectName + std::to_wstring(m_iCameraCount);
		m_iCameraCount++;
		m_umapCameraListBox.emplace(m_wstrSelectedListBoxObjectName, std::make_pair(m_wstrModelFilePath, nullptr));
	}
}

void ToolMap::CImGuiWindow::CreateObject(const wstring& _wstrModelFilePath)
{
	if (m_vecGridLandscapeModelFilePaths.size() > m_iSelectedFileIndex &&
		m_vecGridLandscapeModelFilePaths[m_iSelectedFileIndex] == _wstrModelFilePath)
	{
		std::weak_ptr<CToolMapLevel> wpToolMapLevel = dynamic_pointer_cast<CToolMapLevel>(CWorld::GetInstance()->GetCurLevel());
		std::weak_ptr<CToolMapObject> wpToolMapObject = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, _wstrModelFilePath);

		                                                                                                                                   //03-02-12-13-23
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEnv0 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[0]); // 02
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEnv1 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[1]); // 03
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEnv2 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[2]); // 10
		std::weak_ptr<CToolMapObject> wpToolMapObjectEnv3 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[3]); // 11                              
		std::weak_ptr<CToolMapObject> wpToolMapObjectEnv4 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[4]); // 12
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEnv5 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[5]); // 13
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEnv6 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[6]); // 20
		std::weak_ptr<CToolMapObject> wpToolMapObjectEnv7 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[7]); // 21
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEnv8 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[8]); // 22
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEn9 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[9]); // 23
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEnv10 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[10]); // 32
		//std::weak_ptr<CToolMapObject> wpToolMapObjectEnv11 = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[11]); // 33

	
		/*for (int32 iIndex = 0; iIndex < m_vecGridEnvModelFilePaths.size(); iIndex++)
		{
			std::weak_ptr<CToolMapLevel> wpToolMapLevel = dynamic_pointer_cast<CToolMapLevel>(CWorld::GetInstance()->GetCurLevel());
			std::weak_ptr<CToolMapObject> wpToolMapObject = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, m_vecGridEnvModelFilePaths[iIndex]);
		}*/
	}
	else
	{
		std::weak_ptr<CToolMapLevel> wpToolMapLevel = dynamic_pointer_cast<CToolMapLevel>(CWorld::GetInstance()->GetCurLevel());
		std::weak_ptr<CToolMapObject> wpToolMapObject = wpToolMapLevel.lock()->CreateToolMapObject(m_wstrSelectedListBoxObjectName, _wstrModelFilePath);

		// 맵에 추가해줍니다.
		if (m_wstrObjectName == L"Tree")
		{
			FTreeCache tTreeCache = {};
			tTreeCache.m_iHabitatType = static_cast<uint32>(EHabitatType::NATURE);
			tTreeCache.m_vScale = { 1.0f, GetRandomScale(),1.0f };
			tTreeCache.m_vRotate = { 0.0f, GetRandomRotationAngle(), 0.0f };
			tTreeCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tTreeCache.m_wstrModelFilePath = _wstrModelFilePath;
			tTreeCache.m_bTrigger = false;
			tTreeCache.m_iColliderType = static_cast<uint32>(EColliderType::CAPSULE);
			tTreeCache.m_iNatureType = 0;
			tTreeCache.m_fCurHp = 100.0f;
			tTreeCache.m_vCenter  = wpToolMapObject.lock()->GetDebugBoxColliderCenter();
			tTreeCache.m_vExtents = wpToolMapObject.lock()->GetDebugBoxColliderExtents();
			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tTreeCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };

			wpToolMapObject.lock()->SetTreeCache(tTreeCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tTreeCache);
			wpToolMapObject.lock()->SetGameObjectName(m_wstrSelectedListBoxObjectName);
			m_umapTreeListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		else if (m_wstrObjectName == L"Rock")
		{
			FRockCache tRockCache = {};
			tRockCache.m_iHabitatType = static_cast<uint32>(EHabitatType::NATURE);
			tRockCache.m_vScale = { 0.05f, 0.05f, 0.05f };
			tRockCache.m_vRotate = { 0.0f, GetRandomRotationAngle(), 0.0f };
			tRockCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tRockCache.m_wstrModelFilePath = _wstrModelFilePath;
			tRockCache.m_bTrigger = false;
			tRockCache.m_iColliderType = static_cast<uint32>(EColliderType::SPHERE);
			tRockCache.m_iNatureType = 1;
			tRockCache.m_fCurHp = 100.0f;
			tRockCache.m_vCenter = wpToolMapObject.lock()->GetDebugSphereColliderCenter();
			tRockCache.m_fRadius = wpToolMapObject.lock()->GetDebugSphereColliderRadius();
			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tRockCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };

			wpToolMapObject.lock()->SetRockCache(tRockCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tRockCache);
			wpToolMapObject.lock()->SetGameObjectName(m_wstrSelectedListBoxObjectName);
			m_umapRockListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		/*else if (m_wstrObjectName == L"Branch")
		{
			FBranchCache tBranchCache = {};
			tBranchCache.m_iHabitatType = static_cast<uint32>(EHabitatType::NATURE);
			tBranchCache.m_vScale = { 1.f, GetRandomScale(), 1.f };
			tBranchCache.m_vRotate = { 0.0f, GetRandomRotationAngle(), 0.0f };
			tBranchCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tBranchCache.m_wstrModelFilePath = _wstrModelFilePath;
			tBranchCache.m_bTrigger = true;
			tBranchCache.m_iColliderType = static_cast<uint32>(EColliderType::CAPSULE);
			tBranchCache.m_iNatureType = 2;
			tBranchCache.m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
			tBranchCache.m_fRadius = 0.001f;
			tBranchCache.m_fHalfHeight = 0.001f;
			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tBranchCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };	

			wpToolMapObject.lock()->SetBranchCache(tBranchCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tBranchCache);
			m_umapWoodListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		else if (m_wstrObjectName == L"Stone")
		{
			FStoneCache tStoneCache = {};
			tStoneCache.m_iHabitatType = static_cast<uint32>(EHabitatType::NATURE);
			tStoneCache.m_vScale = { 1.f, GetRandomScale(), 1.f };
			tStoneCache.m_vRotate = { 0.0f, GetRandomRotationAngle(), 0.0f };
			tStoneCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tStoneCache.m_wstrModelFilePath = _wstrModelFilePath;
			tStoneCache.m_bTrigger = true;
			tStoneCache.m_iColliderType = static_cast<uint32>(EColliderType::SPHERE);
			tStoneCache.m_iNatureType = 3;
			tStoneCache.m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
			tStoneCache.m_fRadius = 0.001f;
			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tStoneCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };

			wpToolMapObject.lock()->SetStoneCache(tStoneCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tStoneCache);
			m_umapStoneListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		else if (m_wstrObjectName == L"Pal")
		{
			FPalCache tPalCache = {};
			tPalCache.m_bTrigger = false;
			tPalCache.m_iHabitatType = static_cast<uint32>(EHabitatType::PAL);
			tPalCache.m_iColliderType = static_cast<uint32>(EColliderType::SPHERE);
			tPalCache.m_fRadius = 0.01f;
			tPalCache.m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
			tPalCache.m_vScale = { 1.f, GetRandomScale(), 1.f };
			tPalCache.m_vRotate = { 0.0f, GetRandomRotationAngle(), 0.0f };
			tPalCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tPalCache.m_wstrModelFilePath = _wstrModelFilePath;

			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tPalCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };

			wpToolMapObject.lock()->SetPalCache(tPalCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tPalCache);
			m_umapPalListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		else if (m_wstrObjectName == L"Architecture")
		{
			FArchitectureCache tArchitectureCache = {};
			tArchitectureCache.m_bTrigger = false;
			tArchitectureCache.m_iHabitatType = static_cast<uint32>(EHabitatType::ARCHITECTURE);
			tArchitectureCache.m_iColliderType = static_cast<uint32>(EColliderType::TRIANGLE_MESH);
			tArchitectureCache.m_fExtent = 0.01f;
			tArchitectureCache.m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
			tArchitectureCache.m_vScale = { 1.f, GetRandomScale(), 1.f };
			tArchitectureCache.m_vRotate = { 0.0f, GetRandomRotationAngle(), 0.0f };
			tArchitectureCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tArchitectureCache.m_wstrModelFilePath = _wstrModelFilePath;

			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tArchitectureCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };

			wpToolMapObject.lock()->SetArchitectureCache(tArchitectureCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tArchitectureCache);
			m_umapPalListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		else if (m_wstrObjectName == L"SpotLight")
		{
			FSpotLightCache tSpotLightCache = {};
			tSpotLightCache.m_bTrigger = false;
			tSpotLightCache.m_iHabitatType = static_cast<uint32>(EHabitatType::SPOTLIGHT);
			tSpotLightCache.m_iColliderType = static_cast<uint32>(EColliderType::ENUM_END);
			tSpotLightCache.m_vScale = { 1.0f, 1.0f, 1.0f };
			tSpotLightCache.m_vRotate = { 0.0f, 0.0f, 0.0f };
			tSpotLightCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tSpotLightCache.m_wstrModelFilePath = _wstrModelFilePath;

			tSpotLightCache.m_fRadius = 1.0f;												
			tSpotLightCache.m_fCosAngle = 0.0f;;											
			tSpotLightCache.m_vEnergy = { 0.0f, 0.0f, 0.0f };							
			tSpotLightCache.m_vPosition = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tSpotLightCache.m_vDirection = { 0.0f, 0.0f, 0.0f };		

			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tSpotLightCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };

			wpToolMapObject.lock()->SetSpotLightCache(tSpotLightCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tSpotLightCache);
			m_umapSpotLightListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		else if (m_wstrObjectName == L"PointLight")
		{
			FPointLightCache tPointLightCache = {};
			tPointLightCache.m_bTrigger = false;
			tPointLightCache.m_iHabitatType = static_cast<uint32>(EHabitatType::POINTLIGHT);
			tPointLightCache.m_iColliderType = static_cast<uint32>(EColliderType::ENUM_END);
			tPointLightCache.m_vScale = { 1.0f, 1.0f, 1.0f };
			tPointLightCache.m_vRotate = { 0.0f, 0.0f, 0.0f };
			tPointLightCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tPointLightCache.m_wstrModelFilePath = _wstrModelFilePath;

			tPointLightCache.m_fRadius = 1.0f;											 
			tPointLightCache.m_vEnergy = { 0.0f, 0.0f, 0.0f };						
			tPointLightCache.m_vPosition = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };

			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tPointLightCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };

			wpToolMapObject.lock()->SetPointLightCache(tPointLightCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tPointLightCache);
			m_umapPointLightListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		else if (m_wstrObjectName == L"DirectionalLight")
		{
			FDirectionalLightCache tDirectionalLightCache = {};
			tDirectionalLightCache.m_bTrigger = false;
			tDirectionalLightCache.m_iHabitatType = static_cast<uint32>(EHabitatType::DIRECTIONALLIGHT);
			tDirectionalLightCache.m_iColliderType = static_cast<uint32>(EColliderType::ENUM_END);
			tDirectionalLightCache.m_vScale = { 1.0f, 1.0f, 1.0f };
			tDirectionalLightCache.m_vRotate = { 0.0f, 0.0f, 0.0f };
			tDirectionalLightCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tDirectionalLightCache.m_wstrModelFilePath = _wstrModelFilePath;

			tDirectionalLightCache.m_fSolidAngle = 0.0f;					
			tDirectionalLightCache.m_fPerceivedRadius = 1.0f;				
			tDirectionalLightCache.m_fPerceivedDistance = 1.0f;			
			tDirectionalLightCache.m_vEnergy = { 0.0f, 0.0f, 0.0f };		
			tDirectionalLightCache.m_vDirection = { 0.0f, 0.0f, 0.0f };	

			int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(Vector3(m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2])));
			tDirectionalLightCache.m_tpOctreeNodeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };

			wpToolMapObject.lock()->SetDirectionalLightCache(tDirectionalLightCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tDirectionalLightCache);
			m_umapDirectionalLightListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}
		else if (m_wstrObjectName == L"Camera")
		{
			FCameraCache tCameraCache = {};
			tCameraCache.m_bTrigger = false;
			tCameraCache.m_iHabitatType = static_cast<uint32>(EHabitatType::CAMERA);
			tCameraCache.m_iColliderType = static_cast<uint32>(EColliderType::ENUM_END);
			tCameraCache.m_vScale = { 1.0f, 1.0f, 1.0f };
			tCameraCache.m_vRotate = { 0.0f, 0.0f, 0.0f };
			tCameraCache.m_vTranslate = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tCameraCache.m_wstrModelFilePath = _wstrModelFilePath;

			tCameraCache.m_fRightSlope = 1.0f;
			tCameraCache.m_fLeftSlope = 1.0f;
			tCameraCache.m_fTopSlope = 1.0f;
			tCameraCache.m_fBottomSlope = 1.0f;
			tCameraCache.m_fNearPlane = 0.0f;
			tCameraCache.m_fFarPlane = 1.0f;
			tCameraCache.m_vCenter = { m_arrPickedPos[0], m_arrPickedPos[1], m_arrPickedPos[2] };
			tCameraCache.m_vRadian = { 0.0f, 0.0f, 0.0f };

			wpToolMapObject.lock()->SetCameraCache(tCameraCache);
			wpToolMapObject.lock()->SetHabitatBasicInfo(tCameraCache);
			m_umapCameraListBox.find(m_wstrSelectedListBoxObjectName)->second.second = wpToolMapObject.lock();
		}*/
	}
}

void ToolMap::CImGuiWindow::DeleteLatestObject()
{
	wstring wstrGameObjectName = ExtractWstringWithoutNumber(m_wpToolMapLevel.lock()->FindGameObjectByID(m_iGameObjectID)->GetGameObjectName());
	//m_wpToolMapLevel.lock()->RemoveGameObjectByID(m_iGameObjectID);

	if (L"Tree" == wstrGameObjectName)
	{
		wstring wstrTreeName;
		for (auto& iter : m_umapTreeListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrTreeName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapTreeListBox.erase(wstrTreeName);
	}
	else if (L"Rock" == wstrGameObjectName)
	{
		wstring wstrRockName;
		for (auto& iter : m_umapRockListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrRockName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapRockListBox.erase(wstrRockName);
	}
	else if (L"Branch" == wstrGameObjectName)
	{
		wstring wstrBranchName;
		for (auto& iter : m_umapWoodListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrBranchName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapWoodListBox.erase(wstrBranchName);
	}
	else if (L"Stone" == wstrGameObjectName)
	{
		wstring wstrStoneName;
		for (auto& iter : m_umapStoneListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrStoneName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapStoneListBox.erase(wstrStoneName);
	}
	else if (L"Pal" == wstrGameObjectName)
	{
		wstring wstrPalName;
		for (auto& iter : m_umapPalListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrPalName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapPalListBox.erase(wstrPalName);
	}
	else if (L"Architecture" == wstrGameObjectName)
	{
		wstring wstrArchitectureName;
		for (auto& iter : m_umapArchitectureListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrArchitectureName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapArchitectureListBox.erase(wstrArchitectureName);
	}
	else if (L"SpotLight" == wstrGameObjectName)
	{
		wstring wstrSpotLightName;
		for (auto& iter : m_umapSpotLightListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrSpotLightName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapSpotLightListBox.erase(wstrSpotLightName);
	}
	else if (L"PointLight" == wstrGameObjectName)
	{
		wstring wstrPointLightName;
		for (auto& iter : m_umapPointLightListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrPointLightName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapPointLightListBox.erase(wstrPointLightName);
	}
	else if (L"DirectionalLight" == wstrGameObjectName)
	{
		wstring wstrDirectionalLightName;
		for (auto& iter : m_umapDirectionalLightListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrDirectionalLightName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapDirectionalLightListBox.erase(wstrDirectionalLightName);
	}
	else if (L"Camera" == wstrGameObjectName)
	{
		wstring wstrCameraName;
		for (auto& iter : m_umapCameraListBox)
		{
			int32 iGameObjectID = iter.second.second->GetGameObjectID();
			if (iGameObjectID == m_iGameObjectID) { wstrCameraName = iter.second.second->GetGameObjectName(); }
			else { continue; }
		}
		m_umapCameraListBox.erase(wstrCameraName);
	}
}

void ToolMap::CImGuiWindow::DeleteAllObject()
{
}

/* Gizmo */
void ToolMap::CImGuiWindow::EditTransform(const uint32& _iGameObjectID)
{
	weak_ptr<CToolMapObject> wpToolMapObject = dynamic_pointer_cast<CToolMapObject>(m_wpToolMapLevel.lock()->FindGameObjectByID(_iGameObjectID));
	if (wpToolMapObject.expired()) { return; }
	optional<Matrix> matWorldMatrix = wpToolMapObject.lock()->GetTransformMatrix();

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::SCALE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	array<float, 16> arrWorldMatrix =
	{
		matWorldMatrix->_11, matWorldMatrix->_12, matWorldMatrix->_13, matWorldMatrix->_14,
		matWorldMatrix->_21, matWorldMatrix->_22, matWorldMatrix->_23, matWorldMatrix->_24,
		matWorldMatrix->_31, matWorldMatrix->_32, matWorldMatrix->_33, matWorldMatrix->_34,
		matWorldMatrix->_41, matWorldMatrix->_42, matWorldMatrix->_43, matWorldMatrix->_44,
	};

	// 입력창
	if (0 != _iGameObjectID)
	{
		ImGuizmo::DecomposeMatrixToComponents(arrWorldMatrix.data(), m_arrObjectTranslation.data(), m_arrObjectRotation.data(), m_arrObjectScale.data());
		{
			ImGui::SeparatorText("World SRT");
			{
				if (ImGui::RadioButton("S", mCurrentGizmoOperation == ImGuizmo::SCALE))
				{
					mCurrentGizmoOperation = ImGuizmo::SCALE;
				}
				ImGui::SameLine();
				ImGui::DragFloat3("Scale", m_arrObjectScale.data(), 0.01f);

				if (ImGui::RadioButton("R", mCurrentGizmoOperation == ImGuizmo::ROTATE))
				{
					mCurrentGizmoOperation = ImGuizmo::ROTATE;
				}
				ImGui::SameLine();
				ImGui::DragFloat3("Rotation", m_arrObjectRotation.data(), 0.01f);

				if (ImGui::RadioButton("T", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
				{
					mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
				}
				ImGui::SameLine();
				ImGui::DragFloat3("Translation", m_arrObjectTranslation.data(), 0.01f);
			}
		}
		ImGuizmo::RecomposeMatrixFromComponents(m_arrObjectTranslation.data(), m_arrObjectRotation.data(), m_arrObjectScale.data(), arrWorldMatrix.data());

		const Matrix& matViewMatrix = m_wpCameraManager.lock()->GetCurViewMatrix();
		const Matrix& matProjMatrix = m_wpCameraManager.lock()->GetCurPerspectiveProjectionMatrix();

		array<float, 16> arrViewMatrix =
		{
			matViewMatrix._11, matViewMatrix._12, matViewMatrix._13, matViewMatrix._14,
			matViewMatrix._21, matViewMatrix._22, matViewMatrix._23, matViewMatrix._24,
			matViewMatrix._31, matViewMatrix._32, matViewMatrix._33, matViewMatrix._34,
			matViewMatrix._41, matViewMatrix._42, matViewMatrix._43, matViewMatrix._44,
		};
		array<float, 16> arrProjMatrix =
		{
			matProjMatrix._11, matProjMatrix._12, matProjMatrix._13, matProjMatrix._14,
			matProjMatrix._21, matProjMatrix._22, matProjMatrix._23, matProjMatrix._24,
			matProjMatrix._31, matProjMatrix._32, matProjMatrix._33, matProjMatrix._34,
			matProjMatrix._41, matProjMatrix._42, matProjMatrix._43, matProjMatrix._44,
		};

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(arrViewMatrix.data(), arrProjMatrix.data(), mCurrentGizmoOperation, mCurrentGizmoMode, arrWorldMatrix.data(), NULL, NULL);

		const Matrix matNewWorldMatrix =
		{
			arrWorldMatrix[0],arrWorldMatrix[1],arrWorldMatrix[2],arrWorldMatrix[3],
			arrWorldMatrix[4],arrWorldMatrix[5],arrWorldMatrix[6],arrWorldMatrix[7],
			arrWorldMatrix[8],arrWorldMatrix[9],arrWorldMatrix[10],arrWorldMatrix[11],
			arrWorldMatrix[12],arrWorldMatrix[13],arrWorldMatrix[14],arrWorldMatrix[15],
		};
		wpToolMapObject.lock()->SetTransformMatrix(matNewWorldMatrix);

		//// 여기서	
		Vector3 vWorldPos = { arrWorldMatrix[12], arrWorldMatrix[13], arrWorldMatrix[14] };

		int32 iLeafNodeX = get<0>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(vWorldPos));
		int32 iLeafNodeY = get<1>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(vWorldPos));
		int32 iLeafNodeZ = get<2>(wpToolMapObject.lock()->CalculateOctreeNodeIndex(vWorldPos));

		std::tuple<int32, int32, int32> tpLeafNodeIndex{ iLeafNodeX,iLeafNodeY, iLeafNodeZ };

		wpToolMapObject.lock()->SetOctreeIndex(0);
		wpToolMapObject.lock()->SetOctreeNodeIndex(tpLeafNodeIndex);
		m_arrOctreeIndex = { iLeafNodeX, iLeafNodeY, iLeafNodeZ };
	}
	else
	{
		ImGui::SeparatorText("World SRT");
		{
			if (ImGui::RadioButton("S", mCurrentGizmoOperation == ImGuizmo::SCALE))
			{
				mCurrentGizmoOperation = ImGuizmo::SCALE;
			}
			ImGui::SameLine();
			ImGui::DragFloat3("Scale", m_arrObjectScale.data(), 0.01f);

			if (ImGui::RadioButton("R", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			{
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			}
			ImGui::SameLine();
			ImGui::DragFloat3("Rotation", m_arrObjectRotation.data(), 0.01f);

			if (ImGui::RadioButton("T", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			{
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			}
			ImGui::SameLine();
			ImGui::DragFloat3("Translation", m_arrObjectTranslation.data(), 0.01f);
		}
	}
}

/* Methods */
void ToolMap::CImGuiWindow::SaveDataInto64Json()
{
	vector<vector<vector<multimap<wstring, weak_ptr<CToolMapObject>>>>> vecOctreeNodeDesc;

	vecOctreeNodeDesc.resize(4);
	for (int x = 0; x < 4; ++x)
	{
		vecOctreeNodeDesc[x].resize(4);
		for (int y = 0; y < 4; ++y)
		{
			vecOctreeNodeDesc[x][y].resize(4);
		}
	}

	if (!m_umapTreeListBox.empty())
	{
		for (auto& Tree : m_umapTreeListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Tree.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Tree.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Tree.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Tree", Tree.second.second));
		}
	}
	if (!m_umapRockListBox.empty())
	{
		for (auto& Rock : m_umapRockListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Rock.second.second->GetOctreeNodeIndex())* 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Rock.second.second->GetOctreeNodeIndex())* 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Rock.second.second->GetOctreeNodeIndex())* 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Rock", Rock.second.second));
		}
	}
	if (!m_umapWoodListBox.empty())
	{
		for (auto& Branch : m_umapWoodListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Branch.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Branch.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Branch.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Branch", Branch.second.second));
		}
	}
	if (!m_umapStoneListBox.empty())
	{
		for (auto& Stone : m_umapStoneListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Stone.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Stone.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Stone.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Stone", Stone.second.second));
		}
	}
	if (!m_umapPalListBox.empty())
	{
		for (auto& Pal : m_umapPalListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Pal.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Pal.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Pal.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Pal", Pal.second.second));
		}
	}
	if (!m_umapArchitectureListBox.empty())
	{
		for (auto& Architecture : m_umapArchitectureListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Architecture.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Architecture.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Architecture.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Architecture", Architecture.second.second));
		}
	}
	if (!m_umapSpotLightListBox.empty())
	{
		for (auto& SpotLight : m_umapSpotLightListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(SpotLight.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(SpotLight.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(SpotLight.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"SpotLight", SpotLight.second.second));
		}
	}
	if (!m_umapPointLightListBox.empty())
	{
		for (auto& PointLight : m_umapPointLightListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(PointLight.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(PointLight.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(PointLight.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"PointLight", PointLight.second.second));
		}
	}
	if (!m_umapDirectionalLightListBox.empty())
	{
		for (auto& DirectionalLight : m_umapDirectionalLightListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(DirectionalLight.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(DirectionalLight.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(DirectionalLight.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"DirectionalLight", DirectionalLight.second.second));
		}
	}
	if (!m_umapCameraListBox.empty())
	{
		for (auto& Camera : m_umapCameraListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Camera.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Camera.second.second->GetOctreeNodeIndex()) * 0.5);
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Camera.second.second->GetOctreeNodeIndex()) * 0.5);

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 3) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 3) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 3) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Camera", Camera.second.second));
		}
	}

	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			for (int z = 0; z < 4; ++z)
			{
				rapidjson::StringBuffer buffer;
				rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

				writer.StartObject();
				{
					if (0 == vecOctreeNodeDesc[x][y][z].size())
					{
						writer.Key("Is Empty?");
						writer.Bool(true);
					}
					else
					{
						writer.Key("Is Empty?");
						writer.Bool(false);

						int32 iTreeCount = 0;
						int32 iRockCount = 0;
						int32 iBranchCount = 0;
						int32 iStoneCount = 0;
						int32 iPalCount = 0;
						int32 iArchitectureCount = 0;
						int32 iSpotLightCount = 0;
						int32 iPointLightCount = 0;
						int32 iDirectionalLightCount = 0;
						int32 iCameraCount = 0;

						for (const auto& pair : vecOctreeNodeDesc[x][y][z])
						{
							if (pair.first == L"Tree") { iTreeCount++; }
							else if (pair.first == L"Rock") { iRockCount++; }
							else if (pair.first == L"Branch") { iBranchCount++; }
							else if (pair.first == L"Stone") { iStoneCount++; }
							else if (pair.first == L"Pal") { iPalCount++; }
							else if (pair.first == L"Architecture") { iArchitectureCount++; }
							else if (pair.first == L"SpotLight") { iSpotLightCount++; }
							else if (pair.first == L"PointLight") { iPointLightCount++; }
							else if (pair.first == L"DirectionalLight") { iDirectionalLightCount++; }
							else if (pair.first == L"Camera") { iCameraCount++; }
						}

						writer.Key("Tree Count");
						writer.Int(iTreeCount);
						writer.Key("Rock Count");
						writer.Int(iRockCount);
						writer.Key("Branch Count");
						writer.Int(iBranchCount);
						writer.Key("Stone Count");
						writer.Int(iStoneCount);
						writer.Key("Pal Count");
						writer.Int(iPalCount);
						writer.Key("Architecture Count");
						writer.Int(iArchitectureCount);
						writer.Key("SpotLight Count");
						writer.Int(iSpotLightCount);
						writer.Key("PointLight Count");
						writer.Int(iPointLightCount);
						writer.Key("DirectionalLight Count");
						writer.Int(iDirectionalLightCount);
						writer.Key("Camera Count");
						writer.Int(iCameraCount);

						if (0 != iTreeCount)
						{
							writer.Key("Trees Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_iHabitatType);
								writer.Key("Nature Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_iNatureType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_iColliderType);
								writer.Key("CurHp");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_fCurHp);

								writer.Key("Total Number of Trees");
								writer.Int(iTreeCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Tree")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = L"Tree" + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Center");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vCenter.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vCenter.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vCenter.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Extents");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vExtents.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vExtents.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vExtents.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetTreeCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetTreeCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetTreeCache().m_tpOctreeNodeIndex) * 0.5));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetTreeCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iRockCount)
						{
							writer.Key("Rocks Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_iHabitatType);
								writer.Key("Nature Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_iNatureType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_iColliderType);
								writer.Key("CurHp");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_fCurHp);

								writer.Key("Total Number of Rocks");
								writer.Int(iRockCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Rock")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = L"Rock" + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Center");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vCenter.x);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vCenter.y);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vCenter.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Radius");
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_fRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetRockCache().m_tpOctreeNodeIndex)* 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetRockCache().m_tpOctreeNodeIndex)* 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetRockCache().m_tpOctreeNodeIndex)* 0.5));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetRockCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iBranchCount)
						{
							writer.Key("Branches Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_iHabitatType);
								writer.Key("Nature Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_iNatureType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_iColliderType);

								writer.Key("Center");
								writer.StartArray();
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_vCenter.x);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_vCenter.y);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_vCenter.z);
								writer.EndArray();

								writer.Key("Radius");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_fRadius);
								writer.Key("HalfHeight");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_fHalfHeight);

								writer.Key("Total Number of Branches");
								writer.Int(iBranchCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Branch")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetBranchCache().m_tpOctreeNodeIndex)* 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetBranchCache().m_tpOctreeNodeIndex)* 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetBranchCache().m_tpOctreeNodeIndex)* 0.5));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetBranchCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iStoneCount)
						{
							writer.Key("Stones Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_iHabitatType);
								writer.Key("Nature Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_iNatureType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_iColliderType);

								writer.Key("Center");
								writer.StartArray();
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_vCenter.x);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_vCenter.y);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_vCenter.z);
								writer.EndArray();

								writer.Key("Radius");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_fRadius);

								writer.Key("Total Number of Stones");
								writer.Int(iStoneCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Stone")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetStoneCache().m_tpOctreeNodeIndex)* 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetStoneCache().m_tpOctreeNodeIndex)* 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetStoneCache().m_tpOctreeNodeIndex)* 0.5));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetStoneCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();

						}

						if (0 != iPalCount)
						{
							writer.Key("Pals Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_iHabitatType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_iColliderType);

								writer.Key("Total Number of Pals");
								writer.Int(iPalCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Pal")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Radius");
													writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_fRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Center");
													writer.StartArray();
													writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_vCenter.x);
													writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_vCenter.y);
													writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_vCenter.z);
													writer.EndArray(); 
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetPalCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetPalCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetPalCache().m_tpOctreeNodeIndex) * 0.5));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetPalCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();

						}

						if (0 != iArchitectureCount)
						{
							writer.Key("Architectures Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_iHabitatType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_bTrigger); 
								writer.Key("Extent");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_fExtent);
								
								writer.Key("Center");
								writer.StartArray();
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_vCenter.x);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_vCenter.y);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_vCenter.z);
								writer.EndArray();
								
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_iColliderType);

								writer.Key("Total Number of Architectures");
								writer.Int(iArchitectureCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Architecture")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetArchitectureCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetArchitectureCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetArchitectureCache().m_tpOctreeNodeIndex) * 0.5));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetArchitectureCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iSpotLightCount)
						{
							writer.Key("SpotLights Information");
							writer.StartObject();
							{
								writer.Key("Model File Path");
								writer.String(CStringUtility::ToString(vecOctreeNodeDesc[x][y][z].find(L"SpotLight")->second.lock()->GetSpotLightCache().m_wstrModelFilePath).c_str());
								
								writer.Key("Total Number of SpotLights");
								writer.Int(iSpotLightCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"SpotLight")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Radius");
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_fRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("CosAngle");
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_fCosAngle);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Energy");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vEnergy.x);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vEnergy.y);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vEnergy.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Position");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vPosition.x);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vPosition.y);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vPosition.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Direction");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vDirection.x);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vDirection.y);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vDirection.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetSpotLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetSpotLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetSpotLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.EndArray();
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iPointLightCount)
						{
							writer.Key("PointLights Information");
							writer.StartObject();
							{
								writer.Key("Model File Path");
								writer.String(CStringUtility::ToString(vecOctreeNodeDesc[x][y][z].find(L"PointLight")->second.lock()->GetPointLightCache().m_wstrModelFilePath).c_str());

								writer.Key("Total Number of PointLights ");
								writer.Int(iPointLightCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"PointLight")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Radius");
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_fRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Energy");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vEnergy.x);
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vEnergy.y);
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vEnergy.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Position");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vPosition.x);
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vPosition.y);
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vPosition.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetPointLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetPointLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetPointLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.EndArray();
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iDirectionalLightCount)
						{
							writer.Key("DirectionalLights Information");
							writer.StartObject();
							{
								writer.Key("Model File Path");
								writer.String(CStringUtility::ToString(vecOctreeNodeDesc[x][y][z].find(L"DirectionalLight")->second.lock()->GetDirectionalLightCache().m_wstrModelFilePath).c_str());

								writer.Key("Total Number of DirectionalLights ");
								writer.Int(iDirectionalLightCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"DirectionalLight")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("SolidAngle");
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_fSolidAngle);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("PerceivedRadius");
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_fPerceivedRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("PerceivedDistance");
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_fPerceivedDistance);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Energy");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vEnergy.x);
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vEnergy.y);
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vEnergy.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Direction");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vDirection.x);
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vDirection.y);
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vDirection.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetDirectionalLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetDirectionalLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetDirectionalLightCache().m_tpOctreeNodeIndex) * 0.5));
													writer.EndArray();
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iCameraCount)
						{
							writer.Key("Cameras Information");
							writer.StartObject();
							{
								writer.Key("Model File Path");
								writer.String(CStringUtility::ToString(vecOctreeNodeDesc[x][y][z].find(L"Camera")->second.lock()->GetCameraCache().m_wstrModelFilePath).c_str());

								writer.Key("Total Number of Cameras ");
								writer.Int(iCameraCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Camera")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("RightSlope");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fRightSlope);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("LeftSlope");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fLeftSlope);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("TopSlope");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fTopSlope);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("BottomSlope");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fBottomSlope);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("NearPlane");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fNearPlane);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("FarPlane");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fFarPlane);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Center");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vCenter.x);
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vCenter.y);
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vCenter.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Radian");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vRadian.x);
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vRadian.y);
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vRadian.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(static_cast<int32>(get<0>(umapGameObject.second.lock()->GetCameraCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<1>(umapGameObject.second.lock()->GetCameraCache().m_tpOctreeNodeIndex) * 0.5));
													writer.Int(static_cast<int32>(get<2>(umapGameObject.second.lock()->GetCameraCache().m_tpOctreeNodeIndex) * 0.5));
													writer.EndArray();
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

					}
				}
				writer.EndObject();

				ofstream ofs("../../Resource/Models/Dat/MapMesh/Prototype/Octree4/" + m_strSaveLoadFileName + "_" + to_string(x) + to_string(y) + to_string(z) + ".json");
				if (!ofs.is_open()) { return; }
				CHECKF(true, L"Save Completed");
				ofs << buffer.GetString();
				ofs.close();
			}
		}
	}
}

void ToolMap::CImGuiWindow::LoadDataFrom64Json()
{
	// 이번튼 누르면, MSGBox 뜨고, text창 안에 있는 글자 지우기
	// 1. JSON문서를 읽기 모드로 연다.
	ifstream isJsonpath(L"../../Resource/Models/Dat/MapMesh/Prototype/Octree4/" + m_wstrSaveLoadFileName + L".json");
	if (!isJsonpath.is_open())
	{
		throw runtime_error("failed to open json file");
	}

	// 2. 문서 내용을 문자열로 변환.
	string strJsonfile((istreambuf_iterator<char>(isJsonpath)), (istreambuf_iterator<char>()));

	// 3. JSON문서의 문자열을 파싱해온다.
	rapidjson::Document doc;
	doc.Parse(strJsonfile.c_str());

	// 4. JSON문서로부터 데이터를 읽는다.
	rapidjson::Value& bIsEmpty = doc["Is Empty?"];
	bool bIsFileEmpty = bIsEmpty.GetBool();

	if (!bIsFileEmpty)
	{
		rapidjson::Value& iTreeCount = doc["Tree Count"];
		rapidjson::Value& iRockCount = doc["Rock Count"];
		rapidjson::Value& iBranchCount = doc["Branch Count"];
		rapidjson::Value& iStoneCount = doc["Stone Count"];
		rapidjson::Value& iPalCount = doc["Pal Count"];
		rapidjson::Value& iArchitectureCount = doc["Architecture Count"];
		rapidjson::Value& iSpotLightCount = doc["SpotLight Count"];
		rapidjson::Value& iPointLightCount = doc["PointLight Count"];
		rapidjson::Value& iDirectionalLightCount = doc["DirectionalLight Count"];
		rapidjson::Value& iCameraCount = doc["Camera Count"];


		int32 iNumTreeCount = iTreeCount.GetInt();
		int32 iNumRockCount = iRockCount.GetInt();
		int32 iNumBranchCount = iBranchCount.GetInt();
		int32 iNumStoneCount = iStoneCount.GetInt();
		int32 iNumPalCount = iPalCount.GetInt();
		int32 iNumArchitectureCount = iArchitectureCount.GetInt();
		int32 iNumSpotLightCount = iSpotLightCount.GetInt();
		int32 iNumPointLightCount = iPointLightCount.GetInt();
		int32 iNumDirectionalLightCount = iDirectionalLightCount.GetInt();
		int32 iNumCameraCount = iCameraCount.GetInt();

		if (0 != iNumTreeCount)
		{
			rapidjson::Value& arrTree = doc["Trees Information"];

			FTreeCache tTreeCache{};

			tTreeCache.m_iHabitatType = arrTree["Habitat Type"].GetInt();
			tTreeCache.m_iNatureType = arrTree["Nature Type"].GetInt();
			tTreeCache.m_bTrigger = arrTree["Is Trigger"].GetBool();
			tTreeCache.m_iColliderType = arrTree["Collider Type"].GetInt();
			tTreeCache.m_fCurHp = arrTree["CurHp"].GetFloat();

			int32 iTotalTreeCount = arrTree["Total Number of Trees"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrTree["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalTreeCount; iIndex++)
			{
				wstring wstrTreeID = to_wstring(iIndex);
				wstring wstrTreeName = L"Tree" + wstrTreeID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrTreeName).c_str()];

				tTreeCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tTreeCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tTreeCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tTreeCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tTreeCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tTreeCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tTreeCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tTreeCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tTreeCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				tTreeCache.m_vCenter.x = arrChildDesc[3]["Center"][0].GetFloat();
				tTreeCache.m_vCenter.y = arrChildDesc[3]["Center"][1].GetFloat();
				tTreeCache.m_vCenter.z = arrChildDesc[3]["Center"][2].GetFloat();

				tTreeCache.m_vExtents.x = arrChildDesc[4]["Extents"][0].GetFloat();
				tTreeCache.m_vExtents.y = arrChildDesc[4]["Extents"][1].GetFloat();
				tTreeCache.m_vExtents.z = arrChildDesc[4]["Extents"][2].GetFloat();

				get<0>(tTreeCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tTreeCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tTreeCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				tTreeCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[6]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumRockCount)
		{
			rapidjson::Value& arrRock = doc["Rocks Information"];

			FRockCache tRockCache{};

			tRockCache.m_iHabitatType = arrRock["Habitat Type"].GetInt();
			tRockCache.m_iNatureType = arrRock["Nature Type"].GetInt();
			tRockCache.m_bTrigger = arrRock["Is Trigger"].GetBool();
			tRockCache.m_iColliderType = arrRock["Collider Type"].GetInt();
			tRockCache.m_fCurHp = arrRock["CurHp"].GetFloat();

			int32 iTotalRockCount = arrRock["Total Number of Rocks"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrRock["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalRockCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Rock" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tRockCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tRockCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tRockCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tRockCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tRockCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tRockCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tRockCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tRockCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tRockCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				tRockCache.m_vCenter.x = arrChildDesc[3]["Center"][0].GetFloat();
				tRockCache.m_vCenter.y = arrChildDesc[3]["Center"][1].GetFloat();
				tRockCache.m_vCenter.z = arrChildDesc[3]["Center"][2].GetFloat();

				tRockCache.m_fRadius = arrChildDesc[4]["Radius"].GetFloat();

				get<0>(tRockCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tRockCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tRockCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				tRockCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[6]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumBranchCount)
		{
			rapidjson::Value& arrBranch = doc["Branches Information"];

			FBranchCache tBranchCache{};

			tBranchCache.m_iHabitatType = arrBranch["Habitat Type"].GetInt();
			tBranchCache.m_iNatureType = arrBranch["Nature Type"].GetInt();
			tBranchCache.m_bTrigger = arrBranch["Is Trigger"].GetBool();
			tBranchCache.m_iColliderType = arrBranch["Collider Type"].GetInt();
			tBranchCache.m_vCenter.x = arrBranch["Center"][0].GetFloat();
			tBranchCache.m_vCenter.y = arrBranch["Center"][1].GetFloat();
			tBranchCache.m_vCenter.z = arrBranch["Center"][2].GetFloat();
			tBranchCache.m_fRadius = arrBranch["Radius"].GetFloat();
			tBranchCache.m_fHalfHeight = arrBranch["HalfHeight"].GetFloat();

			int32 iTotalBranchCount = arrBranch["Total Number of Branches"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrBranch["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalBranchCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Branch" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tBranchCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tBranchCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tBranchCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tBranchCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tBranchCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tBranchCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tBranchCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tBranchCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tBranchCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				get<0>(tBranchCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][0].GetInt();
				get<1>(tBranchCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][1].GetInt();
				get<2>(tBranchCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][2].GetInt();

				tBranchCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[4]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumStoneCount)
		{
			rapidjson::Value& arrStone = doc["Stones Information"];

			FStoneCache tStoneCache{};

			tStoneCache.m_iHabitatType = arrStone["Habitat Type"].GetInt();
			tStoneCache.m_iNatureType = arrStone["Nature Type"].GetInt();
			tStoneCache.m_bTrigger = arrStone["Is Trigger"].GetBool();
			tStoneCache.m_iColliderType = arrStone["Collider Type"].GetInt();
			tStoneCache.m_vCenter.x = arrStone["Center"][0].GetFloat();
			tStoneCache.m_vCenter.y = arrStone["Center"][1].GetFloat();
			tStoneCache.m_vCenter.z = arrStone["Center"][2].GetFloat();
			tStoneCache.m_fRadius = arrStone["Radius"].GetFloat();

			int32 iTotalStoneCount = arrStone["Total Number of Stones"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrStone["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalStoneCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Stone" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tStoneCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tStoneCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tStoneCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tStoneCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tStoneCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tStoneCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tStoneCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tStoneCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tStoneCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				get<0>(tStoneCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][0].GetInt();
				get<1>(tStoneCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][1].GetInt();
				get<2>(tStoneCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][2].GetInt();

				tStoneCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[4]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumPalCount)
		{
			rapidjson::Value& arrPal = doc["Pals Information"];

			FPalCache tPalCache{};

			tPalCache.m_iHabitatType = arrPal["Habitat Type"].GetInt();
			tPalCache.m_bTrigger = arrPal["Is Trigger"].GetBool();
			tPalCache.m_iColliderType = arrPal["Collider Type"].GetInt();

			int32 iTotalPalCount = arrPal["Total Number of Pals"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrPal["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalPalCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Pal" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tPalCache.m_fRadius = arrChildDesc[0]["Radius"].GetFloat();

				tPalCache.m_vCenter.x = arrChildDesc[1]["Center"][0].GetFloat();
				tPalCache.m_vCenter.y = arrChildDesc[1]["Center"][1].GetFloat();
				tPalCache.m_vCenter.z = arrChildDesc[1]["Center"][2].GetFloat();

				tPalCache.m_vScale.x = arrChildDesc[2]["Scale"][0].GetFloat();
				tPalCache.m_vScale.y = arrChildDesc[2]["Scale"][1].GetFloat();
				tPalCache.m_vScale.z = arrChildDesc[2]["Scale"][2].GetFloat();

				tPalCache.m_vRotate.x = arrChildDesc[3]["Rotation"][0].GetFloat();
				tPalCache.m_vRotate.y = arrChildDesc[3]["Rotation"][1].GetFloat();
				tPalCache.m_vRotate.z = arrChildDesc[3]["Rotation"][2].GetFloat();

				tPalCache.m_vTranslate.x = arrChildDesc[4]["Translation"][0].GetFloat();
				tPalCache.m_vTranslate.y = arrChildDesc[4]["Translation"][1].GetFloat();
				tPalCache.m_vTranslate.z = arrChildDesc[4]["Translation"][2].GetFloat();

				get<0>(tPalCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tPalCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tPalCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				tPalCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[6]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumArchitectureCount)
		{
			rapidjson::Value& arrArchitecture = doc["Architectures Information"];

			FArchitectureCache tArchitectureCache{};

			tArchitectureCache.m_iHabitatType = arrArchitecture["Habitat Type"].GetInt();
			tArchitectureCache.m_bTrigger = arrArchitecture["Is Trigger"].GetBool();

			tArchitectureCache.m_fExtent = arrArchitecture["Extent"].GetFloat();

			tArchitectureCache.m_vCenter.x = arrArchitecture["Center"][0].GetFloat();
			tArchitectureCache.m_vCenter.y = arrArchitecture["Center"][1].GetFloat();
			tArchitectureCache.m_vCenter.z = arrArchitecture["Center"][2].GetFloat();

			tArchitectureCache.m_iColliderType = arrArchitecture["Collider Type"].GetInt();

			int32 iTotalArchitectureCount = arrArchitecture["Total Number of Architectures"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrArchitecture["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalArchitectureCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Architecture" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tArchitectureCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tArchitectureCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tArchitectureCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tArchitectureCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tArchitectureCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tArchitectureCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tArchitectureCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tArchitectureCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tArchitectureCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				get<0>(tArchitectureCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][0].GetInt();
				get<1>(tArchitectureCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][1].GetInt();
				get<2>(tArchitectureCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][2].GetInt();

				tArchitectureCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[4]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumSpotLightCount)
		{
			rapidjson::Value& arrSpotLight = doc["SpotLights Information"];

			FSpotLightCache tSpotLightCache{};

			tSpotLightCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrSpotLight["Model File Path"].GetString());

			int32 iTotalSpotLightCount = arrSpotLight["Total Number of SpotLights"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrSpotLight["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalSpotLightCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"SpotLight" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tSpotLightCache.m_fRadius = arrChildDesc[0]["Radius"].GetFloat();

				tSpotLightCache.m_fCosAngle = arrChildDesc[1]["CosAngle"].GetFloat();

				tSpotLightCache.m_vEnergy.x = arrChildDesc[2]["Energy"][0].GetFloat();
				tSpotLightCache.m_vEnergy.y = arrChildDesc[2]["Energy"][1].GetFloat();
				tSpotLightCache.m_vEnergy.z = arrChildDesc[2]["Energy"][2].GetFloat();

				tSpotLightCache.m_vPosition.x = arrChildDesc[3]["Position"][0].GetFloat();
				tSpotLightCache.m_vPosition.y = arrChildDesc[3]["Position"][1].GetFloat();
				tSpotLightCache.m_vPosition.z = arrChildDesc[3]["Position"][2].GetFloat();

				tSpotLightCache.m_vDirection.x = arrChildDesc[4]["Direction"][0].GetFloat();
				tSpotLightCache.m_vDirection.y = arrChildDesc[4]["Direction"][1].GetFloat();
				tSpotLightCache.m_vDirection.z = arrChildDesc[4]["Direction"][2].GetFloat();

				get<0>(tSpotLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tSpotLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tSpotLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumPointLightCount)
		{
			rapidjson::Value& arrPointLight = doc["PointLights Information"];

			FPointLightCache tPointLightCache{};

			tPointLightCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrPointLight["Model File Path"].GetString());

			int32 iTotalPointLightCount = arrPointLight["Total Number of PointLights"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrPointLight["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalPointLightCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"PointLight" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tPointLightCache.m_fRadius = arrChildDesc[0]["Radius"].GetFloat();

				tPointLightCache.m_vEnergy.x = arrChildDesc[1]["Energy"][0].GetFloat();
				tPointLightCache.m_vEnergy.y = arrChildDesc[1]["Energy"][1].GetFloat();
				tPointLightCache.m_vEnergy.z = arrChildDesc[1]["Energy"][2].GetFloat();

				tPointLightCache.m_vPosition.x = arrChildDesc[2]["Position"][0].GetFloat();
				tPointLightCache.m_vPosition.y = arrChildDesc[2]["Position"][1].GetFloat();
				tPointLightCache.m_vPosition.z = arrChildDesc[2]["Position"][2].GetFloat();

				get<0>(tPointLightCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][0].GetInt();
				get<1>(tPointLightCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][1].GetInt();
				get<2>(tPointLightCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][2].GetInt();

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumDirectionalLightCount)
		{
			rapidjson::Value& arrDirectionalLight = doc["DirectionalLights Information"];

			FDirectionalLightCache tDirectionalLightCache{};

			tDirectionalLightCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrDirectionalLight["Model File Path"].GetString());

			int32 iTotalDirectionalLightCount = arrDirectionalLight["Total Number of DirectionalLights"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrDirectionalLight["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalDirectionalLightCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"DirectionalLight" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tDirectionalLightCache.m_fSolidAngle = arrChildDesc[0]["SolidAngle"].GetFloat();
				tDirectionalLightCache.m_fPerceivedRadius = arrChildDesc[1]["PerceivedRadius"].GetFloat();
				tDirectionalLightCache.m_fPerceivedDistance = arrChildDesc[2]["PerceivedDistance"].GetFloat();

				tDirectionalLightCache.m_vEnergy.x = arrChildDesc[3]["Energy"][0].GetFloat();
				tDirectionalLightCache.m_vEnergy.y = arrChildDesc[3]["Energy"][1].GetFloat();
				tDirectionalLightCache.m_vEnergy.z = arrChildDesc[3]["Energy"][2].GetFloat();

				tDirectionalLightCache.m_vDirection.x = arrChildDesc[4]["Direction"][0].GetFloat();
				tDirectionalLightCache.m_vDirection.y = arrChildDesc[4]["Direction"][1].GetFloat();
				tDirectionalLightCache.m_vDirection.z = arrChildDesc[4]["Direction"][2].GetFloat();

				get<0>(tDirectionalLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tDirectionalLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tDirectionalLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumCameraCount)
		{
			rapidjson::Value& arrCamera = doc["Cameras Information"];

			FCameraCache tCameraCache{};

			tCameraCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrCamera["Model File Path"].GetString());

			int32 iTotalCameraCount = arrCamera["Total Number of Cameras"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrCamera["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalCameraCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Camera" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tCameraCache.m_fRightSlope = arrChildDesc[0]["RightSlope"].GetFloat();
				tCameraCache.m_fLeftSlope = arrChildDesc[1]["LeftSlope"].GetFloat();
				tCameraCache.m_fTopSlope = arrChildDesc[2]["TopSlope"].GetFloat();
				tCameraCache.m_fBottomSlope = arrChildDesc[3]["BottomSlope"].GetFloat();
				tCameraCache.m_fNearPlane = arrChildDesc[4]["NearPlane"].GetFloat();
				tCameraCache.m_fFarPlane = arrChildDesc[5]["FarPlane"].GetFloat();

				tCameraCache.m_vCenter.x = arrChildDesc[6]["Center"][0].GetFloat();
				tCameraCache.m_vCenter.y = arrChildDesc[6]["Center"][1].GetFloat();
				tCameraCache.m_vCenter.z = arrChildDesc[6]["Center"][2].GetFloat();

				tCameraCache.m_vRadian.x = arrChildDesc[7]["Radian"][0].GetFloat();
				tCameraCache.m_vRadian.y = arrChildDesc[7]["Radian"][1].GetFloat();
				tCameraCache.m_vRadian.z = arrChildDesc[7]["Radian"][2].GetFloat();

				get<0>(tCameraCache.m_tpOctreeNodeIndex) = arrChildDesc[8]["Included Octree Node Index"][0].GetInt();
				get<1>(tCameraCache.m_tpOctreeNodeIndex) = arrChildDesc[8]["Included Octree Node Index"][1].GetInt();
				get<2>(tCameraCache.m_tpOctreeNodeIndex) = arrChildDesc[8]["Included Octree Node Index"][2].GetInt();

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		CHECKF(true, L"Save Completed");
	}
	else { CHECKF(true, L"Load Failed"); }
}

void ToolMap::CImGuiWindow::SaveDataInto512Json()
{
	vector<vector<vector<multimap<wstring, weak_ptr<CToolMapObject>>>>> vecOctreeNodeDesc;

	vecOctreeNodeDesc.resize(8);
	for (int x = 0; x < 8; ++x)
	{
		vecOctreeNodeDesc[x].resize(8);
		for (int y = 0; y < 8; ++y)
		{
			vecOctreeNodeDesc[x][y].resize(8);
		}
	}

	if (!m_umapTreeListBox.empty())
	{
		for (auto& Tree : m_umapTreeListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Tree.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Tree.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Tree.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Tree", Tree.second.second));
		}
	}
	if (!m_umapRockListBox.empty())
	{
		for (auto& Rock : m_umapRockListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Rock.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Rock.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Rock.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Rock", Rock.second.second));
		}
	}
	if (!m_umapWoodListBox.empty())
	{
		for (auto& Branch : m_umapWoodListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Branch.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Branch.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Branch.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Branch", Branch.second.second));
		}
	}
	if (!m_umapStoneListBox.empty())
	{
		for (auto& Stone : m_umapStoneListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Stone.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Stone.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Stone.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Stone", Stone.second.second));
		}
	}
	if (!m_umapPalListBox.empty())
	{
		for (auto& Pal : m_umapPalListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Pal.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Pal.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Pal.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Pal", Pal.second.second));
		}
	}
	if (!m_umapArchitectureListBox.empty())
	{
		for (auto& Architecture : m_umapArchitectureListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Architecture.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Architecture.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Architecture.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Architecture", Architecture.second.second));
		}
	}
	if (!m_umapSpotLightListBox.empty())
	{
		for (auto& SpotLight : m_umapSpotLightListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(SpotLight.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(SpotLight.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(SpotLight.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"SpotLight", SpotLight.second.second));
		}
	}
	if (!m_umapPointLightListBox.empty())
	{
		for (auto& PointLight : m_umapPointLightListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(PointLight.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(PointLight.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(PointLight.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"PointLight", PointLight.second.second));
		}
	}
	if (!m_umapDirectionalLightListBox.empty())
	{
		for (auto& DirectionalLight : m_umapDirectionalLightListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(DirectionalLight.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(DirectionalLight.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(DirectionalLight.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"DirectionalLight", DirectionalLight.second.second));
		}
	}
	if (!m_umapCameraListBox.empty())
	{
		for (auto& Camera : m_umapCameraListBox)
		{
			int32 iOctreeNodeIndexX = static_cast<int32>(get<0>(Camera.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexY = static_cast<int32>(get<1>(Camera.second.second->GetOctreeNodeIndex()));
			int32 iOctreeNodeIndexZ = static_cast<int32>(get<2>(Camera.second.second->GetOctreeNodeIndex()));

			if (iOctreeNodeIndexX < 0 || iOctreeNodeIndexX > 7) { continue; }
			if (iOctreeNodeIndexY < 0 || iOctreeNodeIndexY > 7) { continue; }
			if (iOctreeNodeIndexZ < 0 || iOctreeNodeIndexZ > 7) { continue; }
			vecOctreeNodeDesc[iOctreeNodeIndexX][iOctreeNodeIndexY][iOctreeNodeIndexZ].insert(make_pair(L"Camera", Camera.second.second));
		}
	}

	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			for (int z = 0; z < 8; ++z)
			{
				rapidjson::StringBuffer buffer;
				rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

				writer.StartObject();
				{
					if (0 == vecOctreeNodeDesc[x][y][z].size())
					{
						writer.Key("Is Empty?");
						writer.Bool(true);
					}
					else
					{
						writer.Key("Is Empty?");
						writer.Bool(false);

						int32 iTreeCount = 0;
						int32 iRockCount = 0;
						int32 iBranchCount = 0;
						int32 iStoneCount = 0;
						int32 iPalCount = 0;
						int32 iArchitectureCount = 0;
						int32 iSpotLightCount = 0;
						int32 iPointLightCount = 0;
						int32 iDirectionalLightCount = 0;
						int32 iCameraCount = 0;

						for (const auto& pair : vecOctreeNodeDesc[x][y][z])
						{
							if (pair.first == L"Tree") { iTreeCount++; }
							else if (pair.first == L"Rock") { iRockCount++; }
							else if (pair.first == L"Branch") { iBranchCount++; }
							else if (pair.first == L"Stone") { iStoneCount++; }
							else if (pair.first == L"Pal") { iPalCount++; }
							else if (pair.first == L"Architecture") { iArchitectureCount++; }
							else if (pair.first == L"SpotLight") { iSpotLightCount++; }
							else if (pair.first == L"PointLight") { iPointLightCount++; }
							else if (pair.first == L"DirectionalLight") { iDirectionalLightCount++; }
							else if (pair.first == L"Camera") { iCameraCount++; }
						}

						writer.Key("Tree Count");
						writer.Int(iTreeCount);
						writer.Key("Rock Count");
						writer.Int(iRockCount);
						writer.Key("Branch Count");
						writer.Int(iBranchCount);
						writer.Key("Stone Count");
						writer.Int(iStoneCount);
						writer.Key("Pal Count");
						writer.Int(iPalCount);
						writer.Key("Architecture Count");
						writer.Int(iArchitectureCount);
						writer.Key("SpotLight Count");
						writer.Int(iSpotLightCount);
						writer.Key("PointLight Count");
						writer.Int(iPointLightCount);
						writer.Key("DirectionalLight Count");
						writer.Int(iDirectionalLightCount);
						writer.Key("Camera Count");
						writer.Int(iCameraCount);

						if (0 != iTreeCount)
						{
							writer.Key("Trees Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_iHabitatType);
								writer.Key("Nature Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_iNatureType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_iColliderType);
								writer.Key("CurHp");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Tree")->second.lock()->GetTreeCache().m_fCurHp);

								writer.Key("Total Number of Trees");
								writer.Int(iTreeCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Tree")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = L"Tree" + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Center");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vCenter.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vCenter.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vCenter.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Extents");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vExtents.x);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vExtents.y);
													writer.Double(umapGameObject.second.lock()->GetTreeCache().m_vExtents.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetTreeCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetTreeCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetTreeCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetTreeCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iRockCount)
						{
							writer.Key("Rocks Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_iHabitatType);
								writer.Key("Nature Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_iNatureType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_iColliderType);
								writer.Key("CurHp");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Rock")->second.lock()->GetRockCache().m_fCurHp);

								writer.Key("Total Number of Rocks");
								writer.Int(iRockCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Rock")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = L"Rock" + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Center");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vCenter.x);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vCenter.y);
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_vCenter.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Radius");
													writer.Double(umapGameObject.second.lock()->GetRockCache().m_fRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetRockCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetRockCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetRockCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetRockCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iBranchCount)
						{
							writer.Key("Branches Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_iHabitatType);
								writer.Key("Nature Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_iNatureType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_iColliderType);

								writer.Key("Center");
								writer.StartArray();
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_vCenter.x);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_vCenter.y);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_vCenter.z);
								writer.EndArray();

								writer.Key("Radius");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_fRadius);
								writer.Key("HalfHeight");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Branch")->second.lock()->GetBranchCache().m_fHalfHeight);

								writer.Key("Total Number of Branches");
								writer.Int(iBranchCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Branch")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetBranchCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetBranchCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetBranchCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetBranchCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetBranchCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iStoneCount)
						{
							writer.Key("Stones Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_iHabitatType);
								writer.Key("Nature Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_iNatureType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_iColliderType);

								writer.Key("Center");
								writer.StartArray();
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_vCenter.x);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_vCenter.y);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_vCenter.z);
								writer.EndArray();

								writer.Key("Radius");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Stone")->second.lock()->GetStoneCache().m_fRadius);

								writer.Key("Total Number of Stones");
								writer.Int(iTreeCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Stone")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetStoneCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetStoneCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetStoneCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetStoneCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetStoneCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();

						}

						if (0 != iPalCount)
						{
							writer.Key("Pals Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_iHabitatType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_bTrigger);
								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_iColliderType);

								writer.Key("Total Number of Pals");
								writer.Int(iPalCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Pal")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Radius");
													writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_fRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Center");
													writer.StartArray();
													writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_vCenter.x);
													writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_vCenter.y);
													writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Pal")->second.lock()->GetPalCache().m_vCenter.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetPalCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetPalCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetPalCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetPalCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetPalCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();

						}

						if (0 != iArchitectureCount)
						{
							writer.Key("Architectures Information");
							writer.StartObject();
							{
								writer.Key("Habitat Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_iHabitatType);
								writer.Key("Is Trigger");
								writer.Bool(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_bTrigger);
								writer.Key("Extent");
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_fExtent);

								writer.Key("Center");
								writer.StartArray();
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_vCenter.x);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_vCenter.y);
								writer.Double(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_vCenter.z);
								writer.EndArray();

								writer.Key("Collider Type");
								writer.Int(vecOctreeNodeDesc[x][y][z].find(L"Architecture")->second.lock()->GetArchitectureCache().m_iColliderType);

								writer.Key("Total Number of Architectures");
								writer.Int(iArchitectureCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Architecture")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Scale");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vScale.x);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vScale.y);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vScale.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Rotation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vRotate.x);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vRotate.y);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vRotate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Translation");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vTranslate.x);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vTranslate.y);
													writer.Double(umapGameObject.second.lock()->GetArchitectureCache().m_vTranslate.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetArchitectureCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetArchitectureCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetArchitectureCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Model File Path");
													writer.String(CStringUtility::ToString(umapGameObject.second.lock()->GetArchitectureCache().m_wstrModelFilePath).c_str());
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iSpotLightCount)
						{
							writer.Key("SpotLights Information");
							writer.StartObject();
							{
								writer.Key("Model File Path");
								writer.String(CStringUtility::ToString(vecOctreeNodeDesc[x][y][z].find(L"SpotLight")->second.lock()->GetSpotLightCache().m_wstrModelFilePath).c_str());

								writer.Key("Total Number of SpotLights");
								writer.Int(iSpotLightCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"SpotLight")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Radius");
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_fRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("CosAngle");
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_fCosAngle);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Energy");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vEnergy.x);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vEnergy.y);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vEnergy.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Position");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vPosition.x);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vPosition.y);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vPosition.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Direction");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vDirection.x);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vDirection.y);
													writer.Double(umapGameObject.second.lock()->GetSpotLightCache().m_vDirection.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetSpotLightCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetSpotLightCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetSpotLightCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iPointLightCount)
						{
							writer.Key("PointLights Information");
							writer.StartObject();
							{
								writer.Key("Model File Path");
								writer.String(CStringUtility::ToString(vecOctreeNodeDesc[x][y][z].find(L"PointLight")->second.lock()->GetPointLightCache().m_wstrModelFilePath).c_str());

								writer.Key("Total Number of PointLights ");
								writer.Int(iPointLightCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"PointLight")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("Radius");
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_fRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Energy");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vEnergy.x);
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vEnergy.y);
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vEnergy.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Position");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vPosition.x);
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vPosition.y);
													writer.Double(umapGameObject.second.lock()->GetPointLightCache().m_vPosition.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetPointLightCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetPointLightCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetPointLightCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iDirectionalLightCount)
						{
							writer.Key("DirectionalLights Information");
							writer.StartObject();
							{
								writer.Key("Model File Path");
								writer.String(CStringUtility::ToString(vecOctreeNodeDesc[x][y][z].find(L"DirectionalLight")->second.lock()->GetDirectionalLightCache().m_wstrModelFilePath).c_str());

								writer.Key("Total Number of DirectionalLights ");
								writer.Int(iDirectionalLightCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"DirectionalLight")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("SolidAngle");
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_fSolidAngle);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("PerceivedRadius");
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_fPerceivedRadius);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("PerceivedDistance");
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_fPerceivedDistance);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Energy");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vEnergy.x);
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vEnergy.y);
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vEnergy.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Direction");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vDirection.x);
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vDirection.y);
													writer.Double(umapGameObject.second.lock()->GetDirectionalLightCache().m_vDirection.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetDirectionalLightCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetDirectionalLightCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetDirectionalLightCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

						if (0 != iCameraCount)
						{
							writer.Key("Cameras Information");
							writer.StartObject();
							{
								writer.Key("Model File Path");
								writer.String(CStringUtility::ToString(vecOctreeNodeDesc[x][y][z].find(L"Camera")->second.lock()->GetCameraCache().m_wstrModelFilePath).c_str());

								writer.Key("Total Number of Cameras ");
								writer.Int(iCameraCount);

								writer.Key("Individual Information");
								writer.StartObject();
								{
									int32 iIndex = -1;

									for (auto& umapGameObject : vecOctreeNodeDesc[x][y][z]) // map을 순회합니다
									{
										if (umapGameObject.first == L"Camera")
										{
											iIndex++;
											wstring wstrID = to_wstring(iIndex);
											wstring wstrName = umapGameObject.first + wstrID;

											writer.Key(CStringUtility::ToString(wstrName).c_str());
											writer.StartArray();
											{
												writer.StartObject();
												{
													writer.Key("RightSlope");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fRightSlope);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("LeftSlope");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fLeftSlope);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("TopSlope");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fTopSlope);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("BottomSlope");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fBottomSlope);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("NearPlane");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fNearPlane);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("FarPlane");
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_fFarPlane);
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Center");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vCenter.x);
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vCenter.y);
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vCenter.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Radian");
													writer.StartArray();
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vRadian.x);
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vRadian.y);
													writer.Double(umapGameObject.second.lock()->GetCameraCache().m_vRadian.z);
													writer.EndArray();
												}
												writer.EndObject();

												writer.StartObject();
												{
													writer.Key("Included Octree Node Index");
													writer.StartArray();
													writer.Int(get<0>(umapGameObject.second.lock()->GetCameraCache().m_tpOctreeNodeIndex));
													writer.Int(get<1>(umapGameObject.second.lock()->GetCameraCache().m_tpOctreeNodeIndex));
													writer.Int(get<2>(umapGameObject.second.lock()->GetCameraCache().m_tpOctreeNodeIndex));
													writer.EndArray();
												}
												writer.EndObject();
											}
											writer.EndArray();
										}
									}
								}
								writer.EndObject();
							}
							writer.EndObject();
						}

					}
				}
				writer.EndObject();

				ofstream ofs("../../Resource/Models/Dat/MapMesh/Prototype/Octree8/" + m_strSaveLoadFileName + "_" + to_string(x) + to_string(y) + to_string(z) + ".json");
				if (!ofs.is_open()) { return; }
				CHECKF(true, L"Save Completed");
				ofs << buffer.GetString();
				ofs.close();
			}
		}
	}
}

void ToolMap::CImGuiWindow::LoadDataFrom512Json()
{
	// 이번튼 누르면, MSGBox 뜨고, text창 안에 있는 글자 지우기
		// 1. JSON문서를 읽기 모드로 연다.
	ifstream isJsonpath(L"../../Resource/Models/Dat/MapMesh/Prototype/Octree8/" + m_wstrSaveLoadFileName + L".json");
	if (!isJsonpath.is_open())
	{
		throw runtime_error("failed to open json file");
	}

	// 2. 문서 내용을 문자열로 변환.
	string strJsonfile((istreambuf_iterator<char>(isJsonpath)), (istreambuf_iterator<char>()));

	// 3. JSON문서의 문자열을 파싱해온다.
	rapidjson::Document doc;
	doc.Parse(strJsonfile.c_str());

	// 4. JSON문서로부터 데이터를 읽는다.
	rapidjson::Value& bIsEmpty = doc["Is Empty?"];
	bool bIsFileEmpty = bIsEmpty.GetBool();

	if (!bIsFileEmpty)
	{
		rapidjson::Value& iTreeCount = doc["Tree Count"];
		rapidjson::Value& iRockCount = doc["Rock Count"];
		rapidjson::Value& iBranchCount = doc["Branch Count"];
		rapidjson::Value& iStoneCount = doc["Stone Count"];
		rapidjson::Value& iPalCount = doc["Pal Count"];
		rapidjson::Value& iArchitectureCount = doc["Architecture Count"];
		rapidjson::Value& iSpotLightCount = doc["SpotLight Count"];
		rapidjson::Value& iPointLightCount = doc["PointLight Count"];
		rapidjson::Value& iDirectionalLightCount = doc["DirectionalLight Count"];
		rapidjson::Value& iCameraCount = doc["Camera Count"];


		int32 iNumTreeCount = iTreeCount.GetInt();
		int32 iNumRockCount = iRockCount.GetInt();
		int32 iNumBranchCount = iBranchCount.GetInt();
		int32 iNumStoneCount = iStoneCount.GetInt();
		int32 iNumPalCount = iPalCount.GetInt();
		int32 iNumArchitectureCount = iArchitectureCount.GetInt();
		int32 iNumSpotLightCount = iSpotLightCount.GetInt();
		int32 iNumPointLightCount = iPointLightCount.GetInt();
		int32 iNumDirectionalLightCount = iDirectionalLightCount.GetInt();
		int32 iNumCameraCount = iCameraCount.GetInt();

		if (0 != iNumTreeCount)
		{
			rapidjson::Value& arrTree = doc["Trees Information"];

			FTreeCache tTreeCache{};

			tTreeCache.m_iHabitatType = arrTree["Habitat Type"].GetInt();
			tTreeCache.m_iNatureType = arrTree["Nature Type"].GetInt();
			tTreeCache.m_bTrigger = arrTree["Is Trigger"].GetBool();
			tTreeCache.m_iColliderType = arrTree["Collider Type"].GetInt();
			tTreeCache.m_fCurHp = arrTree["CurHp"].GetFloat();

			int32 iTotalTreeCount = arrTree["Total Number of Trees"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrTree["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalTreeCount; iIndex++)
			{
				wstring wstrTreeID = to_wstring(iIndex);
				wstring wstrTreeName = L"Tree" + wstrTreeID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrTreeName).c_str()];

				tTreeCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tTreeCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tTreeCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tTreeCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tTreeCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tTreeCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tTreeCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tTreeCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tTreeCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				tTreeCache.m_vCenter.x = arrChildDesc[3]["Center"][0].GetFloat();
				tTreeCache.m_vCenter.y = arrChildDesc[3]["Center"][1].GetFloat();
				tTreeCache.m_vCenter.z = arrChildDesc[3]["Center"][2].GetFloat();

				tTreeCache.m_vExtents.x = arrChildDesc[4]["Extents"][0].GetFloat();
				tTreeCache.m_vExtents.y = arrChildDesc[4]["Extents"][1].GetFloat();
				tTreeCache.m_vExtents.z = arrChildDesc[4]["Extents"][2].GetFloat();

				get<0>(tTreeCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tTreeCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tTreeCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				tTreeCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[6]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumRockCount)
		{
			rapidjson::Value& arrRock = doc["Rocks Information"];

			FRockCache tRockCache{};

			tRockCache.m_iHabitatType = arrRock["Habitat Type"].GetInt();
			tRockCache.m_iNatureType = arrRock["Nature Type"].GetInt();
			tRockCache.m_bTrigger = arrRock["Is Trigger"].GetBool();
			tRockCache.m_iColliderType = arrRock["Collider Type"].GetInt();
			tRockCache.m_fCurHp = arrRock["CurHp"].GetFloat();

			int32 iTotalRockCount = arrRock["Total Number of Rocks"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrRock["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalRockCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Rock" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tRockCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tRockCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tRockCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tRockCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tRockCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tRockCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tRockCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tRockCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tRockCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				tRockCache.m_vCenter.x = arrChildDesc[3]["Center"][0].GetFloat();
				tRockCache.m_vCenter.y = arrChildDesc[3]["Center"][1].GetFloat();
				tRockCache.m_vCenter.z = arrChildDesc[3]["Center"][2].GetFloat();

				tRockCache.m_fRadius = arrChildDesc[4]["Radius"].GetFloat();

				get<0>(tRockCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tRockCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tRockCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				tRockCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[6]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumBranchCount)
		{
			rapidjson::Value& arrBranch = doc["Branches Information"];

			FBranchCache tBranchCache{};

			tBranchCache.m_iHabitatType = arrBranch["Habitat Type"].GetInt();
			tBranchCache.m_iNatureType = arrBranch["Nature Type"].GetInt();
			tBranchCache.m_bTrigger = arrBranch["Is Trigger"].GetBool();
			tBranchCache.m_iColliderType = arrBranch["Collider Type"].GetInt();
			tBranchCache.m_vCenter.x = arrBranch["Center"][0].GetFloat();
			tBranchCache.m_vCenter.y = arrBranch["Center"][1].GetFloat();
			tBranchCache.m_vCenter.z = arrBranch["Center"][2].GetFloat();
			tBranchCache.m_fRadius = arrBranch["Radius"].GetFloat();
			tBranchCache.m_fHalfHeight = arrBranch["HalfHeight"].GetFloat();

			int32 iTotalBranchCount = arrBranch["Total Number of Branches"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrBranch["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalBranchCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Branch" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tBranchCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tBranchCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tBranchCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tBranchCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tBranchCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tBranchCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tBranchCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tBranchCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tBranchCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				get<0>(tBranchCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][0].GetInt();
				get<1>(tBranchCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][1].GetInt();
				get<2>(tBranchCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][2].GetInt();

				tBranchCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[4]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumStoneCount)
		{
			rapidjson::Value& arrStone = doc["Stones Information"];

			FStoneCache tStoneCache{};

			tStoneCache.m_iHabitatType = arrStone["Habitat Type"].GetInt();
			tStoneCache.m_iNatureType = arrStone["Nature Type"].GetInt();
			tStoneCache.m_bTrigger = arrStone["Is Trigger"].GetBool();
			tStoneCache.m_iColliderType = arrStone["Collider Type"].GetInt();
			tStoneCache.m_vCenter.x = arrStone["Center"][0].GetFloat();
			tStoneCache.m_vCenter.y = arrStone["Center"][1].GetFloat();
			tStoneCache.m_vCenter.z = arrStone["Center"][2].GetFloat();
			tStoneCache.m_fRadius = arrStone["Radius"].GetFloat();

			int32 iTotalStoneCount = arrStone["Total Number of Stones"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrStone["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalStoneCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Stone" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tStoneCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tStoneCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tStoneCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tStoneCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tStoneCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tStoneCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tStoneCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tStoneCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tStoneCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				get<0>(tStoneCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][0].GetInt();
				get<1>(tStoneCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][1].GetInt();
				get<2>(tStoneCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][2].GetInt();

				tStoneCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[4]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumPalCount)
		{
			rapidjson::Value& arrPal = doc["Pals Information"];

			FPalCache tPalCache{};

			tPalCache.m_iHabitatType = arrPal["Habitat Type"].GetInt();
			tPalCache.m_bTrigger = arrPal["Is Trigger"].GetBool();
			tPalCache.m_iColliderType = arrPal["Collider Type"].GetInt();

			int32 iTotalPalCount = arrPal["Total Number of Pals"].GetInt();
			rapidjson::Value& arrIndividualInfo = arrPal["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalPalCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Pal" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tPalCache.m_fRadius = arrChildDesc[0]["Radius"].GetFloat();

				tPalCache.m_vCenter.x = arrChildDesc[1]["Center"][0].GetFloat();
				tPalCache.m_vCenter.y = arrChildDesc[1]["Center"][1].GetFloat();
				tPalCache.m_vCenter.z = arrChildDesc[1]["Center"][2].GetFloat();

				tPalCache.m_vScale.x = arrChildDesc[2]["Scale"][0].GetFloat();
				tPalCache.m_vScale.y = arrChildDesc[2]["Scale"][1].GetFloat();
				tPalCache.m_vScale.z = arrChildDesc[2]["Scale"][2].GetFloat();

				tPalCache.m_vRotate.x = arrChildDesc[3]["Rotation"][0].GetFloat();
				tPalCache.m_vRotate.y = arrChildDesc[3]["Rotation"][1].GetFloat();
				tPalCache.m_vRotate.z = arrChildDesc[3]["Rotation"][2].GetFloat();

				tPalCache.m_vTranslate.x = arrChildDesc[4]["Translation"][0].GetFloat();
				tPalCache.m_vTranslate.y = arrChildDesc[4]["Translation"][1].GetFloat();
				tPalCache.m_vTranslate.z = arrChildDesc[4]["Translation"][2].GetFloat();

				get<0>(tPalCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tPalCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tPalCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				tPalCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[6]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumArchitectureCount)
		{
			rapidjson::Value& arrArchitecture = doc["Architectures Information"];

			FArchitectureCache tArchitectureCache{};

			tArchitectureCache.m_iHabitatType = arrArchitecture["Habitat Type"].GetInt();
			tArchitectureCache.m_bTrigger = arrArchitecture["Is Trigger"].GetBool();

			tArchitectureCache.m_fExtent = arrArchitecture["Extent"].GetFloat();

			tArchitectureCache.m_vCenter.x = arrArchitecture["Center"][0].GetFloat();
			tArchitectureCache.m_vCenter.y = arrArchitecture["Center"][1].GetFloat();
			tArchitectureCache.m_vCenter.z = arrArchitecture["Center"][2].GetFloat();

			tArchitectureCache.m_iColliderType = arrArchitecture["Collider Type"].GetInt();

			int32 iTotalArchitectureCount = arrArchitecture["Total Number of Architectures"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrArchitecture["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalArchitectureCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Architecture" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tArchitectureCache.m_vScale.x = arrChildDesc[0]["Scale"][0].GetFloat();
				tArchitectureCache.m_vScale.y = arrChildDesc[0]["Scale"][1].GetFloat();
				tArchitectureCache.m_vScale.z = arrChildDesc[0]["Scale"][2].GetFloat();

				tArchitectureCache.m_vRotate.x = arrChildDesc[1]["Rotation"][0].GetFloat();
				tArchitectureCache.m_vRotate.y = arrChildDesc[1]["Rotation"][1].GetFloat();
				tArchitectureCache.m_vRotate.z = arrChildDesc[1]["Rotation"][2].GetFloat();

				tArchitectureCache.m_vTranslate.x = arrChildDesc[2]["Translation"][0].GetFloat();
				tArchitectureCache.m_vTranslate.y = arrChildDesc[2]["Translation"][1].GetFloat();
				tArchitectureCache.m_vTranslate.z = arrChildDesc[2]["Translation"][2].GetFloat();

				get<0>(tArchitectureCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][0].GetInt();
				get<1>(tArchitectureCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][1].GetInt();
				get<2>(tArchitectureCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][2].GetInt();

				tArchitectureCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrChildDesc[4]["Model File Path"].GetString());

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumSpotLightCount)
		{
			rapidjson::Value& arrSpotLight = doc["SpotLights Information"];

			FSpotLightCache tSpotLightCache{};

			tSpotLightCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrSpotLight["Model File Path"].GetString());

			int32 iTotalSpotLightCount = arrSpotLight["Total Number of SpotLights"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrSpotLight["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalSpotLightCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"SpotLight" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tSpotLightCache.m_fRadius = arrChildDesc[0]["Radius"].GetFloat();

				tSpotLightCache.m_fCosAngle = arrChildDesc[1]["CosAngle"].GetFloat();

				tSpotLightCache.m_vEnergy.x = arrChildDesc[2]["Energy"][0].GetFloat();
				tSpotLightCache.m_vEnergy.y = arrChildDesc[2]["Energy"][1].GetFloat();
				tSpotLightCache.m_vEnergy.z = arrChildDesc[2]["Energy"][2].GetFloat();

				tSpotLightCache.m_vPosition.x = arrChildDesc[3]["Position"][0].GetFloat();
				tSpotLightCache.m_vPosition.y = arrChildDesc[3]["Position"][1].GetFloat();
				tSpotLightCache.m_vPosition.z = arrChildDesc[3]["Position"][2].GetFloat();

				tSpotLightCache.m_vDirection.x = arrChildDesc[4]["Direction"][0].GetFloat();
				tSpotLightCache.m_vDirection.y = arrChildDesc[4]["Direction"][1].GetFloat();
				tSpotLightCache.m_vDirection.z = arrChildDesc[4]["Direction"][2].GetFloat();

				get<0>(tSpotLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tSpotLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tSpotLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumPointLightCount)
		{
			rapidjson::Value& arrPointLight = doc["PointLights Information"];

			FPointLightCache tPointLightCache{};

			tPointLightCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrPointLight["Model File Path"].GetString());

			int32 iTotalPointLightCount = arrPointLight["Total Number of PointLights"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrPointLight["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalPointLightCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"PointLight" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tPointLightCache.m_fRadius = arrChildDesc[0]["Radius"].GetFloat();

				tPointLightCache.m_vEnergy.x = arrChildDesc[1]["Energy"][0].GetFloat();
				tPointLightCache.m_vEnergy.y = arrChildDesc[1]["Energy"][1].GetFloat();
				tPointLightCache.m_vEnergy.z = arrChildDesc[1]["Energy"][2].GetFloat();

				tPointLightCache.m_vPosition.x = arrChildDesc[2]["Position"][0].GetFloat();
				tPointLightCache.m_vPosition.y = arrChildDesc[2]["Position"][1].GetFloat();
				tPointLightCache.m_vPosition.z = arrChildDesc[2]["Position"][2].GetFloat();

				get<0>(tPointLightCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][0].GetInt();
				get<1>(tPointLightCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][1].GetInt();
				get<2>(tPointLightCache.m_tpOctreeNodeIndex) = arrChildDesc[3]["Included Octree Node Index"][2].GetInt();

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumDirectionalLightCount)
		{
			rapidjson::Value& arrDirectionalLight = doc["DirectionalLights Information"];

			FDirectionalLightCache tDirectionalLightCache{};

			tDirectionalLightCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrDirectionalLight["Model File Path"].GetString());

			int32 iTotalDirectionalLightCount = arrDirectionalLight["Total Number of DirectionalLights"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrDirectionalLight["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalDirectionalLightCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"DirectionalLight" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tDirectionalLightCache.m_fSolidAngle = arrChildDesc[0]["SolidAngle"].GetFloat();
				tDirectionalLightCache.m_fPerceivedRadius = arrChildDesc[1]["PerceivedRadius"].GetFloat();
				tDirectionalLightCache.m_fPerceivedDistance = arrChildDesc[2]["PerceivedDistance"].GetFloat();

				tDirectionalLightCache.m_vEnergy.x = arrChildDesc[3]["Energy"][0].GetFloat();
				tDirectionalLightCache.m_vEnergy.y = arrChildDesc[3]["Energy"][1].GetFloat();
				tDirectionalLightCache.m_vEnergy.z = arrChildDesc[3]["Energy"][2].GetFloat();

				tDirectionalLightCache.m_vDirection.x = arrChildDesc[4]["Direction"][0].GetFloat();
				tDirectionalLightCache.m_vDirection.y = arrChildDesc[4]["Direction"][1].GetFloat();
				tDirectionalLightCache.m_vDirection.z = arrChildDesc[4]["Direction"][2].GetFloat();

				get<0>(tDirectionalLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][0].GetInt();
				get<1>(tDirectionalLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][1].GetInt();
				get<2>(tDirectionalLightCache.m_tpOctreeNodeIndex) = arrChildDesc[5]["Included Octree Node Index"][2].GetInt();

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		if (0 != iNumCameraCount)
		{
			rapidjson::Value& arrCamera = doc["Cameras Information"];

			FCameraCache tCameraCache{};

			tCameraCache.m_wstrModelFilePath = CStringUtility::ToWideString(arrCamera["Model File Path"].GetString());

			int32 iTotalCameraCount = arrCamera["Total Number of Cameras"].GetInt();

			rapidjson::Value& arrIndividualInfo = arrCamera["Individual Information"];

			for (int32 iIndex = 0; iIndex < iTotalCameraCount; iIndex++)
			{
				wstring wstrID = to_wstring(iIndex);
				wstring wstrName = L"Camera" + wstrID;

				rapidjson::Value& arrChildDesc = arrIndividualInfo[CStringUtility::ToString(wstrName).c_str()];

				tCameraCache.m_fRightSlope = arrChildDesc[0]["RightSlope"].GetFloat();
				tCameraCache.m_fLeftSlope = arrChildDesc[1]["LeftSlope"].GetFloat();
				tCameraCache.m_fTopSlope = arrChildDesc[2]["TopSlope"].GetFloat();
				tCameraCache.m_fBottomSlope = arrChildDesc[3]["BottomSlope"].GetFloat();
				tCameraCache.m_fNearPlane = arrChildDesc[4]["NearPlane"].GetFloat();
				tCameraCache.m_fFarPlane = arrChildDesc[5]["FarPlane"].GetFloat();

				tCameraCache.m_vCenter.x = arrChildDesc[6]["Center"][0].GetFloat();
				tCameraCache.m_vCenter.y = arrChildDesc[6]["Center"][1].GetFloat();
				tCameraCache.m_vCenter.z = arrChildDesc[6]["Center"][2].GetFloat();

				tCameraCache.m_vRadian.x = arrChildDesc[7]["Radian"][0].GetFloat();
				tCameraCache.m_vRadian.y = arrChildDesc[7]["Radian"][1].GetFloat();
				tCameraCache.m_vRadian.z = arrChildDesc[7]["Radian"][2].GetFloat();

				get<0>(tCameraCache.m_tpOctreeNodeIndex) = arrChildDesc[8]["Included Octree Node Index"][0].GetInt();
				get<1>(tCameraCache.m_tpOctreeNodeIndex) = arrChildDesc[8]["Included Octree Node Index"][1].GetInt();
				get<2>(tCameraCache.m_tpOctreeNodeIndex) = arrChildDesc[8]["Included Octree Node Index"][2].GetInt();

				//m_vecHabitatTreeOrRockCaches.push_back(tHabitatTree);
			}
		}
		CHECKF(true, L"Save Completed");
	}
	else { CHECKF(true, L"Load Failed"); }
}

/* Load Resource */
void ToolMap::CImGuiWindow::LoadMapMeshResources()
{
	// Grid Landscape
	vector<wstring> vecGridLandscapeModelFilePaths = CModelHandler::GetInstance()->GetGridLandscapeMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecGridLandscapeModelFilePaths.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecGridLandscapeModelFilePaths.push_back(vecGridLandscapeModelFilePaths[iIndex]);
	}
	vecGridLandscapeModelFilePaths.clear();

	// Grid Env
	vector<wstring> vecGridEnvModelFilePaths = CModelHandler::GetInstance()->GetGridEnvMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecGridEnvModelFilePaths.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecGridEnvModelFilePaths.push_back(vecGridEnvModelFilePaths[iIndex]);
	}
	vecGridEnvModelFilePaths.clear();

	// Placeable Tree
	vector<wstring> vecPlaceableTreeModelFilePaths = CModelHandler::GetInstance()->GetPlaceableTreeMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecPlaceableTreeModelFilePaths.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecPlaceableTreeModelFilePaths.push_back(vecPlaceableTreeModelFilePaths[iIndex]);
	}
	vecPlaceableTreeModelFilePaths.clear();

	// Placeable Rock
	vector<wstring> vecPlaceableRockModelFilePath = CModelHandler::GetInstance()->GetPlaceableRockMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecPlaceableRockModelFilePath.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecPlaceableRockModelFilePaths.push_back(vecPlaceableRockModelFilePath[iIndex]);
	}
	vecPlaceableRockModelFilePath.clear();

	// Placeable Wood
	vector<wstring> vecPlaceableWoodModelFilePath = CModelHandler::GetInstance()->GetPlaceableWoodMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecPlaceableWoodModelFilePath.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecPlaceableWoodModelFilePaths.push_back(vecPlaceableWoodModelFilePath[iIndex]);
	}
	vecPlaceableWoodModelFilePath.clear();

	// Placeable Stone
	vector<wstring> vecPlaceableStoneModelFilePath = CModelHandler::GetInstance()->GetPlaceableStoneMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecPlaceableStoneModelFilePath.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecPlaceableStoneModelFilePaths.push_back(vecPlaceableStoneModelFilePath[iIndex]);
	}
	vecPlaceableStoneModelFilePath.clear();

	// Placeable Pal
	vector<wstring> vecPlaceablePalModelFilePath = CModelHandler::GetInstance()->GetPlaceablePalMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecPlaceablePalModelFilePath.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecPlaceablePalModelFilePaths.push_back(vecPlaceablePalModelFilePath[iIndex]);
	}
	vecPlaceablePalModelFilePath.clear();

	// Placeable Architecture
	vector<wstring> vecPlaceableArchitectureModelFilePath = CModelHandler::GetInstance()->GetPlaceableArchitectureMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecPlaceableArchitectureModelFilePath.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecPlaceableArchitectureModelFilePaths.push_back(vecPlaceableArchitectureModelFilePath[iIndex]);
	}
	vecPlaceableArchitectureModelFilePath.clear();

	// Placeable Light
	vector<wstring> vecPlaceableLightModelFilePath = CModelHandler::GetInstance()->GetPlaceableLightMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecPlaceableLightModelFilePath.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecPlaceableSpotLightModelFilePaths.push_back(vecPlaceableLightModelFilePath[iIndex]);
		m_vecPlaceablePointLightModelFilePaths.push_back(vecPlaceableLightModelFilePath[iIndex]);
		m_vecPlaceableDirectionalLightModelFilePaths.push_back(vecPlaceableLightModelFilePath[iIndex]);
	}
	vecPlaceableLightModelFilePath.clear();

	// Placeable Camera
	vector<wstring> vecPlaceableCameraModelFilePath = CModelHandler::GetInstance()->GetPlaceableCameraMeshFilePaths();
	for (int32 iIndex = 0; iIndex < vecPlaceableCameraModelFilePath.size(); iIndex++)
	{
		// 파일의 전체 경로를 저장합니다.
		m_vecPlaceableCameraModelFilePaths.push_back(vecPlaceableCameraModelFilePath[iIndex]);
	}
	vecPlaceableCameraModelFilePath.clear();
}

std::wstring ToolMap::CImGuiWindow::ExtractFileNameFromFilePath(uint32 _iIndex, std::vector<std::wstring> _vecModelFilePaths)
{
	// 경로를 짜르고 파일명만 저장합니다.
	size_t iLastSlashIndex = _vecModelFilePaths[_iIndex].find_last_of(L"/\\");
	wstring wstrFilename = _vecModelFilePaths[_iIndex].substr(iLastSlashIndex + 1);
	
	return wstrFilename;
}

std::wstring ToolMap::CImGuiWindow::ExtractWstringWithoutNumber(std::wstring _wstr)
{
	wstring wstrCharacter = _wstr;
	int32 iIndex = static_cast<int32>(wstrCharacter.size()) - 1;
	while (iIndex >= 0 && std::isdigit(wstrCharacter[iIndex])) { --iIndex; }
	if (iIndex >= 0) { wstrCharacter = wstrCharacter.substr(0, iIndex + 1); }
	else { wstrCharacter.clear(); }
	return wstrCharacter;
}

float ToolMap::CImGuiWindow::GetRandomRotationAngle()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(0.0f, DirectX::XM_2PI); 
	return dis(gen) * 60;
}

float ToolMap::CImGuiWindow::GetRandomScale()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(0.8f, 1.5f);
	return dis(gen);
}
