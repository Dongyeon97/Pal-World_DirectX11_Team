#include "pch.h"
#include "ToolMapLevel.h"

// Module
#include "EngineModule.h"

// Manager
#include "InputManager.h"
#include "PickManager.h"

// GameObject
#include "Camera.h"
#include "ToolMapObject.h"

// ImGui
#include "ImGuiManager.h"
#include "ImGuiWindow.h"

HRESULT ToolMap::CToolMapLevel::Initialize()
{
	m_wpInputManager = CEngineModule::GetInstance()->GetInputManager();
	m_wpPickManager = CEngineModule::GetInstance()->GetPickManager();
	m_wpIGuiManager = CImGuiManager::GetInstance();
	
	FCameraDesc tCameraObjectDesc;
	m_spCamera = CCamera::Create(make_shared<FCameraDesc>(tCameraObjectDesc));
	CHECKF(m_spCamera, L"CCamera : Failed");
	AddGameObject(m_spCamera);
	//m_spCamera->SetPosition(Vector3(-4080,0,-2040));
	m_spCamera->SetPosition(Vector3(0,0,0));
	return CLevel::Initialize();
}

HRESULT ToolMap::CToolMapLevel::BeginPlay()
{
	m_spCamera->SetOnAir();

	return CLevel::BeginPlay();
}

int32 ToolMap::CToolMapLevel::PreTick(float _fDeltaSeconds)
{
	return CLevel::PreTick(_fDeltaSeconds);
}

int32 ToolMap::CToolMapLevel::Tick(float _fDeltaSeconds)
{ 
	LockCameraViewDir();
	SelectPickMode();
	PickLevelGameObject();
	FastPickingMode();
	return CLevel::Tick(_fDeltaSeconds);
}

int32 ToolMap::CToolMapLevel::PostTick(float _fDeltaSeconds)
{
	return CLevel::PostTick(_fDeltaSeconds);
}

HRESULT ToolMap::CToolMapLevel::EndPlay()
{
	return CLevel::EndPlay();
}

void ToolMap::CToolMapLevel::Release()
{
	m_spCamera->Release();
	m_spCamera = nullptr;

	CLevel::Release();
}

std::weak_ptr<CToolMapObject> ToolMap::CToolMapLevel::CreateToolMapObject(const std::wstring& _wstrObjectName, const std::wstring& _wstrModelFilePath)
{
	CToolMapObject::FToolMapObjectDesc tToolMapObjectDesc = {};
	tToolMapObjectDesc.m_wstrGameObjectName = _wstrObjectName;
	tToolMapObjectDesc.m_wstrModelFilePath = _wstrModelFilePath;

	if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree0.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 0;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree1.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 1;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree2.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 2;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree3.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 3;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree4.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 4;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree5.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 5;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree6.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 6;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree7.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 7;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree8.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 8;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree9.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 9;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree10.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 10;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree11.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 11;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Tree/tree12.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 12;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Rock/pal_b00_rock_MineableRocks.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 13;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Rock/pal_b00_Rock_MineableRocks_GroundRevealRock_Cliff.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 14;
	}
	else if (L"../../Resource/Models/Dat/MapMesh/Placeable/Rock/pal_b00_Rock_MineableRocks_GroundRevealRock_var1.dat" == _wstrModelFilePath)
	{
		tToolMapObjectDesc.m_iIndex = 15;
	}

	
	m_spToolMapObject = CToolMapObject::Create(make_shared<CToolMapObject::FToolMapObjectDesc>(tToolMapObjectDesc));
	CHECKF(m_spToolMapObject, L"CToolMapObject : Failed");
	AddGameObject(m_spToolMapObject);
	
	Vector3 vPickingPos = m_wpPickManager.lock()->GetPickingPos();
	if (vPickingPos != Vector3(0.0f, 0.0f, 0.0f))
	{
		m_spToolMapObject->SetPosition(vPickingPos);
	}

	return m_spToolMapObject;
}

HRESULT ToolMap::CToolMapLevel::AddToolMapObject(EToolMapObjectType _eToolMapObjectType, const std::wstring& _wstrToolMapObjectName, const std::shared_ptr<CToolMapObject>& _spToolMapObject)
{
	switch (_eToolMapObjectType)
	{
	case ToolMap::EToolMapObjectType::WOOD:
		m_umapToolMapObjects.insert({ _wstrToolMapObjectName, _spToolMapObject });
		break;

	/*	case ToolMap::EToolMapObjectType::STONE:
			m_umapToolMapObjects.insert({ _wstrToolMapObjectName, dynamic_pointer_cast<CToolMapNature>(_spToolMapObject) });
			break;

		case ToolMap::EToolMapObjectType::TOWER:
			m_umapToolMapObjects.insert({ _wstrToolMapObjectName, dynamic_pointer_cast<CToolMapObject>(_spToolMapObject) });
			break;*/

	default:
		CHECKF(false, L"ToolMap ObjectType is not designated.");
		break;
	}
	return S_OK;
}

HRESULT ToolMap::CToolMapLevel::DeleteToolMapObject(EToolMapObjectType _eToolMapObjectType, const std::wstring& _wstrToolMapObjectName)
{
	switch (_eToolMapObjectType)
	{
	case ToolMap::EToolMapObjectType::WOOD:
	{
		if (m_umapToolMapObjects.find(_wstrToolMapObjectName) == m_umapToolMapObjects.end()) { return E_FAIL; }
		m_umapToolMapObjects.erase(_wstrToolMapObjectName);
	}
	break;
	}

	return S_OK;
}

std::weak_ptr<CToolMapObject> ToolMap::CToolMapLevel::FindToolMapObject(EToolMapObjectType _eToolMapObjectType, const std::wstring& _wstrToolMapObjectName)
{
	switch (_eToolMapObjectType)
	{
	case ToolMap::EToolMapObjectType::WOOD:
	{
		if (m_umapToolMapObjects.find(_wstrToolMapObjectName) == m_umapToolMapObjects.end())
		{
			return std::weak_ptr<CToolMapObject>();
		}

		auto iter = m_umapToolMapObjects.find(_wstrToolMapObjectName);
		return (iter->second);
	}
	break;
	}

	return std::weak_ptr<CToolMapObject>();
}

HRESULT ToolMap::CToolMapLevel::LockCameraViewDir()
{
	// Lock Camera
	if (!m_bLockCameraViewDir && m_wpInputManager.lock()->IsKeyDown(DIK_1))
	{
		m_bLockCameraViewDir = !m_bLockCameraViewDir;
		ENSURE(m_spCamera->LockCameraViewDirection(m_bLockCameraViewDir));
	}
	else if (m_bLockCameraViewDir && m_wpInputManager.lock()->IsKeyDown(DIK_1))
	{
		m_bLockCameraViewDir = !m_bLockCameraViewDir;
		ENSURE(m_spCamera->LockCameraViewDirection(m_bLockCameraViewDir));
	}
	
	return S_OK;
}

void ToolMap::CToolMapLevel::SelectPickMode()
{
	// Pick Mode
	if (!m_bPickMode && m_wpInputManager.lock()->IsKeyDown(DIK_P))
	{
		m_bPickMode = !m_bPickMode;
		m_wpPickManager.lock()->SetPickMode(m_bPickMode);
	}
	else if (m_bPickMode && m_wpInputManager.lock()->IsKeyDown(DIK_P))
	{
		m_bPickMode = !m_bPickMode;
		m_wpPickManager.lock()->SetPickingPos(Vector3(0.0f, 0.0f, 0.0f));
		m_wpPickManager.lock()->SetPickMode(m_bPickMode);
	}
}

void ToolMap::CToolMapLevel::PickLevelGameObject()
{
	if (m_wpInputManager.lock()->IsMouseDown(EMouseActionType::RIGHT_BUTTON))
	{
		m_wpPickManager.lock()->PickGameObject();
		uint32 iGameObjectID = m_wpPickManager.lock()->GetPickedGameObjectID();
		if (0 == iGameObjectID) { return; }
		weak_ptr<CImGuiWindow> wpImGuiWindow = m_wpIGuiManager.lock()->GetImGuiWindow();
		wpImGuiWindow.lock()->SetGameObjectID(iGameObjectID);
	}
}

void ToolMap::CToolMapLevel::FastPickingMode()
{
	// Lock Camera
	if (!m_bFastPickingMode && m_wpInputManager.lock()->IsKeyDown(DIK_C))
	{
		m_bFastPickingMode = !m_bFastPickingMode;
		weak_ptr<CImGuiWindow> wpImGuiWindow = m_wpIGuiManager.lock()->GetImGuiWindow();
		wpImGuiWindow.lock()->SetFastPickingMode(true);
	}
	else if (m_bFastPickingMode && m_wpInputManager.lock()->IsKeyDown(DIK_C))
	{
		m_bFastPickingMode = !m_bFastPickingMode;
		weak_ptr<CImGuiWindow> wpImGuiWindow = m_wpIGuiManager.lock()->GetImGuiWindow();
		wpImGuiWindow.lock()->SetFastPickingMode(false);
	}
}

