#pragma once

namespace Engine
{
	class CEngineModule;
	class CWorld;
}

namespace ToolUI
{
	class CToolUIModule;
	class CToolUILevel;

	class CToolUILoop final
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CToolUILoop() = default;

		virtual ~CToolUILoop() = default;

	/********************
		Events
	********************/
	public:
		HRESULT Initialize();

		HRESULT BeginPlay();

		int32 Tick();

		void Render();

		void Release();
		
	/********************
		Data Members
	********************/
	public:
		std::shared_ptr<CEngineModule> m_spEngineModule = nullptr;

		std::shared_ptr<CToolUIModule> m_spToolUIModule = nullptr;

	public:
		std::shared_ptr<CWorld> m_spWorld = nullptr;

		std::shared_ptr<CToolUILevel> m_spToolUILevel = nullptr;
	};
}

 