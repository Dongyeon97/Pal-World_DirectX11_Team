#pragma once
#include "Manager.h"

namespace Engine
{
	class CRectComponent;
}

namespace ToolUI
{
	class CIMGUIManager final : public CManager
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		DECLARE_SINGLETON(CIMGUIManager)

	/********************
		Events
	********************/
	public:
		HRESULT Initialize();

		int32 Tick(float _fDeltaSeconds);

		void Render();

		void Release();
		
	/********************
		Events
	********************/
	public:
		HRESULT InitializeIMGUI();

		void TickIMGUI();

		void RenderIMGUI();

		void ReleaseIMGUI();

	/******************
		Methods
	******************/
	private:
		void CreateBaseIMGUI();

		void ChangeTransformIMGUI();

	private:
		std::shared_ptr<CRectComponent> GetFindSelectComponent();


	/********************
		Getter/Setter
	********************/
	public:

	/******************
		Data Members
	******************/
		// 사용할 셰이더 파일의 이름을 저장합니다.
		std::wstring m_wstrSaveShaderName = L"";
		// 기본 텍스쳐의 이름을 저장합니다.
		std::wstring m_wstrSaveTextureName = L"";
		// 마스크 텍스쳐의 이름을 저장합니다.
		std::wstring m_wstrSaveMaskTextureName = L"";
		// 버튼 닿았을 때 텍스쳐 이름을 저장합니다.
		std::wstring m_wstrSaveTextureHoverName = L"";
		// 버튼 클릭했을때/ 슬라이더바 버튼의 텍스쳐 이름을 저장합니다. 
		std::wstring m_wstrSaveTextureClickName = L"";
		// 폰트 파일의 이름을 저장합니다.
		std::wstring m_wstrSaveFontFileName = L"";
		// Owner Object의 이름을 저장합니다.
		std::wstring m_wstrSaveOwnerPanelName = L"";
		// 선택한 부모 패널의 이름을 저장합니다.
		std::wstring m_wstrSaveParentPanelName = L"";
		// 선택한 자식 패널의 이름을 저장합니다.
		std::wstring m_wstrSaveChildPanelName = L"";
		// 지정한 색깔을 저장합니다.
		Vector4 m_vSaveColor = {1.0f, 1.0f, 1.0f, 1.0f};
		// 월드 UI일시 현재 컴포넌트를 저장합니다.
		std::weak_ptr<CRectComponent> m_wpCurComponent;
	};
}