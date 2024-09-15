#pragma once

namespace Engine
{
	class CLevel;
}

namespace ToolEffect
{
	class CToolEffectLevel;

	class CWindow
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CWindow() = default;

		virtual ~CWindow() = default;

	/********************
		Events
	********************/
	public:
		virtual HRESULT Initialize();

		virtual HRESULT BeginPlay();
		
		virtual int32 PreTick(float _fDeltaSeconds);

		virtual int32 Tick(float _fDeltaSeconds);

		virtual int32 PostTick(float _fDeltaSeconds);

		virtual int32 Render();

		virtual int32 EndPlay();

		virtual void Release();

	/********************
		Methods
	********************/
	public:


	/********************
		Getter/Setter
	********************/
	public:

		
	/********************
		Data Members
	********************/
	protected:
		std::weak_ptr<CToolEffectLevel> m_wpToolEffectLevel;
	};

}