#pragma once

namespace Engine
{
	class CEngineModule;
	class CWorld;
}

namespace ToolEffect
{
	class CToolEffectModule;
	class CInspector;

	class CToolEffectLoop final
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CToolEffectLoop() = default;

		virtual ~CToolEffectLoop() = default;

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

		std::shared_ptr<CToolEffectModule> m_spToolEffectModule = nullptr;

	public:
		std::shared_ptr<CWorld> m_spWorld = nullptr;

		std::shared_ptr<CInspector> m_spInspector = nullptr;
	};
}
