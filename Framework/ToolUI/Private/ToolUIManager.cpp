#include "pch.h"
#include "ToolUIManager.h"
#include "ToolUIPanel.h"

#include "World.h"
#include "Level.h"

#include "ShaderHandler.h"

#include "EngineModule.h"
#include "InputManager.h"

#include "RectComponent.h"
#include "ImageComponent.h"
#include "SliderComponent.h"
#include "ButtonComponent.h"
#include "AnimationImageComponent.h"
#include "ProgressBarComponent.h"
#include "TextComponent.h"

#include "StringUtility.h"

IMPLEMENT_SINGLETON(CToolUIManager)

HRESULT ToolUI::CToolUIManager::Initialize()
{
	return S_OK;
}

int32 ToolUI::CToolUIManager::Tick(float _fDeltaSeconds)
{
	return 0;
}

void ToolUI::CToolUIManager::Render()
{
}

void ToolUI::CToolUIManager::Release()
{
	m_vecstrPanelNames.clear();

	for (auto& pair : m_mapUIPanels)
	{
		pair.second->Release();
		pair.second = nullptr;
	}

	m_mapUIPanels.clear();

	for (auto& pair : m_mapParentPanels)
	{
		pair.second->Release();
		pair.second = nullptr;
	}

	m_mapParentPanels.clear();

	m_spCurPanel = nullptr;

	m_spTopPanel = nullptr;

	m_spCurChildPanel = nullptr;

	m_spInstance = nullptr;
}

void ToolUI::CToolUIManager::AddPanel(const std::wstring& _wstrPanelName, shared_ptr<CToolUIPanel>& _spUIPanel)
{
	if (m_mapUIPanels.contains(_wstrPanelName)) { return; }

	_spUIPanel->SetGameObjectName(_wstrPanelName);

	m_mapUIPanels.emplace(_wstrPanelName, _spUIPanel);
}

void ToolUI::CToolUIManager::AddParentPanel(const std::wstring& _wstrParentName, shared_ptr<CPanel>& _spParentPanel)
{
	if (m_mapParentPanels.contains(_wstrParentName)) { return; }

	_spParentPanel->SetGameObjectName(_wstrParentName);

	m_mapParentPanels.emplace(_wstrParentName, _spParentPanel);
}

bool ToolUI::CToolUIManager::FindstrName(std::string _strName)
{
	for (auto& iter : m_vecstrPanelNames)
	{
		if (iter == _strName)
		{
			return true;
		}
	}

	return false;
}

std::vector<std::string> ToolUI::CToolUIManager::GetShaderFileNames()
{
	// ���� �� ���� �ѹ��� ShaderHandler���� mapEffectShader�� �����ɴϴ�. // ������ �ȸ¾Ƽ� �ϴ��� ��� �ε��ϴ� �����Դϴ�.
	m_vecstrShaderName.clear();

	std::unordered_map<std::wstring, std::shared_ptr<CEffectShader>> mapEffectShaders = CShaderHandler::GetInstance()->GetEffectShaders();

	for (auto& pair : mapEffectShaders)
	{
		std::string strName;
		strName.assign(pair.first.begin(), pair.first.end());
		m_vecstrShaderName.push_back(strName);
	}

	return m_vecstrShaderName;
}

std::vector<std::string> ToolUI::CToolUIManager::GetTextureFileName()
{
	m_vecstrTextureName.clear();

	//std::unordered_map<std::wstring, std::shared_ptr<>>

	return std::vector<std::string>();
}

void ToolUI::CToolUIManager::FindComponent()
{
	if (CEngineModule::GetInstance()->GetInputManager()->IsMouseDown(EMouseActionType::RIGHT_BUTTON))
	{
		// ������Ʈ�� ������ �ִ� depth�� ���ؼ� ���� ����� ���� �����ϰ� �մϴ�.
		int32 iDepth = 0;
		for (auto& pair : m_spTopPanel->GetMapComponents())
		{
			if (EComponentType::RECT != pair.second->GetComponentType()) { continue; }
			else
			{
				// ���� �ȿ� ������ ���� ������Ʈ�� �������ݴϴ�.
				if (dynamic_pointer_cast<CRectComponent>(pair.second)->IsInMouse())
				{
					if (iDepth <= dynamic_pointer_cast<CRectComponent>(pair.second)->GetDepth())
					{
						SetCurComponent(dynamic_pointer_cast<CRectComponent>(pair.second));
						iDepth = dynamic_pointer_cast<CRectComponent>(pair.second)->GetDepth();
					}
				}
			}
		}

		for (auto& iter : m_spTopPanel->GetVecChildGameObjects())
		{
			for (auto& pair : dynamic_pointer_cast<CPanel>(iter)->GetMapComponents())
			{
				if (EComponentType::RECT != pair.second->GetComponentType()) { continue; }
				else
				{
					// ���� �ȿ� ������ ���� ������Ʈ�� �������ݴϴ�.
					if (dynamic_pointer_cast<CRectComponent>(pair.second)->IsInMouse())
					{
						if (iDepth <= dynamic_pointer_cast<CRectComponent>(pair.second)->GetDepth())
						{
							SetCurComponent(dynamic_pointer_cast<CRectComponent>(pair.second));
							iDepth = dynamic_pointer_cast<CRectComponent>(pair.second)->GetDepth();
						}
					}
				}
			}
		}
	}
}

void ToolUI::CToolUIManager::MoveComponent()
{
	if (CEngineModule::GetInstance()->GetInputManager()->IsMousePressed(EMouseActionType::RIGHT_BUTTON))
	{
		if (nullptr != m_wpCurRectComponent.lock())
		{
			if (m_wpCurRectComponent.lock()->IsInMouse())
			{
				m_wpCurRectComponent.lock()->AddMouseMove();
			}
		}
	}
}

void ToolUI::CToolUIManager::ResetComponent()
{
	if (nullptr != m_wpCurRectComponent.lock())
	{
		if (CEngineModule::GetInstance()->GetInputManager()->IsMouseUp(EMouseActionType::RIGHT_BUTTON))
		{
			m_wpCurRectComponent.lock()->SetMouseReset(false);
		}
	}
}

// Ŭ���̾�Ʈ UI Manager������ ����� �� �ְ��մϴ�.
void ToolUI::CToolUIManager::UIMouseDown()
{
	// ��ȣ�ۿ��� ������ ������Ʈ�����մϴ�. // uitype�� ��ư, �����̴�, �̹����� ���
	// ���콺�� Ŭ���� ��Ȳ���� ������ ���� ���� ��Ʈ ������ ���콺 ��ġ�� ��ġ�ϴ� ������Ʈ�� �ִٸ�
	// �� ������Ʈ�� ��Ŀ�� ��Ʈ���� ���ְ� �Ŵ����� weak_ptr�� ä���ݴϴ�.
	
	// �� ��� Ŭ���� �� �ִ� �г��� �ϳ������ϱ� ������ ����â�� �г��� Ȯ���ϰ� ������ �˴ϴ�.
	if (CEngineModule::GetInstance()->GetInputManager()->IsMouseDown(EMouseActionType::LEFT_BUTTON))
	{
		for (auto& pair : m_spTopPanel->GetMapComponents())
		{
			if (EComponentType::RECT != pair.second->GetComponentType()) { continue; }
			if (EUIType::BUTTON == dynamic_pointer_cast<CRectComponent>(pair.second)->GetUIType() ||
				EUIType::SLIDER == dynamic_pointer_cast<CRectComponent>(pair.second)->GetUIType())
			{
				// �� �� ���콺 ��ġ�� ��Ʈ�� ������ ��ġ�ϸ鼭 �̹��� �� ����Ʈ�� �ƴ� ������Ʈ�� �˻��մϴ�.
				if (dynamic_pointer_cast<CRectComponent>(pair.second)->IsInMouse())
				{
					dynamic_pointer_cast<CRectComponent>(pair.second)->SetFocusedControl(true);
					m_wpFocusedControlComponent = dynamic_pointer_cast<CRectComponent>(pair.second);
				}
			}
			else if (EUIType::STATIC_IMAGE == dynamic_pointer_cast<CRectComponent>(pair.second)->GetUIType())
			{
				// �� �� ���콺 ��ġ�� ��Ʈ�� ������ ��ġ�ϸ鼭 �̹��� �� ����Ʈ�� �ƴ� ������Ʈ�� �˻��մϴ�.
				if (dynamic_pointer_cast<CRectComponent>(pair.second)->IsInMouse() &&
					EImageType::DEFAULT != dynamic_pointer_cast<CImageComponent>(pair.second)->GetImageType())
				{
					dynamic_pointer_cast<CRectComponent>(pair.second)->SetFocusedControl(true);
					m_wpFocusedControlComponent = dynamic_pointer_cast<CRectComponent>(pair.second);
				}
			}
			else { continue; }
		}

		for (auto& iter : m_spTopPanel->GetVecChildGameObjects())
		{
			for (auto& pair : dynamic_pointer_cast<CPanel>(iter)->GetMapComponents())
			{
				if (EComponentType::RECT != pair.second->GetComponentType()) { continue; }
				if (EUIType::BUTTON == dynamic_pointer_cast<CRectComponent>(pair.second)->GetUIType() ||
					EUIType::SLIDER == dynamic_pointer_cast<CRectComponent>(pair.second)->GetUIType())
				{
					// �� �� ���콺 ��ġ�� ��Ʈ�� ������ ��ġ�ϸ鼭 �̹��� �� ����Ʈ�� �ƴ� ������Ʈ�� �˻��մϴ�.
					if (dynamic_pointer_cast<CRectComponent>(pair.second)->IsInMouse())
					{
						dynamic_pointer_cast<CRectComponent>(pair.second)->SetFocusedControl(true);
						m_wpFocusedControlComponent = dynamic_pointer_cast<CRectComponent>(pair.second);
					}
				}
				else if (EUIType::STATIC_IMAGE == dynamic_pointer_cast<CRectComponent>(pair.second)->GetUIType())
				{
					// �� �� ���콺 ��ġ�� ��Ʈ�� ������ ��ġ�ϸ鼭 �̹��� �� ����Ʈ�� �ƴ� ������Ʈ�� �˻��մϴ�.
					if (dynamic_pointer_cast<CRectComponent>(pair.second)->IsInMouse() &&
						EImageType::DEFAULT != dynamic_pointer_cast<CImageComponent>(pair.second)->GetImageType())
					{
						dynamic_pointer_cast<CRectComponent>(pair.second)->SetFocusedControl(true);
						m_wpFocusedControlComponent = dynamic_pointer_cast<CRectComponent>(pair.second);
					}
				}
				else { continue; }
			}
		}
	}
}

void ToolUI::CToolUIManager::UIMousePress()
{
	// ��ư�� ��� ������ ����� ���µǾ���ϰ� �����̴��� ���� ��� ���ӵǾ���մϴ�.
	// �̹���(�κ��丮 ������)�� ��� ���콺�� ����ٴϰ� �˴ϴ�.
	if (nullptr != m_wpFocusedControlComponent.lock())
	{
		if (EUIType::SLIDER == m_wpFocusedControlComponent.lock()->GetUIType())
		{
			dynamic_pointer_cast<CSliderComponent>(m_wpFocusedControlComponent.lock())->IsDragged();
		}
	}
}

void ToolUI::CToolUIManager::UIMouseUp()
{
	// ���콺�� ������ �� ��Ŀ�� ��Ʈ���� �ʱ�ȭ���ݴϴ�.
	if (nullptr != m_wpFocusedControlComponent.lock())
	{
		if (CEngineModule::GetInstance()->GetInputManager()->IsMouseUp(EMouseActionType::LEFT_BUTTON))
		{
			if (EUIType::BUTTON == m_wpFocusedControlComponent.lock()->GetUIType())
			{
				dynamic_pointer_cast<CButtonComponent>(m_wpFocusedControlComponent.lock())->AddButtonClickEvent([this]() { this->UIMouseClick(); });
				dynamic_pointer_cast<CButtonComponent>(m_wpFocusedControlComponent.lock())->IsClicked();
			}

			m_wpFocusedControlComponent.lock()->SetMouseReset(false);
			m_wpFocusedControlComponent.lock()->SetFocusedControl(false);
			m_wpFocusedControlComponent.reset();
		}
	}
}

void ToolUI::CToolUIManager::UIMouseClick()
{
	int32 a = 5;
}

void ToolUI::CToolUIManager::UIMouseHover()
{
	// ������ ������Ʈ�� �ִ� ����Դϴ�.
	if (nullptr != m_wpFocusedControlComponent.lock())
	{
		if (EUIType::BUTTON == m_wpFocusedControlComponent.lock()->GetUIType())
		{
			dynamic_pointer_cast<CButtonComponent>(m_wpFocusedControlComponent.lock())->IsHoverd();
		}
	}
	// ���� ������ ������Ʈ�� ���� ����Դϴ�.
	else
	{
		for (auto& pair : m_spTopPanel->GetMapComponents())
		{
			if (EComponentType::RECT != pair.second->GetComponentType()) { continue; }
			if (EUIType::BUTTON == dynamic_pointer_cast<CRectComponent>(pair.second)->GetUIType())
			{
				dynamic_pointer_cast<CButtonComponent>(pair.second)->IsHoverd();
			}
		}

		for (auto& iter : m_spTopPanel->GetVecChildGameObjects())
		{
			for (auto& pair : dynamic_pointer_cast<CPanel>(iter)->GetMapComponents())
			{
				if (EComponentType::RECT != pair.second->GetComponentType()) { continue; }
				if (EUIType::BUTTON == dynamic_pointer_cast<CRectComponent>(pair.second)->GetUIType())
				{
					dynamic_pointer_cast<CButtonComponent>(pair.second)->IsHoverd();
				}
			}
		}
	}
}

std::weak_ptr<CToolUIPanel> ToolUI::CToolUIManager::GetCurPanel()
{
	//for (auto& pair : m_mapUIPanels)
	//{
	//	if (pair.second->GetActive())
	//	{
	//		return pair.second;
	//	}
	//}

	return static_pointer_cast<CToolUIPanel>(m_spCurPanel);
	//return std::weak_ptr<CToolUIPanel>();
}

void ToolUI::CToolUIManager::SetCurPanel(const std::wstring& _wstrPanelName)
{
	m_spCurPanel = FindParentsPanel(_wstrPanelName);

	for (auto& pair : m_mapParentPanels)//m_mapUIPanels)
	{
		if (_wstrPanelName == pair.first)
		{
			pair.second->SetActive(true);
		}
		else
		{
			pair.second->SetActive(false);
		}
	}
}

std::vector<const char*> ToolUI::CToolUIManager::GetPanelNames()
{
	std::vector<const char*> vecNames;


	for (auto& pair : m_mapUIPanels)
	{
		std::string str;
		str.assign(pair.first.begin(), pair.first.end());

		if (!FindstrName(str))
		{
			m_vecstrPanelNames.push_back(str);
		}
	}

	for (auto& iter : m_vecstrPanelNames)
	{
		vecNames.push_back(iter.c_str());
	}

	return vecNames;
}

std::wstring ToolUI::CToolUIManager::GetCurrentPanelName(const int _iIndex)
{
	std::wstring wstrName;
	wstrName.assign(m_vecstrPanelNames[_iIndex].begin(), m_vecstrPanelNames[_iIndex].end());

	return wstrName;
}

std::weak_ptr<CPanel> ToolUI::CToolUIManager::GetChildPanel()
{
	return m_spCurChildPanel;
}

void ToolUI::CToolUIManager::SetChildPanel(wstring _wstrChildPanelName)
{
	shared_ptr<CPanel> spPanel = static_pointer_cast<CPanel>(GetCurPanel().lock());

	m_spCurChildPanel = dynamic_pointer_cast<CPanel>(spPanel->FindChildGameObject(_wstrChildPanelName));
}

void ToolUI::CToolUIManager::SelectActiveChildPanel(int32 _iOption)
{
	// ��� �ڽ� �г��� active�� ���ݴϴ�.
	if (0 == _iOption)
	{
		for (auto& iter : m_spCurPanel->GetVecChildGameObjects())
		{
			iter->SetActive(true);
		}
	}
	// ���� �ڽ� �г��� active�� ���ְ� �������� ���ݴϴ�.
	else if (1 == _iOption)
	{
		for (auto& iter : m_spCurPanel->GetVecChildGameObjects())
		{
			if (iter == m_spCurChildPanel)
			{
				iter->SetActive(true);
			}
			else
			{
				iter->SetActive(false);
			}
		}
	}
}

void ToolUI::CToolUIManager::SavePanelsJson(shared_ptr<CPanel> _spPanel)
{
	rapidjson::StringBuffer strBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strBuffer);

	writer.StartObject();

	writer.Key("Child Panel Size");
	writer.Int(_spPanel->GetVecChildGameObjects().size());

	SavePanel(writer, _spPanel);
	m_iPanelIndex++;
	if (0 != _spPanel->GetVecChildGameObjects().size())
	{
		for (auto& iter : _spPanel->GetVecChildGameObjects())
		{
			SavePanel(writer, dynamic_pointer_cast<CPanel>(iter));
			m_iPanelIndex++;
		}
	}
	m_iPanelIndex = 0;
	// �г��� �ڽİ��ӿ�����Ʈ ���θ� �����մϴ�.
	//�ڽ� ���ӿ�����Ʈ�� ������ ���� �ٸ��� �����մϴ�.


	writer.EndObject();

	string strPanelName = CStringUtility::ToString(_spPanel->GetGameObjectName()).c_str();

	ofstream ofs("../../Resource/UIs/" + strPanelName + ".json");
	if (!ofs.is_open()) { return; }
	ofs << strBuffer.GetString();
	ofs.close();
}

void ToolUI::CToolUIManager::SavePanel(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer, shared_ptr<CPanel> _spPanel)
{
	std::string strPanels = "Panels" + to_string(m_iPanelIndex);
	_writer.Key(strPanels.c_str());
	_writer.StartObject();

	// ���ӿ�����Ʈ�� �̸��� ����մϴ�.
	_writer.Key("Panel Name");
	_writer.String(CStringUtility::ToString(_spPanel->GetGameObjectName()).c_str());
	// ���ӿ�����Ʈ�� Projection Ÿ���� ����մϴ�.
	_writer.Key("Panel Projection Type");
	_writer.Int((int32)_spPanel->GetUIProjectionType());

	// �г��� Depth�� ����մϴ�.
	_writer.Key("Panel Depth");
	_writer.Int(_spPanel->GetDepth());

	if (nullptr != _spPanel->GetPivotComponent())
	{
		_writer.Key("Panel Pivot Component Name");
		_writer.String(CStringUtility::ToString(_spPanel->GetPivotComponent()->GetComponentName()).c_str());
	}
	else
	{
		_writer.Key("Panel Pivot Component Name");
		_writer.String("");
	}

	// transformcomponent�� �ϳ� ���ݴϴ�.
	_writer.Key("Components Size");
	_writer.Uint(_spPanel->GetMapComponents().size() - 1);

	for (auto& pair : _spPanel->GetMapComponents())
	{
		if (pair.second->GetComponentType() != EComponentType::RECT) { continue; }

		SaveComponent(_writer, dynamic_pointer_cast<CRectComponent>(pair.second));
		m_iComponentIndex++;
	}

	m_iComponentIndex = 0;

	_writer.EndObject();
}

void ToolUI::CToolUIManager::LoadPanel(std::wstring& _wstrLoadFileName)
{
	shared_ptr<CPanel> spPanel = CPanel::CreateFromJson(_wstrLoadFileName);
	if (nullptr == spPanel)
	{
		MESSAGE_BOX(L"There are no panels on the list");
		return;
	}
	// ���� �г��� ������ �־��ݴϴ�.
	CWorld::GetInstance()->GetCurLevel()->AddGameObject(spPanel);
	//shared_ptr<CToolUIPanel> spUIPanel = static_pointer_cast<CToolUIPanel>(spPanel);
	AddParentPanel(spPanel->GetGameObjectName(), spPanel);

	// �ε� ���ϰ� istool������ �� ���ݴϴ�.
	for (auto& pair : spPanel->GetMapComponents())
	{
		static_pointer_cast<CRectComponent>(pair.second)->IsTool(true);
		for (auto& Child : spPanel->GetVecChildGameObjects())
		{
			for (auto& ChildPair : static_pointer_cast<CPanel>(Child)->GetMapComponents())
			{
				static_pointer_cast<CRectComponent>(pair.second)->IsTool(true);
			}
		}
	}
}

void ToolUI::CToolUIManager::SaveComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer, shared_ptr<CRectComponent> _spComponent)
{
	std::string strComponents = "Components" + to_string(m_iComponentIndex);
	_writer.Key(strComponents.c_str());
	_writer.StartArray();

	_writer.StartObject();

	// component�� �ҼӵǾ� �ִ� ���� �������� �������ݴϴ�.
	_writer.Key("Component Name");
	_writer.String(CStringUtility::ToString(_spComponent->GetComponentName()).c_str());
	// componenttype�� rect�� ����ó���� �����Դϴ�.
	_writer.Key("Render Type");
	_writer.Int((int32)_spComponent->GetRenderType());
	// rect component�� �ҼӵǾ� �ִ� ���� �������� �������ݴϴ�.
	_writer.Key("UI Type");
	_writer.Int((int32)_spComponent->GetUIType());
	_writer.Key("ShaderFileName");
	_writer.String(CStringUtility::ToString(_spComponent->GetShaderFileName()).c_str());
	_writer.Key("PassIndex");
	_writer.Int(_spComponent->GetPassIndex());
	// ��ġ ���� ������ �����մϴ�.
	_writer.Key("Center");
	_writer.StartArray();
	_writer.Double(_spComponent->GetCenterPosition().x);
	_writer.Double(_spComponent->GetCenterPosition().y);
	_writer.EndArray();

	_writer.Key("World Position");
	_writer.StartArray();
	_writer.Double(_spComponent->GetRectTransform(ETransformType::POSITION).x);
	_writer.Double(_spComponent->GetRectTransform(ETransformType::POSITION).y);
	_writer.Double(_spComponent->GetRectTransform(ETransformType::POSITION).z);
	_writer.EndArray();

	_writer.Key("Scale");
	_writer.StartArray();
	_writer.Double(_spComponent->GetRectScale().x);
	_writer.Double(_spComponent->GetRectScale().y);
	_writer.Double(_spComponent->GetRectScale().z);
	_writer.EndArray();

	_writer.Key("Angle");
	_writer.Double(_spComponent->GetRectAngle());

	_writer.Key("Depth");
	_writer.Int(_spComponent->GetDepth());

	_writer.Key("Ratio");
	_writer.Double(_spComponent->GetRatio());

	_writer.Key("Color");
	_writer.StartArray();
	_writer.Double(_spComponent->GetColor().x);
	_writer.Double(_spComponent->GetColor().y);
	_writer.Double(_spComponent->GetColor().z);
	_writer.Double(_spComponent->GetColor().w);
	_writer.EndArray();

	_writer.Key("Alpha");
	_writer.Double(_spComponent->GetAlpha());

	_writer.Key("Mask Texture Name");
	_writer.String(CStringUtility::ToString(_spComponent->GetMaskTextureFileName()).c_str());

	_writer.Key("IsMask");
	_writer.Bool(_spComponent->GetBoolMask());
	_writer.Key("IsAlpha");
	_writer.Bool(_spComponent->GetBoolAlpha());
	_writer.Key("IsColor");
	_writer.Bool(_spComponent->GetBoolColor());

	_writer.Key("RelativeCenter");
	_writer.StartArray();
	_writer.Double(_spComponent->GetScreenRelativeCenter().x);
	_writer.Double(_spComponent->GetScreenRelativeCenter().y);
	_writer.EndArray();

	_writer.Key("IsRelative");
	_writer.Bool(_spComponent->IsRelativeCenter());

	_writer.Key("IsInteraction");
	_writer.Bool(_spComponent->GetInteraction());

	// ������ ������Ʈ�� �°� ĳ������ �Լ��� ȣ���մϴ�.
	if (EUIType::STATIC_IMAGE == _spComponent->GetUIType())
	{
		shared_ptr<CImageComponent> spImageComponent = dynamic_pointer_cast<CImageComponent>(_spComponent);
		_writer.Key("Image Texture Name");
		_writer.String(CStringUtility::ToString(spImageComponent->GetTextureName()).c_str());

		_writer.Key("Image Type");
		_writer.Int((int32)spImageComponent->GetImageType());
	}
	else if (EUIType::BUTTON == _spComponent->GetUIType())
	{
		shared_ptr<CButtonComponent> spButtonComponent = dynamic_pointer_cast<CButtonComponent>(_spComponent);
		_writer.Key("Static Texture Name");
		_writer.String(CStringUtility::ToString(spButtonComponent->GetStaticTextureFileName()).c_str());

		_writer.Key("Hover Texture Name");
		_writer.String(CStringUtility::ToString(spButtonComponent->GetHoverTextureFileName()).c_str());

		_writer.Key("Click Texture Name"); 
		_writer.String(CStringUtility::ToString(spButtonComponent->GetClickTextureFileName()).c_str());
	}
	else if (EUIType::SLIDER == _spComponent->GetUIType())
	{
		shared_ptr<CSliderComponent> spSliderComponent = dynamic_pointer_cast<CSliderComponent>(_spComponent);
		_writer.Key("Slider Texture Name");
		_writer.String(CStringUtility::ToString(spSliderComponent->GetTextureFileName()).c_str());

		_writer.Key("BackGround Component Name");
		_writer.String(CStringUtility::ToString(spSliderComponent->GetBackGroundComponentName()).c_str());
	}
	else if (EUIType::CIRCULAR_PROGRESSBAR == _spComponent->GetUIType() || EUIType::LINEAR_HORIZONTAL_PROGRESSBAR == _spComponent->GetUIType())
	{
		shared_ptr<CProgressBarComponent> spProgressBarComponent = dynamic_pointer_cast<CProgressBarComponent>(_spComponent);
		_writer.Key("Progress Texture Name");
		_writer.String(CStringUtility::ToString(spProgressBarComponent->GetTextureFileName()).c_str());
	}
	else if (EUIType::DYNAMIC_IMAGE == _spComponent->GetUIType())
	{
		shared_ptr<CAnimationImageComponent> spAniImageComponent = dynamic_pointer_cast<CAnimationImageComponent>(_spComponent);
		// 
		_writer.Key("Animation Texture Name");
		_writer.String(CStringUtility::ToString(spAniImageComponent->GetTextureFileName()).c_str());

		_writer.Key("EasingType");
		_writer.Int(static_cast<int>(spAniImageComponent->GetEasingType()));

		_writer.Key("Loop");
		_writer.Bool(spAniImageComponent->GetLoop());

		_writer.Key("Start Index");
		_writer.Int(spAniImageComponent->GetStartIndex());

		_writer.Key("End Index");
		_writer.Int(spAniImageComponent->GetEndIndex());

		_writer.Key("KeyFrame Size");
		_writer.Int(spAniImageComponent->GetKeyFrames()->size());

		_writer.Key("KeyFrames");
		_writer.StartArray();

		int iIndex = 0;
		// Ű�����ӵ��� �������ݴϴ�.
		std::vector<pair<float, FImageAnimationFrameDesc>> vecKeyFrames = *spAniImageComponent->GetKeyFrames();
		for (auto& pair : vecKeyFrames)
		{
			//_writer.Key("KeyFrame" + to_string(iIndex));
			_writer.StartObject();

			string strKeyFrameIndex = "KeyFrame" + to_string(iIndex);
			_writer.Key(strKeyFrameIndex.c_str());
			_writer.StartArray();
			_writer.StartObject();
			//_writer.String(strKeyFrameIndex.data());

			_writer.Key("KeyFrame Name");
			_writer.Double(pair.first);

			_writer.Key("KeyFrame Time");
			_writer.Double(pair.second.fTime);

			_writer.Key("KeyFrame Position");
			_writer.StartArray();
			_writer.Double(pair.second.vPosition.x);
			_writer.Double(pair.second.vPosition.y);
			_writer.Double(pair.second.vPosition.z);
			_writer.EndArray();

			_writer.Key("KeyFrame RelativeCenter");
			_writer.StartArray();
			_writer.Double(pair.second.vRelativeCenter.x);
			_writer.Double(pair.second.vRelativeCenter.y);
			_writer.EndArray();

			_writer.Key("KeyFrame Scale");
			_writer.StartArray();
			_writer.Double(pair.second.vScale.x);
			_writer.Double(pair.second.vScale.y);
			_writer.Double(pair.second.vScale.z);
			_writer.EndArray();

			_writer.Key("KeyFrame Angle");
			_writer.Double(pair.second.fAngle);

			_writer.Key("KeyFrame Color");
			_writer.StartArray();
			_writer.Double(pair.second.vColor.x);
			_writer.Double(pair.second.vColor.y);
			_writer.Double(pair.second.vColor.z);
			_writer.Double(pair.second.vColor.w);
			_writer.EndArray();

			_writer.Key("KeyFrame Alpha");
			_writer.Double(pair.second.fAlpha);

			// ���� �Ⱦ��� ���Դϴ�.
			_writer.Key("KeyFrame Speed");
			_writer.Double(pair.second.fSpeed);

			_writer.EndObject();
			_writer.EndArray();

			_writer.EndObject();

			iIndex++;
		}

		//_writer.EndObject();

		_writer.EndArray();
	}
	else if (EUIType::TEXT == _spComponent->GetUIType())
	{
		shared_ptr<CTextComponent> spTextComponent = dynamic_pointer_cast<CTextComponent>(_spComponent);

		_writer.Key("Font File Name");
		_writer.String(CStringUtility::ToString(spTextComponent->GetFontFileName()).c_str());

		_writer.Key("Text");
		_writer.String(CStringUtility::ToString(spTextComponent->GetText()).c_str(), spTextComponent->GetText().length());

		FFontDesc fFontDesc = spTextComponent->GetFontDesc();

		_writer.Key("Origin");
		_writer.StartArray();
		_writer.Double(fFontDesc.m_vOrigin.x);
		_writer.Double(fFontDesc.m_vOrigin.y);
		_writer.EndArray();

		_writer.Key("Option");
		_writer.Int(fFontDesc.m_iOption);

		_writer.Key("Text Type");
		_writer.Int((int32)spTextComponent->GetTextType());
	}
	else if (EUIType::FRAME_IMAGE == _spComponent->GetUIType())
	{
		shared_ptr<CAnimationImageComponent> spAniImageComponent = dynamic_pointer_cast<CAnimationImageComponent>(_spComponent);
		// 
		_writer.Key("Animation Texture Name");
		_writer.String(CStringUtility::ToString(spAniImageComponent->GetTextureFileName()).c_str());

		_writer.Key("EasingType");
		_writer.Int(static_cast<int>(spAniImageComponent->GetEasingType()));

		_writer.Key("Loop");
		_writer.Bool(spAniImageComponent->GetLoop());

		_writer.Key("Row");
		_writer.Int(spAniImageComponent->GetRow());

		_writer.Key("Coloumn");
		_writer.Int(spAniImageComponent->GetColoumn());

		_writer.Key("Next Time");
		_writer.Double(spAniImageComponent->GetNextTime());
	}
	else if (EUIType::LINEAR_INTERPOLATION_PROGRESSBAR == _spComponent->GetUIType() || EUIType::CIRCLE_INTERPOLATION_PROGRESSBAR == _spComponent->GetUIType())
	{
		shared_ptr<CProgressBarComponent> spProgressBarComponent = dynamic_pointer_cast<CProgressBarComponent>(_spComponent);
		_writer.Key("Progress Texture Name");
		_writer.String(CStringUtility::ToString(spProgressBarComponent->GetTextureFileName()).c_str());

		_writer.Key("Other Ratio");
		_writer.Double(spProgressBarComponent->GetOtherRatio());

		_writer.Key("Other Color");
		_writer.StartArray();
		_writer.Double(spProgressBarComponent->GetOtherColor().x);
		_writer.Double(spProgressBarComponent->GetOtherColor().y);
		_writer.Double(spProgressBarComponent->GetOtherColor().z);
		_writer.Double(spProgressBarComponent->GetOtherColor().w);
		_writer.EndArray();
	}

	_writer.EndObject();

	_writer.EndArray();
}

