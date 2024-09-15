#pragma once

#include "DirectXMath.h"

namespace ToolMap
{
	/********************
		Habitat Cache
	********************/
	struct FHabitatTreeOrRockCache final
	{
		bool m_bActive = true;											// 활성화 여부
		bool m_bRespawn = false;											// 재생성 여부
		bool m_bMinable = true;											// 체굴 가능 여부
		bool m_bIsTrigger = false;											// Trigger, Collider 여부  
		float m_fRadius = 0.001f;
		float m_fHalfHeight = 0.001f;
		float m_fCurHp = 100.0f;							// 현재 체력입니다. 
		float m_fMaxHp = 100.0f;							// 최대 체력입니다.
		float m_fDropItemPercentage = 50.0f;							// 아이템 드롭 퍼센트 입니다. 
		float m_fDropItemEvent = 50.0f;							// 어느 시점에서 Drop아이템 이벤트가 발생할지에 대한 시간입니다.
		float m_fRespawnTime = 0.0f;								// 얼마 후 재생성 시간 입니다. 
		unsigned int m_iTotalDropItemCount = 0;								// 총 드롭할 아이템 개수 입니다.
		unsigned int m_iMobilityType = 0;								// static, dynamic 입니다.
		unsigned int m_iHabitatType = 0;								// Tree, Rock 입니다.
		unsigned int m_iColliderType = 0;								// Sphere, AABB, OBB, Frustum, Capsule... 입니다.
		DirectX::XMFLOAT3 m_vScale = { 1.0f, 1.0f, 1.0f };						// 사이즈 입니다.
		DirectX::XMFLOAT3 m_vRotation = { 0.0f, 0.0f, 0.0f };						// 회전값 입니다.
		DirectX::XMFLOAT3 m_vTranslation = { 0.0f, 0.0f, 0.0f };						// 위치값 입니다.
		std::wstring m_wstrDropItemAName = L"";							// A아이템을 드롭합니다. 
		std::wstring m_wstrDropItemBName = L"";							// B아이템을 드롭합니다.
		std::wstring m_wstrModelFilePath = L"";							// 모델의 파일경로 입니다.
		std::wstring m_wstrEffectFilePath = L"";							// 이펙트(파티클)의 파일경로 입니다.
		std::tuple<int, int, int> m_tpOctreeNodeIndex = { 0, 0, 0 };  // 포함되어 있는 옥트리 인덱스 입니다.
	};

	struct FHabitatBranchOrStoneCache final
	{
		bool m_bActive = true;										// 활성화 여부
		bool m_bRespawn = false;									// 재생성 여부
		bool m_bIsTrigger = true;										// Collider, Trigger 여부
		float m_fRadius = 0.001f;
		float m_fHalfHeight = 0.001f;
		float m_fRespawnTime = 0.0f;										// 얼마 후 재생성 될지 시간 입니다.
		unsigned int m_iHabitatType = 2;										// Wood, Stone 입니다.
		unsigned int m_iMobilityType = 0;										// static, dynamic 입니다.
		unsigned int m_iColliderType = 0;										// Sphere, AABB, OBB, Frustum, Capsule...입니다.
		DirectX::XMFLOAT3 m_vScale = { 1.0f, 1.0f, 1.0f };						// 사이즈 입니다.
		DirectX::XMFLOAT3 m_vRotation = { 0.0f, 0.0f, 0.0f };						// 회전값 입니다.
		DirectX::XMFLOAT3 m_vTranslation = { 0.0f, 0.0f, 0.0f };						// 위치값 입니다.
		std::wstring m_wstrModelFilePath = L"";								// 모델의 파일경로 입니다.
		std::tuple<int, int, int> m_tpOctreeNodeIndex = { 0, 0, 0 };	// 포함되어 있는 옥트리 인덱스 입니다.
	};

	struct FHabitatPalCache final
	{
		bool m_bIsTrigger = false;
		DirectX::XMFLOAT3 m_vPosition = { 0.0f, 0.0f, 0.0f };
		std::wstring m_wstrModelFilePath = L"";
		std::tuple<int, int, int> m_tpOctreeNodeIndex = { 0, 0, 0 };
	};

	struct FHabitatArchitectureCache final
	{
	};

	struct FHabitatSpotLightCache final
	{
	};

	struct FHabitatPointLightCache final
	{
		
	};

	struct FHabitatDirectionalLightCache final
	{
		
	};

	struct FHabitatCameraFrustumCache final
	{
		
	};


	/********************
	   Habitat Cache
	********************/
	struct FHabitatCache
	{
		int32 m_iHabitatType = 0;
		Vector3 m_vScale = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 m_vRotate = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 m_vTranslate = Vector3(0.0f, 0.0f, 0.0f);
		std::tuple<int32, int32, int32> m_tpOctreeNodeIndex = { 0, 0, 0 };
		std::wstring m_wstrModelFilePath = L"";
		bool m_bTrigger = false;
		int32 m_iColliderType = 0;
	};

	struct FNatureCache : public FHabitatCache
	{
		int32 m_iNatureType = 0;
	};

	struct FTreeCache : public FNatureCache
	{
		float m_fCurHp = 100.0f;
		Vector3 m_vExtents = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
	};

	struct FRockCache : public FNatureCache
	{
		float m_fCurHp = 100.0f;
		float m_fRadius = 0.001f;
		Vector3 m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
	};

	struct FBranchCache : public FNatureCache
	{
		float m_fRadius = 0.001f;
		float m_fHalfHeight = 0.001f;
		Vector3 m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
	};

	struct FStoneCache : public FNatureCache
	{
		float m_fRadius = 0.001f;
		Vector3 m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
	};

	struct FPalCache : public FHabitatCache
	{
		float m_fRadius = 0.001f;
		Vector3 m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
	};

	struct FArchitectureCache : public FHabitatCache
	{
		float m_fExtent = 0.001f;
		Vector3 m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
	};

	struct FSpotLightCache : public FHabitatCache
	{
		float m_fRadius = 1.0f;						
		float m_fCosAngle = 0.0f;;							
		Vector3 m_vEnergy = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 m_vPosition = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 m_vDirection = Vector3(0.0f, 0.0f, 0.0f);
	};

	struct FPointLightCache : public FHabitatCache
	{
		float m_fRadius = 1.0f;					
		Vector3  m_vEnergy = Vector3(0.0f, 0.0f, 0.0f);
		Vector3  m_vPosition = Vector3(0.0f, 0.0f, 0.0f);
	};

	struct FDirectionalLightCache : public FHabitatCache
	{
		float m_fSolidAngle = 0.0f;					
		float m_fPerceivedRadius = 1.0f;					
		float m_fPerceivedDistance = 1.0f;					
		Vector3 m_vEnergy = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 m_vDirection = Vector3(0.0f, 0.0f, 0.0f);
	};
	struct FCameraCache : public FHabitatCache
	{
		float m_fRightSlope = 1.0f;
		float m_fLeftSlope = 1.0f;
		float m_fTopSlope = 1.0f;
		float m_fBottomSlope = 1.0f;
		float m_fNearPlane = 0.0f;
		float m_fFarPlane = 1.0f;
		Vector3 m_vCenter = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 m_vRadian = Vector3(0.0f, 0.0f, 0.0f); 
	};
}