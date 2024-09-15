#pragma once
#include "Panel.h"

namespace Engine
{
	class CRectComponent;
}

namespace ToolUI
{
	class CToolUIPanel : public CPanel
	{
	public:
		struct FUIPanelDesc : public FGameObjectDesc
		{

		};
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CToolUIPanel() = default;

		virtual ~CToolUIPanel() = default;

	/********************
		Events
	********************/
	public:
		static std::shared_ptr<CToolUIPanel> Create(const std::shared_ptr<FUIPanelDesc>& _spUIPanelDesc);

		virtual HRESULT Initialize(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc) override;

		virtual HRESULT BeginPlay() override;

		virtual int32 PreTick(float _fDeltaSeconds) override;

		virtual int32 Tick(float _fDeltaSeconds) override;

		virtual int32 PostTick(float _fDeltaSeconds) override;

		virtual HRESULT EndPlay() override;

		virtual void Release() override;


	/********************
		Data Members
	********************/
	private:
	};
}

