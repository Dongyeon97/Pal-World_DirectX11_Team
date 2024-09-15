#pragma once
#include "Level.h"

namespace Engine
{
	class CInputManager;
	class CPickManager;
	class CCamera;
}

namespace ToolMap
{
	class CToolMapObject;
	class CImGuiManager;

	class CToolMapLevel final : public CLevel
	{
	/******************
		Ctor/Dtor
	******************/
	public:
		explicit CToolMapLevel() = default;

		virtual ~CToolMapLevel() = default;

	/********************
		Events
	********************/
	public:
		virtual HRESULT Initialize() override;

		virtual HRESULT BeginPlay() override;

		virtual int32 PreTick(float _fDeltaSeconds) override;

		virtual int32 Tick(float _fDeltaSeconds) override;

		virtual int32 PostTick(float _fDeltaSeconds) override;

		virtual HRESULT EndPlay() override;

		virtual void Release() override;

	/********************
		Getter/Setter
	********************/
	public:
		bool IsCameraLocked() const { return m_bLockCameraViewDir; }

		bool GetPickMode() const { return m_bPickMode; }

		uint32 GetGridMapID() const { return m_iGridMapID; }

	public:
		void SetCameraLock(const bool _bLockCameraViewDir) { m_bLockCameraViewDir = _bLockCameraViewDir; }

		void SetPickMode(const bool _bPickMode) { m_bPickMode = _bPickMode; }

	public:
		const std::unordered_map<std::wstring, std::weak_ptr<CToolMapObject>>& GetToolMapObjects() { return m_umapToolMapObjects; }

	/********************
		Methods
	********************/
	public:
		std::weak_ptr<CToolMapObject> CreateToolMapObject(const std::wstring& _wstrObjectName, const std::wstring& _wstrModelFilePath);

		HRESULT AddToolMapObject(EToolMapObjectType _eToolMapObjectType, const std::wstring& _wstrToolMapObjectName, const std::shared_ptr<CToolMapObject>& _spToolMapObject);

		HRESULT DeleteToolMapObject(EToolMapObjectType _eToolMapObjectType, const std::wstring& _wstrToolMapObjectName);
		
		std::weak_ptr<CToolMapObject> FindToolMapObject(EToolMapObjectType _eToolMapObjectType, const std::wstring& _wstrToolMapObjectName);

	public:
		HRESULT LockCameraViewDir();

		void SelectPickMode();

		void PickLevelGameObject();
		
		void FastPickingMode();

	/********************
		Data Members
	********************/
	private:
		uint32 m_iGridMapID = 0;

		bool m_bPickMode = false;

		bool m_bLockCameraViewDir = false;

		bool m_bFastPickingMode = false;

	private:
		std::shared_ptr<CCamera> m_spCamera = nullptr;

		std::shared_ptr<CToolMapObject> m_spToolMapObject;

		std::weak_ptr<CInputManager> m_wpInputManager;

		std::weak_ptr<CPickManager> m_wpPickManager;

		std::weak_ptr<CImGuiManager> m_wpIGuiManager;

		std::unordered_map<std::wstring, std::weak_ptr<CToolMapObject>> m_umapToolMapObjects;
	};

}

