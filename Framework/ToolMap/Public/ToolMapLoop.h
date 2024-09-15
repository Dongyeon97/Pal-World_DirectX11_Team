#pragma once

namespace Engine
{
	class CEngineModule;
	class CWorld;
}

namespace ToolMap
{
	class CToolMapModule;

	class CToolMapLoop final
	{
		/********************
				Ctor/Dtor
			********************/
	public:
		explicit CToolMapLoop() = default;

		virtual ~CToolMapLoop() = default;

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

		std::shared_ptr<CToolMapModule> m_spToolMapModule = nullptr;

	public:
		std::shared_ptr<CWorld> m_spWorld = nullptr;
	};
}
