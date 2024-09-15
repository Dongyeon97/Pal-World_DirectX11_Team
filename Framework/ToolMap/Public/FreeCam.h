#pragma once
#include "Camera.h"

namespace Engine
{
	class CCamera;
}

namespace ToolMap
{
	class CFreeCam final : public CCamera
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
		virtual HRESULT Initialize() override;

		virtual HRESULT BeginPlay() override;

		virtual int32 PreTick(float _fDeltaSeconds) override;

		virtual int32 Tick(float _fDeltaSeconds) override;

		virtual int32 PostTick(float _fDeltaSeconds) override;

		virtual HRESULT EndPlay() override;

		virtual void Render() override;

		virtual void Release() override;
		
	/********************
		Data Members
	********************/
	public:
	};
}
