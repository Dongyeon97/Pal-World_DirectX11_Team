#pragma once
#include "Level.h"

namespace Engine
{
	class CInputManager;
	class CLevel;
	class CCamera;
}

namespace ToolEffect
{
	class CFreeCam;

	class CToolEffectLevel final : public CLevel
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CToolEffectLevel() = default;

		virtual ~CToolEffectLevel() = default;

	/********************
		Events
	********************/
	public:
		virtual HRESULT Initialize() override;

		virtual HRESULT BeginPlay() override;
		
		virtual int32 PreTick(float _fDeltaSeconds) override;

		virtual int32 Tick(float _fDeltaSeconds) override;

		virtual int32 PostTick(float _fDeltaSeconds) override;

		virtual void Release() override;

	/********************
		Methods
	********************/
	public:
		HRESULT LockCameraViewDir();

	/********************
		Getter/Setter
	********************/
	public:

		
	/********************
		Data Members
	********************/
	private:
		// std::weak_ptr<CFreeCam> m_wpFreeCam;

		std::shared_ptr<CCamera> m_spCamera;

	private:
		std::weak_ptr<CInputManager> m_wpInputManager;

		bool m_bLockCameraViewDir = false;
	};
}
