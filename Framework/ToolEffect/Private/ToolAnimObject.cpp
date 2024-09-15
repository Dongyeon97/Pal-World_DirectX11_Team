#include "pch.h"
#include "ToolAnimObject.h"

#include "MeshComponent.h"

std::shared_ptr<CToolAnimObject> ToolEffect::CToolAnimObject::Create(const std::shared_ptr<FToolAnimObjectDesc>& _spPlayerDesc)
{
	shared_ptr<CToolAnimObject> spAnimObject = make_shared<CToolAnimObject>();
	ENSUREF(spAnimObject->Initialize(_spPlayerDesc), L"Failed to initialize CToolAnimObject.");
	return spAnimObject;
}

HRESULT ToolEffect::CToolAnimObject::Initialize(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc)
{
	if (FAILED(CGameObject::Initialize(_spGameObjectDesc))) { return E_FAIL; }

	InitMeshComponent(static_pointer_cast<FToolAnimObjectDesc>(_spGameObjectDesc));

	return S_OK;
}

HRESULT ToolEffect::CToolAnimObject::BeginPlay()
{
	return CGameObject::BeginPlay();
}

int32 ToolEffect::CToolAnimObject::PreTick(float _fDeltaSeconds)
{
	return CGameObject::PreTick(_fDeltaSeconds);
}

int32 ToolEffect::CToolAnimObject::Tick(float _fDeltaSeconds)
{
	return CGameObject::Tick(_fDeltaSeconds);
}

int32 ToolEffect::CToolAnimObject::PostTick(float _fDeltaSeconds)
{
	return CGameObject::PostTick(_fDeltaSeconds);
}

HRESULT ToolEffect::CToolAnimObject::EndPlay()
{
	return CGameObject::EndPlay();
}

void ToolEffect::CToolAnimObject::Release()
{
	CGameObject::Release();

	if (m_spMeshComponent) { m_spMeshComponent = nullptr; }
}

void ToolEffect::CToolAnimObject::InitMeshComponent(const std::shared_ptr<FToolAnimObjectDesc>& _spPlayerDesc)
{
	// component desc
	FMeshComponentDesc tBodyMeshComponentDesc;
	tBodyMeshComponentDesc.m_wstrComponentName = L"AnimMeshComponent";
	tBodyMeshComponentDesc.m_eRenderType = ERenderType::RENDER_NBLEND;
	tBodyMeshComponentDesc.m_eComponentType = EComponentType::MESH;
	tBodyMeshComponentDesc.m_wpOwnerGameObject = shared_from_this();

	// mesh component desc
	tBodyMeshComponentDesc.m_wstrModelFilePath = _spPlayerDesc->m_wstrAnimModelPath;
	tBodyMeshComponentDesc.m_wstrShaderName = L"FX_AnimMesh_PNT2TBIBW";
	tBodyMeshComponentDesc.m_matPivot = _spPlayerDesc->m_matPivot;

	m_spMeshComponent = CMeshComponent::Create(make_shared<FMeshComponentDesc>(tBodyMeshComponentDesc));
	AddComponent(tBodyMeshComponentDesc.m_wstrComponentName, m_spMeshComponent);
}
