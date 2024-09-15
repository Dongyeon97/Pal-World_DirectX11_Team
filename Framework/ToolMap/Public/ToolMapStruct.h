#pragma once

#include "DirectXMath.h"

namespace ToolMap
{
	/********************
		Habitat Cache
	********************/
	struct FHabitatTreeOrRockCache final
	{
		bool m_bActive = true;											// Ȱ��ȭ ����
		bool m_bRespawn = false;											// ����� ����
		bool m_bMinable = true;											// ü�� ���� ����
		bool m_bIsTrigger = false;											// Trigger, Collider ����  
		float m_fRadius = 0.001f;
		float m_fHalfHeight = 0.001f;
		float m_fCurHp = 100.0f;							// ���� ü���Դϴ�. 
		float m_fMaxHp = 100.0f;							// �ִ� ü���Դϴ�.
		float m_fDropItemPercentage = 50.0f;							// ������ ��� �ۼ�Ʈ �Դϴ�. 
		float m_fDropItemEvent = 50.0f;							// ��� �������� Drop������ �̺�Ʈ�� �߻������� ���� �ð��Դϴ�.
		float m_fRespawnTime = 0.0f;								// �� �� ����� �ð� �Դϴ�. 
		unsigned int m_iTotalDropItemCount = 0;								// �� ����� ������ ���� �Դϴ�.
		unsigned int m_iMobilityType = 0;								// static, dynamic �Դϴ�.
		unsigned int m_iHabitatType = 0;								// Tree, Rock �Դϴ�.
		unsigned int m_iColliderType = 0;								// Sphere, AABB, OBB, Frustum, Capsule... �Դϴ�.
		DirectX::XMFLOAT3 m_vScale = { 1.0f, 1.0f, 1.0f };						// ������ �Դϴ�.
		DirectX::XMFLOAT3 m_vRotation = { 0.0f, 0.0f, 0.0f };						// ȸ���� �Դϴ�.
		DirectX::XMFLOAT3 m_vTranslation = { 0.0f, 0.0f, 0.0f };						// ��ġ�� �Դϴ�.
		std::wstring m_wstrDropItemAName = L"";							// A�������� ����մϴ�. 
		std::wstring m_wstrDropItemBName = L"";							// B�������� ����մϴ�.
		std::wstring m_wstrModelFilePath = L"";							// ���� ���ϰ�� �Դϴ�.
		std::wstring m_wstrEffectFilePath = L"";							// ����Ʈ(��ƼŬ)�� ���ϰ�� �Դϴ�.
		std::tuple<int, int, int> m_tpOctreeNodeIndex = { 0, 0, 0 };  // ���ԵǾ� �ִ� ��Ʈ�� �ε��� �Դϴ�.
	};

	struct FHabitatBranchOrStoneCache final
	{
		bool m_bActive = true;										// Ȱ��ȭ ����
		bool m_bRespawn = false;									// ����� ����
		bool m_bIsTrigger = true;										// Collider, Trigger ����
		float m_fRadius = 0.001f;
		float m_fHalfHeight = 0.001f;
		float m_fRespawnTime = 0.0f;										// �� �� ����� ���� �ð� �Դϴ�.
		unsigned int m_iHabitatType = 2;										// Wood, Stone �Դϴ�.
		unsigned int m_iMobilityType = 0;										// static, dynamic �Դϴ�.
		unsigned int m_iColliderType = 0;										// Sphere, AABB, OBB, Frustum, Capsule...�Դϴ�.
		DirectX::XMFLOAT3 m_vScale = { 1.0f, 1.0f, 1.0f };						// ������ �Դϴ�.
		DirectX::XMFLOAT3 m_vRotation = { 0.0f, 0.0f, 0.0f };						// ȸ���� �Դϴ�.
		DirectX::XMFLOAT3 m_vTranslation = { 0.0f, 0.0f, 0.0f };						// ��ġ�� �Դϴ�.
		std::wstring m_wstrModelFilePath = L"";								// ���� ���ϰ�� �Դϴ�.
		std::tuple<int, int, int> m_tpOctreeNodeIndex = { 0, 0, 0 };	// ���ԵǾ� �ִ� ��Ʈ�� �ε��� �Դϴ�.
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