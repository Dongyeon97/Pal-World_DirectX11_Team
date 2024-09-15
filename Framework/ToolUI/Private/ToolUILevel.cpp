#include "pch.h"
#include "ToolUILevel.h"

#include "ToolUIPanel.h"

HRESULT ToolUI::CToolUILevel::Initialize()
{
	GetOwnerWorld();
	//m_wpToolUIPanel = CreateGameObject<CToolUIPanel>(/*L"Test_UI_Panel", L"Layer_Test_Panel"*/);

	return CLevel::Initialize();
}

HRESULT ToolUI::CToolUILevel::BeginPlay()
{
	if (FAILED(CLevel::BeginPlay())) { return E_FAIL; }

	return S_OK;
}

int32 ToolUI::CToolUILevel::PreTick(float _fDeltaSeconds)
{
	switch (CLevel::PreTick(_fDeltaSeconds))
	{
	case -1:	break;
	default:	break;
	}

	return 0;
}

int32 ToolUI::CToolUILevel::Tick(float _fDeltaSeconds)
{
	switch (CLevel::Tick(_fDeltaSeconds))
	{
	case -1:	break;
	default:	break;
	}

	return 0;
}

int32 ToolUI::CToolUILevel::PostTick(float _fDeltaSeconds)
{
	switch (CLevel::PostTick(_fDeltaSeconds))
	{
	case -1:	break;
	default:	break;
	}

	return 0;
}

void ToolUI::CToolUILevel::Release()
{
	CLevel::Release();
}
