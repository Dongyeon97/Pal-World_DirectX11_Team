#include "pch.h"
#include "ToolUIPanel.h"

#include "RectComponent.h"

std::shared_ptr<CToolUIPanel> ToolUI::CToolUIPanel::Create(const std::shared_ptr<FUIPanelDesc>& _spUIPanelDesc)
{
	const std::shared_ptr<CToolUIPanel> spToolUIPanelObject = make_shared<CToolUIPanel>();
	ENSUREF(spToolUIPanelObject->Initialize(_spUIPanelDesc), L"Failed to initialize uipanel gameobject.");
	return spToolUIPanelObject;
}

HRESULT ToolUI::CToolUIPanel::Initialize(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc)
{
	if(FAILED(CPanel::Initialize(_spGameObjectDesc))) { return E_FAIL;}

	std::shared_ptr<FUIPanelDesc> spUIPanelGameObjectDesc = static_pointer_cast<FUIPanelDesc>(_spGameObjectDesc);

	return S_OK;
}

HRESULT ToolUI::CToolUIPanel::BeginPlay()
{
	if (FAILED(CPanel::BeginPlay())) { return E_FAIL; }

	return S_OK;
}

int32 ToolUI::CToolUIPanel::PreTick(float _fDeltaSeconds)
{
	if (!m_bActive) { return 0; }

	switch (CPanel::PreTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

int32 ToolUI::CToolUIPanel::Tick(float _fDeltaSeconds)
{
	if (!m_bActive) { return 0; }

	switch (CPanel::Tick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

int32 ToolUI::CToolUIPanel::PostTick(float _fDeltaSeconds)
{
	if (!m_bActive) { return 0; }

	switch (CPanel::PostTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

HRESULT ToolUI::CToolUIPanel::EndPlay()
{
	if (FAILED(CPanel::EndPlay())) { return E_FAIL; }

	return S_OK;
}

void ToolUI::CToolUIPanel::Release()
{
	CPanel::Release();
}
