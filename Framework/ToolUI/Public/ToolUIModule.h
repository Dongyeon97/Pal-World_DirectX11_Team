#pragma once

namespace Engine
{
	class CEngineModule;
}

namespace ToolUI
{
	class CIMGUIManager;
	class CToolUIManager;

	class CToolUIModule final
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		DECLARE_SINGLETON(CToolUIModule)

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
		std::shared_ptr<CIMGUIManager> GetIMGUIManager() const { return m_spIMGUIManager; }

		std::shared_ptr<CToolUIManager> GetToolUIManager() const { return m_spToolUIManager; }
		
	/********************
		Data Members
	********************/
	private:
		std::shared_ptr<CIMGUIManager> m_spIMGUIManager = nullptr;

		std::shared_ptr<CToolUIManager> m_spToolUIManager = nullptr;
	};

}
