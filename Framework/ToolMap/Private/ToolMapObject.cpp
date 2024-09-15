#include "pch.h"
#include "ToolMapObject.h"

// Module
#include "EngineModule.h"

// Manager
#include "DeviceManager.h"
#include "InputManager.h"
#include "PickManager.h"

// Component
#include "Model.h"
#include "MeshComponent.h"
#include "DebugSphereComponent.h"
#include "DebugBoxComponent.h"
#include "DebugFrustumComponent.h"

std::shared_ptr<CToolMapObject> ToolMap::CToolMapObject::Create(const std::shared_ptr<FToolMapObjectDesc>& _spToolMapObjectDesc)
{
	shared_ptr<CToolMapObject> spInstance = make_shared<CToolMapObject>();
	CHECKF(spInstance, L"CToolMapObject : Create Failed");
	spInstance->Initialize(_spToolMapObjectDesc);

	return spInstance;
}

HRESULT ToolMap::CToolMapObject::Initialize(const std::shared_ptr<FToolMapObjectDesc>& _spToolMapObjectDesc)
{
	if (FAILED(CGameObject::Initialize(_spToolMapObjectDesc))) { return E_FAIL; }

	m_wstrModelFilePath = static_pointer_cast<FToolMapObjectDesc>(_spToolMapObjectDesc)->m_wstrModelFilePath;
	m_iIndex = static_pointer_cast<FToolMapObjectDesc>(_spToolMapObjectDesc)->m_iIndex;
	{
		FMeshComponentDesc tMeshComponentDesc;
		tMeshComponentDesc.m_wpOwnerGameObject = shared_from_this();
		tMeshComponentDesc.m_eComponentType = EComponentType::MESH;
		tMeshComponentDesc.m_wstrComponentName = L"MeshComponent";
		tMeshComponentDesc.m_eRenderType = ERenderType::RENDER_BLEND;
		tMeshComponentDesc.m_wstrModelFilePath = m_wstrModelFilePath;
		tMeshComponentDesc.m_matPivot = Matrix::CreateScale(0.01f);
		tMeshComponentDesc.m_spParentMeshComponent = nullptr; 
		tMeshComponentDesc.m_wstrShaderName = L"";
		m_spMeshComponent = CMeshComponent::Create(std::make_shared<FMeshComponentDesc>(tMeshComponentDesc));
		CHECKF(m_spMeshComponent, L"CMeshComponent : Failed Initialize");
		m_spMeshComponent->GetOwnerGameObject()->SetPosition(Vector3(0.f, 0.f, 0.f));
		AddComponent(L"MeshComponent", m_spMeshComponent);
	}
	{
		FDebugSphereComponentDesc tDebugSphereComponentDesc;
		tDebugSphereComponentDesc.m_fRadius				= 1.f;
		tDebugSphereComponentDesc.m_vCenter				= Vector3(0.0f, 0.0f, 0.0f);
		tDebugSphereComponentDesc.m_eColliderType		= EColliderType::SPHERE;
		tDebugSphereComponentDesc.m_eCollisionLayer		= ECollisionLayer::ENVIRONMENT;
		tDebugSphereComponentDesc.m_eCollisionFlag		= ECollisionFlag::ENVIRONMENT;
		tDebugSphereComponentDesc.m_eRenderType			= ERenderType::RENDER_DEBUG;
		tDebugSphereComponentDesc.m_eComponentType		= EComponentType::SPHERE;
		tDebugSphereComponentDesc.m_wpOwnerGameObject	= shared_from_this();
		tDebugSphereComponentDesc.m_wstrComponentName	= L"DebugSphereComponent";


		FDebugBoxComponentDesc tDebugBoxComponentDesc;
		tDebugBoxComponentDesc.m_eColliderType		= EColliderType::BOX;
		tDebugBoxComponentDesc.m_eCollisionLayer	= ECollisionLayer::ENVIRONMENT;
		tDebugBoxComponentDesc.m_eCollisionFlag		= ECollisionFlag::ENVIRONMENT;
		tDebugBoxComponentDesc.m_eRenderType		= ERenderType::RENDER_DEBUG;
		tDebugBoxComponentDesc.m_eComponentType		= EComponentType::BOX;
		tDebugBoxComponentDesc.m_wpOwnerGameObject	= shared_from_this();
		tDebugBoxComponentDesc.m_wstrComponentName	= L"DebugBoxComponent";

		/*
		shared_ptr<FDebugFrustumComponentDesc> spDebugFrustumComponentDesc = static_pointer_cast<FDebugFrustumComponentDesc>(_spToolMapObjectDesc);
		FDebugFrustumComponentDesc tDebugFrustumComponentDesc;
		tDebugFrustumComponentDesc.m_eColliderType		= EColliderType::BOX;
		tDebugFrustumComponentDesc.m_eCollisionLayer	= ECollisionLayer::ENVIRONMENT;
		tDebugFrustumComponentDesc.m_eCollisionFlag		= ECollisionFlag::ENVIRONMENT;
		tDebugFrustumComponentDesc.m_eRenderType		= ERenderType::RENDER_DEBUG;
		tDebugFrustumComponentDesc.m_eComponentType		= EComponentType::BOX;
		tDebugFrustumComponentDesc.m_wpOwnerGameObject	= shared_from_this();
		tDebugFrustumComponentDesc.m_wstrComponentName	= L"DebugFrustumComponent";
		tDebugFrustumComponentDesc.m_fRightSlope		= spDebugFrustumComponentDesc->m_fRightSlope;
		tDebugFrustumComponentDesc.m_fLeftSlope			= spDebugFrustumComponentDesc->m_fLeftSlope;
		tDebugFrustumComponentDesc.m_fTopSlope			= spDebugFrustumComponentDesc->m_fTopSlope;
		tDebugFrustumComponentDesc.m_fBottomSlope		= spDebugFrustumComponentDesc->m_fBottomSlope;
		tDebugFrustumComponentDesc.m_fNearPlane			= spDebugFrustumComponentDesc->m_fNearPlane;
		tDebugFrustumComponentDesc.m_fFarPlane			= spDebugFrustumComponentDesc->m_fFarPlane;
		tDebugFrustumComponentDesc.m_vCenter			= spDebugFrustumComponentDesc->m_vCenter;
		tDebugFrustumComponentDesc.m_vRadians			= spDebugFrustumComponentDesc->m_vRadians;
		*/

		switch (m_iIndex)
		{
		case 0:
			tDebugBoxComponentDesc.m_vExtents = Vector3(1.0f, 4.0f, 1.0f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 3.0f, 0.0f);
			break;

		case 1:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.4f, 4.0f, 0.4f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 3.0f, 0.0f);
			break;

		case 2:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.4f, 6.0f, 0.4f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 4.0f, 0.0f);
			break;

		case 3:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.4f, 4.0f, 0.4f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 4.0f, 0.0f);
			break;

		case 4:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.4f, 5.0f, 0.4f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 4.0f, 0.0f);
			break;

		case 5:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.4f, 5.0f, 0.4f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 4.0f, 0.0f);
			break;

		case 6:
			tDebugBoxComponentDesc.m_vExtents = Vector3(1.2f, 3.0f, 1.2f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 0.5f, 0.0f);
			break;

		case 7:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.4f, 3.0f, 0.4f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 1.5f, 0.0f);
			break;

		case 8:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.4f, 4.0f, 0.4f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 4.0f, 0.0f);
			break;

		case 9:
			tDebugBoxComponentDesc.m_vExtents = Vector3(1.2f, 3.0f, 1.2f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 1.0f, 0.0f);
			break;

		case 10:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.4f, 3.0f, 0.4f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 2.0f, 0.0f);
			break;

		case 11:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.35f, 3.0f, 0.35f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 2.0f, 0.0f);
			break;

		case 12:
			tDebugBoxComponentDesc.m_vExtents = Vector3(0.3f, 3.0f, 0.3f);
			tDebugBoxComponentDesc.m_vCenter = Vector3(0.0f, 2.0f, 0.0f);
			break;

			// Sphere
		case 13:
			tDebugSphereComponentDesc.m_fRadius = 9.f;
			tDebugSphereComponentDesc.m_vCenter = Vector3(0.0f, 2.0f, 0.0f);
			break;

		case 14:
			tDebugSphereComponentDesc.m_fRadius = 9.f;
			tDebugSphereComponentDesc.m_vCenter = Vector3(0.0f, 2.0f, 0.0f);
			break;

		case 15:
			tDebugSphereComponentDesc.m_fRadius = 9.f;
			tDebugSphereComponentDesc.m_vCenter = Vector3(0.0f, 2.0f, 0.0f);
			break;
		}

		if (m_iIndex < 13)
		{
			m_spDebugBoxComponent = CDebugBoxComponent::Create(make_shared<FDebugBoxComponentDesc>(tDebugBoxComponentDesc));
			AddComponent(L"DebugBoxComponent", m_spDebugBoxComponent);

			m_vBoxCenter = m_spDebugBoxComponent->GetCenter();
			m_vExtents = m_spDebugBoxComponent->GetExtents();
		}
		else
		{
			m_spDebugSphereComponent = CDebugSphereComponent::Create(make_shared<FDebugSphereComponentDesc>(tDebugSphereComponentDesc));
			AddComponent(L"DebugSphereComponent", m_spDebugSphereComponent);

			m_vSphereCenter = m_spDebugSphereComponent->GetCenter();
			m_fRadius = m_spDebugSphereComponent.get()->GetRadius();
		}

		// Camera Frustum
		//m_spDebugFrustumComponent = CDebugFrustumComponent::Create(make_shared<FDebugFrustumComponentDesc>(tDebugFrustumComponentDesc));
		//AddComponent(L"DebugFrustumComponent", m_spDebugFrustumComponent);
		//m_vFrustumCenter = m_spDebugFrustumComponent->GetCenter();
		//m_vRadians = m_spDebugFrustumComponent.get()->GetRadians();
	}

	return S_OK;
}

HRESULT ToolMap::CToolMapObject::BeginPlay()
{
	if (FAILED(CGameObject::BeginPlay())) { return E_FAIL; }

	return S_OK;
}

int32 ToolMap::CToolMapObject::PreTick(float _fDeltaSeconds)
{
	switch (CGameObject::PreTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}
	return S_OK;
}

int32 ToolMap::CToolMapObject::Tick(float _fDeltaSeconds)
{
	switch (CGameObject::Tick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return S_OK;
}

int32 ToolMap::CToolMapObject::PostTick(float _fDeltaSeconds)
{
	switch (CGameObject::PostTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return S_OK;
}

HRESULT ToolMap::CToolMapObject::EndPlay()
{
	if (FAILED(CGameObject::EndPlay())) { return E_FAIL; }

	return S_OK;
}

void ToolMap::CToolMapObject::Release()
{
	m_spMeshComponent = nullptr;
	m_spDebugSphereComponent = nullptr;
	m_spDebugBoxComponent = nullptr;
	m_spDebugFrustumComponent = nullptr;
	CGameObject::Release();	
}

std::tuple<int, int, int> ToolMap::CToolMapObject::CalculateOctreeNodeIndex(const Vector3& _vPosition)
{
	if (_vPosition.x < m_vOctreeRootCenter.x - m_vOctreeRootExtents.x || m_vOctreeRootCenter.x + m_vOctreeRootExtents.x < _vPosition.x) { return tuple(-1, -1, -1); }
	if (_vPosition.y < m_vOctreeRootCenter.y - m_vOctreeRootExtents.y || m_vOctreeRootCenter.y + m_vOctreeRootExtents.y < _vPosition.y) { return tuple(-1, -1, -1); }
	if (_vPosition.z < m_vOctreeRootCenter.z - m_vOctreeRootExtents.z || m_vOctreeRootCenter.z + m_vOctreeRootExtents.z < _vPosition.z) { return tuple(-1, -1, -1); }

	// 옥트리 왼쪽 아래 위치를 구합니다.
	float fX = m_vOctreeRootCenter.x - m_vOctreeRootExtents.x;
	float fY = m_vOctreeRootCenter.y - m_vOctreeRootExtents.y;
	float fZ = m_vOctreeRootCenter.z - m_vOctreeRootExtents.z;

	int iOctreeWidth = static_cast<int>(m_vOctreeRootExtents.x * 0.25f);
	int iIndexX = static_cast<int>(_vPosition.x - fX) / iOctreeWidth;
	int iIndexY = static_cast<int>(_vPosition.y - fY) / iOctreeWidth;
	int iIndexZ = static_cast<int>(_vPosition.z - fZ) / iOctreeWidth;

	m_spMeshComponent->GetOwnerGameObject()->SetOctreeNodeIndex(tuple(iIndexX, iIndexY, iIndexZ));

	return tuple(iIndexX, iIndexY, iIndexZ);
}

void ToolMap::CToolMapObject::SetHabitatBasicInfo(FHabitatCache _tHabitatCache)
{
	m_spMeshComponent->GetOwnerGameObject()->SetScale(_tHabitatCache.m_vScale);
	m_spMeshComponent->GetOwnerGameObject()->SetRotate(Vector3(0, 1, 0), _tHabitatCache.m_vRotate.y);
	m_spMeshComponent->GetOwnerGameObject()->SetPosition(_tHabitatCache.m_vTranslate);
	m_spMeshComponent->GetOwnerGameObject()->SetOctreeNodeIndex(_tHabitatCache.m_tpOctreeNodeIndex);
}


