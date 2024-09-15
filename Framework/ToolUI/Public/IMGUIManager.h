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
		// ����� ���̴� ������ �̸��� �����մϴ�.
		std::wstring m_wstrSaveShaderName = L"";
		// �⺻ �ؽ����� �̸��� �����մϴ�.
		std::wstring m_wstrSaveTextureName = L"";
		// ����ũ �ؽ����� �̸��� �����մϴ�.
		std::wstring m_wstrSaveMaskTextureName = L"";
		// ��ư ����� �� �ؽ��� �̸��� �����մϴ�.
		std::wstring m_wstrSaveTextureHoverName = L"";
		// ��ư Ŭ��������/ �����̴��� ��ư�� �ؽ��� �̸��� �����մϴ�. 
		std::wstring m_wstrSaveTextureClickName = L"";
		// ��Ʈ ������ �̸��� �����մϴ�.
		std::wstring m_wstrSaveFontFileName = L"";
		// Owner Object�� �̸��� �����մϴ�.
		std::wstring m_wstrSaveOwnerPanelName = L"";
		// ������ �θ� �г��� �̸��� �����մϴ�.
		std::wstring m_wstrSaveParentPanelName = L"";
		// ������ �ڽ� �г��� �̸��� �����մϴ�.
		std::wstring m_wstrSaveChildPanelName = L"";
		// ������ ������ �����մϴ�.
		Vector4 m_vSaveColor = {1.0f, 1.0f, 1.0f, 1.0f};
		// ���� UI�Ͻ� ���� ������Ʈ�� �����մϴ�.
		std::weak_ptr<CRectComponent> m_wpCurComponent;
	};
}