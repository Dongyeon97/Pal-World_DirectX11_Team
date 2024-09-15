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
		/* ����Ʈ �׷� �������� �����̸� ����Ʈ�� ������ */
		void UpdateEffectGroupNames();
			void CreateEffectGroupFromFile();

		/* ���� ������ �����Ǿ� �ִ� ����Ʈ �׷���� ������ */
		void UpdateLevelEffectGroups();
			void CreateEffectGroup();
			void RemoveEffectGroup();
			void PauseEffectGroup();
			void ResetEffectGroup();
			void SaveEffectGroup();
			void LoopEffectGroup();
			void PrintTrackPosition();

		/* ����Ʈ �׷쿡 �ݶ��̴� ���� ���̱� */
		void UpdateAttachingColliderTab();
			void UpdateColliderInfo();
			void UpdateAddButton();

	public:
		/* ���� ������ ����Ʈ ���� �߰� */
		HRESULT AddEffectGroup(const std::wstring& _wstrEffectGroupName, std::shared_ptr<CEffectGroup> _spEffectGroup);

		/* ���� �������� ����Ʈ ���� ã�� */
		std::shared_ptr<CEffectGroup> FindEffectGroup(const std::wstring& _wstrEffectGroupName);

		/* ���� �������� ����Ʈ ���� ���� */
		HRESULT RemoveEffectGroup(const std::wstring& _wstrEffectGroupName);

	/********************
		Getter/Setter
	********************/
	#pragma region �ʱ�ȭ
	public:
		void SetCurLevel(std::shared_ptr<CToolEffectLevel> _spLevel) { m_wpToolEffectLevel = _spLevel; }
		void SetInspector(std::shared_ptr<CInspector> _spInspector) { m_wpInspector = _spInspector; }
		void SetObjectList(std::shared_ptr<CObjectList> _spObjectList) { m_wpObjectList = _spObjectList; }
	#pragma endregion

	public:
		const std::map<std::wstring, std::shared_ptr<CEffectGroup>>& GetEffectGroups() { return m_mapEffectGroups; }

	public:
		/* ���� ���õ� ����Ʈ �׷� ��ȯ */
		std::shared_ptr<CEffectGroup> GetCurEffectGroup();
	/********************
		Data Members
	********************/
	#pragma region �ʱ�ȭ
	private:
		bool m_bEffectGroupWindow = true;

		std::wstring m_wstrEffectGroupPath = L"../../Resource/Effects/EffectGroup/";
	#pragma endregion
	#pragma region ������
	private:
		std::weak_ptr<CInspector> m_wpInspector;
		std::weak_ptr<CObjectList> m_wpObjectList;
	#pragma endregion
	#pragma region ����
	private:
		std::vector<std::wstring> m_vecEffectGroupNames;
		uint16 m_iEffectGroupFileSelector = 0;
	#pragma endregion

	private:
		/* ������ �ִ� ����Ʈ ���� */
		std::map<std::wstring, std::shared_ptr<CEffectGroup>> m_mapEffectGroups;

		/* ������ ����Ʈ ���� �̸� */
		char m_szEffectGroupName[MAX_PATH];

		/* ������ ����Ʈ ������ �෹�̼� */
		float m_fEffectGroupDuration = 0.f;

		/* ���� ���õ� ����Ʈ ���� */
		std::shared_ptr<CEffectGroup> m_spCurEffectGroup = nullptr;
		std::wstring m_wstrCurEffectGroupName = L"";
		uint16 m_iCurEffectGroupSelector = 0;

	private:
		/* �ݶ��̴� ���� */
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