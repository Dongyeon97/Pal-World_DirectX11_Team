#pragma once
#include "GameObject.h"

namespace Engine
{
	class CInputManager;
	class CPickManager;
	class CMeshComponent;
	class CDebugSphereComponent;
	class CDebugBoxComponent;
	class CDebugFrustumComponent;
}

namespace ToolMap
{
	class CToolMapObject : public CGameObject
	{
	public:
		struct FToolMapObjectDesc : public  FGameObjectDesc
		{
			int32 m_iIndex = 0;

			wstring m_wstrModelFilePath = L"";

			//// 초기 크기, 기본 크기는 (1, 1, 1)
			//Vector3 m_vInitScale = Vector3(1.f, 1.f, 1.f);

			//// 초기 회전, 기본 회전은 0도
			//Vector3 m_vInitRotate = Vector3(0.f, 0.f, 0.f);

			//// 초기 위치, 기본 위치는 (0, 0, 0)
			//Vector3 m_vInitPosition = Vector3(0.f, 0.f, 0.f);

			//// 게임 오브젝트 이름
			//std::wstring m_wstrGameObjectName = L"";

			//// 게임 오브젝트 활성화 여부
			//bool m_bActive = true;
		};
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CToolMapObject() = default;

		virtual ~CToolMapObject() = default;

	/********************
		Events
	********************/
	public:
		static std::shared_ptr<CToolMapObject> Create(const std::shared_ptr<FToolMapObjectDesc>& _spToolMapObjectDesc);

		virtual HRESULT Initialize(const std::shared_ptr<FToolMapObjectDesc>& _spToolMapObjectDesc);

		virtual HRESULT BeginPlay() override;

		virtual int32 PreTick(float _fDeltaSeconds) override;

		virtual int32 Tick(float _fDeltaSeconds) override;

		virtual int32 PostTick(float _fDeltaSeconds) override;

		virtual HRESULT EndPlay() override;

		virtual void Release() override;


	/********************
		Methods
	********************/
	public:
		std::tuple<int, int, int> CalculateOctreeNodeIndex(const Vector3& _vPosition);

		void SetHabitatBasicInfo(FHabitatCache _tHabitatCache);

		const Vector3 GetDebugBoxColliderCenter() const { return m_vBoxCenter; }

		const Vector3 GetDebugBoxColliderExtents() const { return m_vExtents; }

		const Vector3 GetDebugSphereColliderCenter() const { return m_vSphereCenter; }
		
		const float GetDebugSphereColliderRadius() const { return m_fRadius; }

	/********************
		Getter / Setter
	********************/
	public:
		const FHabitatCache& GetHabitatCache()						const { return m_tHabitatCache; }
		const FNatureCache& GetNatureCache()						const { return m_tNatureCache; }
		const FTreeCache& GetTreeCache()							const { return m_tTreeCache; }
		const FRockCache& GetRockCache()							const { return m_tRockCache; }
		const FBranchCache& GetBranchCache()						const { return m_tBranchCache; }
		const FStoneCache& GetStoneCache()							const { return m_tStoneCache; }
		const FPalCache& GetPalCache()								const { return m_tPalCache; }
		const FArchitectureCache& GetArchitectureCache()			const { return m_tArchitectureCache; }
		const FSpotLightCache& GetSpotLightCache()					const { return m_tSpotLightCache; }
		const FPointLightCache& GetPointLightCache()				const { return m_tPointLightCache; }
		const FDirectionalLightCache& GetDirectionalLightCache()	const { return m_tDirectionalLightCache; }
		const FCameraCache& GetCameraCache()						const { return m_tCameraCache; }

	public:
		void SetHabitatCache(FHabitatCache _tHabitatCache)								{ m_tHabitatCache = _tHabitatCache; }
		void SetNatureCache(FNatureCache _tNatureCache)									{ m_tNatureCache = _tNatureCache; }
		void SetTreeCache(FTreeCache _tTreeCache)										{ m_tTreeCache = _tTreeCache; }
		void SetRockCache(FRockCache _tRockCache)										{ m_tRockCache = _tRockCache; }
		void SetBranchCache(FBranchCache _tBranchCache)									{ m_tBranchCache = _tBranchCache; }
		void SetStoneCache(FStoneCache _tStoneCache)									{ m_tStoneCache = _tStoneCache; }
		void SetPalCache(FPalCache _tPalCache)											{ m_tPalCache = _tPalCache; }
		void SetArchitectureCache(FArchitectureCache _tArchitectureCache)				{ m_tArchitectureCache = _tArchitectureCache; }
		void SetSpotLightCache(FSpotLightCache _tSpotLightCache)						{ m_tSpotLightCache = _tSpotLightCache; }
		void SetPointLightCache(FPointLightCache _tPointLightCache)						{ m_tPointLightCache = _tPointLightCache; }
		void SetDirectionalLightCache(FDirectionalLightCache _tDiretionalLightCache)	{ m_tDirectionalLightCache = _tDiretionalLightCache; }
		void SetCameraCache(FCameraCache _tCameraCache)									{ m_tCameraCache = _tCameraCache; }



	/********************
		Data Members
	********************/
	private:
		Vector3 m_vOctreeRootExtents = Vector3(510.0f, 510.0f, 510.0f);
		Vector3 m_vOctreeRootCenter = Vector3(-3570.0f, 0.0f, -2550.0f);

		int32 m_iIndex = 0;
		std::wstring m_wstrModelFilePath = L"";
		
		// Box
		Vector3 m_vBoxCenter	= Vector3(0.0f, 0.0f, 0.0f);
		Vector3 m_vExtents		= Vector3(0.0f, 0.0f, 0.0f);

		// Sphere
		Vector3 m_vSphereCenter = Vector3(0.0f, 0.0f, 0.0f);
		float m_fRadius = 1.0f;

		// Frustum
		float	m_fRightSlope		= 1.0f;
		float	m_fLeftSlope		= -1.0f;
		float	m_fTopSlope			= 1.0f;
		float	m_fBottomSlope		= -1.0f;
		float	m_fNearPlane		= 0.1f;
		float	m_fFarPlane			= 1.0f;
		Vector3 m_vFrustumCenter	= Vector3(0.0f, 0.0f, 0.0f);
		Vector3 m_vRadians			= Vector3(0.0f, 0.0f, 0.0f);

		std::shared_ptr<CMeshComponent> m_spMeshComponent = nullptr;
		std::shared_ptr<CDebugSphereComponent> m_spDebugSphereComponent = nullptr;
		std::shared_ptr<CDebugBoxComponent> m_spDebugBoxComponent = nullptr;
		std::shared_ptr<CDebugFrustumComponent> m_spDebugFrustumComponent = nullptr;

	private:
		FHabitatCache			m_tHabitatCache;
		FNatureCache			m_tNatureCache;
		FTreeCache				m_tTreeCache;
		FRockCache				m_tRockCache;
		FBranchCache			m_tBranchCache;
		FStoneCache				m_tStoneCache;
		FPalCache				m_tPalCache;
		FArchitectureCache		m_tArchitectureCache;
		FSpotLightCache			m_tSpotLightCache;
		FPointLightCache		m_tPointLightCache;
		FDirectionalLightCache	m_tDirectionalLightCache;
		FCameraCache			m_tCameraCache;
	};
}
