#pragma once
#include "Manager.h"

namespace ToolMap
{
	class CImGuiWindow;

	class CImGuiManager final : public CManager
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		DECLARE_SINGLETON(CImGuiManager)

	/********************
		Events
	********************/
	public:
		HRESULT Initialize();

		HRESULT BeginPlay();

		void Tick(float _fDeltaSeconds);

		void Render();

		void Release();

	/********************
		Getter/Setter
	********************/
	public:
		std::weak_ptr<CImGuiWindow> GetImGuiWindow() const { return m_spImGuiWindow; }

	/********************
		Methods
	********************/
	public:
		HRESULT InitializeImGui();

		void TickImGui();

		void RenderImGui();

		void ReleaseImGui();




	/********************
		Data Members
	********************/
	private:
		std::shared_ptr<CImGuiWindow> m_spImGuiWindow = nullptr;
	};
}

