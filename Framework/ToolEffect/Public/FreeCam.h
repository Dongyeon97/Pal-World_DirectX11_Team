#pragma once
#include "GameObject.h"

namespace Engine
{
	class CCameraComponent;
}

namespace ToolEffect
{
	class CFreeCam final : public CGameObject
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CFreeCam() = default;

		virtual ~CFreeCam() = default;
		
	/********************
		Events
	********************/
	public:
		// virtual HRESULT Initialize() override;

		virtual HRESULT BeginPlay() override;

		virtual int32 PreTick(float _fDeltaSeconds) override;

		virtual int32 Tick(float _fDeltaSeconds) override;

		virtual int32 PostTick(float _fDeltaSeconds) override;

		virtual HRESULT EndPlay() override;

		// virtual void Render() override;

		virtual void Release() override;
		
	/********************
		Data Members
	********************/
	public:
		std::weak_ptr<CCameraComponent> m_wpCameraComponent;
	};
}
