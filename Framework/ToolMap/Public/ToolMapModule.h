#pragma once

namespace ToolMap 
{
	class CImGuiManager;

	class CToolMapModule final
	{
		/********************
			Ctor/Dtor
		********************/
	public:
		DECLARE_SINGLETON(CToolMapModule)

		/********************
	Events
********************/
	public:
		HRESULT Initialize();

		HRESULT BeginPlay();

		int32 PreTick(float _fDeltaSeconds);

		int32 Tick(float _fDeltaSeconds);

		int32 PostTick(float _fDeltaSeconds);

		void Render();

		void Release();

		/********************
			Getter/Setter
		********************/

		/********************
			Data Members
		********************/
	private:
		std::shared_ptr<CImGuiManager> m_spImGuiManager = nullptr;
	};
}

