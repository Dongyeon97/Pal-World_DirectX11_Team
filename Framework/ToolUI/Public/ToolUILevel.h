#pragma once
#include "Level.h"

namespace ToolUI
{
	class CToolUIPanel;

	class CToolUILevel : public CLevel
	{
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
		Data Members
	********************/
		std::weak_ptr<CToolUIPanel> m_wpToolUIPanel;
	};
}


