#pragma once

namespace Engine
{
	class CEngineModule;
}

namespace ToolEffect
{
	class CImguiManager;

	class CToolEffectModule
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		DECLARE_SINGLETON(CToolEffectModule)

	/********************
		Events
	********************/
	public:
		HRESULT Initialize();

		int32 PreTick(float _fDeltaSeconds);

		int32 Tick(float _fDeltaSeconds);

		int32 PostTick(float _fDeltaSeconds);

		void Render();

		void Release();
		
	/********************
		Getter/Setter
	********************/
	public:
		std::shared_ptr<CImguiManager> GetImguiManager() const { return m_spImguiManager; }
		
	/********************
		Data Members
	********************/
	private:
		std::shared_ptr<CImguiManager> m_spImguiManager = nullptr;
	};
}
