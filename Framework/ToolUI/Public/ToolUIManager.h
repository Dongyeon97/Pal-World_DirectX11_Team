#pragma once
#include "Manager.h"

namespace Engine
{
	class CShaderHandler;
	class CRectComponent;
	class CPanel;
}

namespace ToolUI
{
	class CToolUIPanel;

	class CToolUIManager final : public CManager
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		DECLARE_SINGLETON(CToolUIManager)

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
		void AddPanel(const std::wstring& _wstrPanelName, shared_ptr<CToolUIPanel>& _spUIPanel);

		void AddParentPanel(const std::wstring& _wstrParentName, shared_ptr<CPanel>& _spParentPanel);

		bool FindstrName(std::string _strName);

		std::vector<std::string> GetShaderFileNames();

		std::vector<std::string> GetTextureFileName();

	/******************
		Game Object
	******************/
	public:
		std::shared_ptr<CToolUIPanel> FindPanel(const std::wstring& _wstrPanelName)
		{
			if (!m_mapUIPanels.contains(_wstrPanelName)) { return nullptr; }

			return std::shared_ptr<CToolUIPanel>(m_mapUIPanels[_wstrPanelName]);
		}

		std::shared_ptr<CPanel> FindParentsPanel(const std::wstring& _wstrPanelName)
		{
			if (!m_mapParentPanels.contains(_wstrPanelName)) { return nullptr; }

			return std::shared_ptr<CPanel>(m_mapParentPanels[_wstrPanelName]);
		}

	/******************
		Component
	******************/
	public:
		void FindComponent();

		void MoveComponent();

		void ResetComponent();

	/********************
		Methods
	********************/
	public:
		void UIMouseDown();

		void UIMousePress();

		void UIMouseUp();

		void UIMouseClick();

		void UIMouseHover();

		void SavePanelsJson(shared_ptr<CPanel> _spPanel);

		void SavePanel(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer, shared_ptr<CPanel> _spPanel);

		void LoadPanel(std::wstring& _wstrLoadFileName);

		void SaveComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer, shared_ptr<CRectComponent> _spComponent);



	/********************
		Getter/Setter
	********************/
	public:
		std::weak_ptr<CToolUIPanel> GetCurPanel();

		void SetCurPanel(const std::wstring& _wstrPanelName);

		std::vector<const char*> GetPanelNames();

		std::wstring GetCurrentPanelName(const int _iIndex);

		std::weak_ptr<CRectComponent> GetCurComponent() { return m_wpCurRectComponent; }

		void SetCurComponent(weak_ptr<CRectComponent> _wpCurRectComponent) { m_wpCurRectComponent = _wpCurRectComponent; }

		// �ӽ÷� ���� �г��� �����մϴ�.
		void SetTopPanel(shared_ptr<CPanel> _spPanel) { m_spTopPanel = _spPanel; }

		const shared_ptr<CPanel>& GetTopPanel() { return m_spTopPanel; }

		const std::map<std::wstring, std::shared_ptr<CToolUIPanel>>& GetUIPanels() { return m_mapUIPanels; }

		const std::map<std::wstring, std::shared_ptr<CPanel>>& GetParentsUIPanels() { return m_mapParentPanels; }

		std::weak_ptr<CPanel> GetChildPanel();

		void SetChildPanel(wstring _wstrChildPanelName);

		void SelectActiveChildPanel(int32 _iOption);
	/******************
		Data Members
	******************/
	private:
		// �����ϴ� ��� �г��� ��� �ְ� �մϴ�.
		std::map<std::wstring, std::shared_ptr<CToolUIPanel>> m_mapUIPanels;

		// �θ� �гε鸸 ��� �ְ� �մϴ�.
		std::map<std::wstring, std::shared_ptr<CPanel>> m_mapParentPanels;

		std::weak_ptr<CRectComponent> m_wpCurRectComponent;

		std::vector<std::string> m_vecstrPanelNames;

		std::vector<std::string> m_vecstrShaderName;

		std::vector<std::string> m_vecstrTextureName;

		//bool m_bOnce = false;

		int32 m_iComponentIndex = 0;

		int32 m_iPanelIndex = 0;

	private:
		// ��Ŀ����Ʈ���� �Ѱ����� Ŭ���ǰ��մϴ�.
		std::weak_ptr<CRectComponent> m_wpFocusedControlComponent;

		// ��ư, �����̴�, �̹��� �� ��ȣ�ۿ��� �ʿ��� �гε鸸 ���� ��Ƴ����ϴ�.
		std::map<std::wstring, std::weak_ptr<CToolUIPanel>> m_mapInteractiveUIPanels;

		// �ֻ��� �г��� �����մϴ�.
		std::shared_ptr<CPanel> m_spTopPanel;

		bool m_bFocusedControl = false;

		// ���� ���õ� �г��� �����մϴ�.
		std::shared_ptr<CPanel> m_spCurPanel;

		// ���� ���õ� �ڽ� �г��� �����մϴ�.
		std::shared_ptr<CPanel> m_spCurChildPanel;
	};
}