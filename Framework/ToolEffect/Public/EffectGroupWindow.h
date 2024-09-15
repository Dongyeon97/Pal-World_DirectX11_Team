#pragma once
#include "Window.h"

namespace ToolEffect
{
	class CToolEffectLevel;
	class CInspector;
	class CObjectList;

	class CEffectGroup;
	class CParticleObject;
	class CEffectMeshObject;

	class CEffectGroupWindow final : public CWindow
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CEffectGroupWindow() = default;
		virtual ~CEffectGroupWindow() = default;

	/********************
		Events
	********************/
	public:
		virtual HRESULT Initialize();
		virtual HRESULT BeginPlay();
		virtual int32 PreTick(float _fDeltaSeconds);
		virtual int32 Tick(float _fDeltaSeconds);
		virtual int32 PostTick(float _fDeltaSeconds);
		virtual int32 Render();
		virtual int32 EndPlay();
		virtual void Release();

	/********************
		Methods
	********************/
	private:
		HRESULT LoadEffectGroupNamesFromPath();
		HRESULT ParseEffectGroupToJSON(const wstring& _wstrJsonFileName);
		// void WriteParticle(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer, shared_ptr<CParticleObject> _spParticleObject);
		// void WriteEffectMesh(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer, shared_ptr<CEffectMeshObject> _spEffectMeshObject);

	private:
		/* 이펙트 그룹 폴더내의 파일이름 리스트를 보여줌 */
		void UpdateEffectGroupNames();
			void CreateEffectGroupFromFile();

		/* 현재 레벨에 생성되어 있는 이펙트 그룹들을 보여줌 */
		void UpdateLevelEffectGroups();
			void CreateEffectGroup();
			void RemoveEffectGroup();
			void PauseEffectGroup();
			void ResetEffectGroup();
			void SaveEffectGroup();
			void LoopEffectGroup();
			void PrintTrackPosition();

		/* 이펙트 그룹에 콜라이더 정보 붙이기 */
		void UpdateAttachingColliderTab();
			void UpdateColliderInfo();
			void UpdateAddButton();

	public:
		/* 현재 레벨에 이펙트 묶음 추가 */
		HRESULT AddEffectGroup(const std::wstring& _wstrEffectGroupName, std::shared_ptr<CEffectGroup> _spEffectGroup);

		/* 현재 레벨에서 이펙트 묶음 찾기 */
		std::shared_ptr<CEffectGroup> FindEffectGroup(const std::wstring& _wstrEffectGroupName);

		/* 현재 레벨에서 이펙트 묶음 삭제 */
		HRESULT RemoveEffectGroup(const std::wstring& _wstrEffectGroupName);

	/********************
		Getter/Setter
	********************/
	#pragma region 초기화
	public:
		void SetCurLevel(std::shared_ptr<CToolEffectLevel> _spLevel) { m_wpToolEffectLevel = _spLevel; }
		void SetInspector(std::shared_ptr<CInspector> _spInspector) { m_wpInspector = _spInspector; }
		void SetObjectList(std::shared_ptr<CObjectList> _spObjectList) { m_wpObjectList = _spObjectList; }
	#pragma endregion

	public:
		const std::map<std::wstring, std::shared_ptr<CEffectGroup>>& GetEffectGroups() { return m_mapEffectGroups; }

	public:
		/* 현재 선택된 이펙트 그룹 반환 */
		std::shared_ptr<CEffectGroup> GetCurEffectGroup();
	/********************
		Data Members
	********************/
	#pragma region 초기화
	private:
		bool m_bEffectGroupWindow = true;

		std::wstring m_wstrEffectGroupPath = L"../../Resource/Effects/EffectGroup/";
	#pragma endregion
	#pragma region 윈도우
	private:
		std::weak_ptr<CInspector> m_wpInspector;
		std::weak_ptr<CObjectList> m_wpObjectList;
	#pragma endregion
	#pragma region 파일
	private:
		std::vector<std::wstring> m_vecEffectGroupNames;
		uint16 m_iEffectGroupFileSelector = 0;
	#pragma endregion

	private:
		/* 레벨에 있는 이펙트 묶음 */
		std::map<std::wstring, std::shared_ptr<CEffectGroup>> m_mapEffectGroups;

		/* 생성할 이펙트 묶음 이름 */
		char m_szEffectGroupName[MAX_PATH];

		/* 생성할 이펙트 묶음의 듀레이션 */
		float m_fEffectGroupDuration = 0.f;

		/* 현재 선택된 이펙트 묶음 */
		std::shared_ptr<CEffectGroup> m_spCurEffectGroup = nullptr;
		std::wstring m_wstrCurEffectGroupName = L"";
		uint16 m_iCurEffectGroupSelector = 0;

	private:
		/* 콜라이더 정보 */
		bool m_bCollider = false;

		int32 m_iColliderTypeSelector = 0;
		int32 m_iCollisionFlagSelector = 0;
		int32 m_iCollisionLayerSelector = 0;

		EColliderType m_eColliderType	= EColliderType::SPHERE;
		ECollisionFlag m_eCollisionFlag	= ECollisionFlag::ENUM_END;
		ECollisionLayer m_eCollisionLayer = ECollisionLayer::ENUM_END;

		/* Sphere */
		float m_fRadius = 0.f;

		/* Box */
		Vector3 m_vExtents			= Vector3(0.f);
		Vector3 m_vLocalPosition	= Vector3(0.f);
		Vector3 m_vLocalRotate		= Vector3(0.f);
	};
}