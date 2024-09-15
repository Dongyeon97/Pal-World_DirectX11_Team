#include "pch.h"
#include "EffectMeshObject.h"

#include "EffectMeshComponent.h"

std::shared_ptr<CEffectMeshObject> ToolEffect::CEffectMeshObject::Create(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc, FEffectMeshInfo _tEffectMeshInfo)
{
	shared_ptr<CEffectMeshObject> spInstance = make_shared<CEffectMeshObject>();
	CHECKF(spInstance, L"CEffectObject : Create Failed");

	spInstance->SetObjectEffectMeshInfo(_tEffectMeshInfo);
	spInstance->Initialize(_spGameObjectDesc);

	return spInstance;
}

std::shared_ptr<CEffectMeshObject> ToolEffect::CEffectMeshObject::CreateFromJson(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc, const std::wstring& _wstrJsonFileName)
{
	shared_ptr<CEffectMeshObject> spInstance = make_shared<CEffectMeshObject>();
	CHECKF(spInstance, L"CEffectObject : Create Failed");

	spInstance->InitializeFromJson(_spGameObjectDesc, _wstrJsonFileName);

	return spInstance;
}

HRESULT ToolEffect::CEffectMeshObject::Initialize(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc)
{
	if (FAILED(CEffectBase::Initialize(_spGameObjectDesc))) { return E_FAIL; }

	shared_ptr<FEffectMeshObjectDesc> spEffectObjectDesc = static_pointer_cast<FEffectMeshObjectDesc>(_spGameObjectDesc);

	/* gameobject */
	m_matOrigin = Matrix::CreateScale(m_tEffectMeshInfo.m_vScale) *
				  Matrix::CreateRotationX(m_tEffectMeshInfo.m_vRotation.x) *
				  Matrix::CreateRotationY(m_tEffectMeshInfo.m_vRotation.y) *
				  Matrix::CreateRotationZ(m_tEffectMeshInfo.m_vRotation.z) *
				  Matrix::CreateTranslation(m_tEffectMeshInfo.m_vTranslation);

	/* component */
	FEffectMeshComponentDesc tEffectMeshDesc;

	tEffectMeshDesc.m_wpOwnerGameObject = shared_from_this();
	tEffectMeshDesc.m_eComponentType = EComponentType::MESH;
	tEffectMeshDesc.m_wstrComponentName = L"Effect Mesh Component";
	tEffectMeshDesc.m_eRenderType = m_tEffectMeshInfo.m_eRenderType;

	tEffectMeshDesc.m_eEffectType = EEffectType::EFFECTMESH;

	tEffectMeshDesc.m_wstrJsonFileName = L"";
	tEffectMeshDesc.m_wstrModelFilePath = spEffectObjectDesc->m_wstrModelFilePath;
	tEffectMeshDesc.m_matPivot = spEffectObjectDesc->m_matPivot;

	m_spEffectMesh = CEffectMeshComponent::Create(make_shared<FEffectMeshComponentDesc>(tEffectMeshDesc), m_tEffectMeshInfo);
	AddComponent(L"EffectMesh Component", m_spEffectMesh);

	return S_OK;
}

HRESULT ToolEffect::CEffectMeshObject::InitializeFromJson(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc, const std::wstring& _wstrJsonFileName)
{
	if (FAILED(CEffectBase::Initialize(_spGameObjectDesc))) { return E_FAIL; }

	shared_ptr<FEffectMeshObjectDesc> spEffectObjectDesc = static_pointer_cast<FEffectMeshObjectDesc>(_spGameObjectDesc);

	/* gameobject */
	m_matOrigin = Matrix::CreateScale(m_tEffectMeshInfo.m_vScale) *
				  Matrix::CreateRotationX(m_tEffectMeshInfo.m_vRotation.x) *
				  Matrix::CreateRotationY(m_tEffectMeshInfo.m_vRotation.y) *
				  Matrix::CreateRotationZ(m_tEffectMeshInfo.m_vRotation.z) *
				  Matrix::CreateTranslation(m_tEffectMeshInfo.m_vTranslation);

	/* component */
	FEffectMeshComponentDesc tEffectMeshDesc;

	tEffectMeshDesc.m_wpOwnerGameObject = shared_from_this();
	tEffectMeshDesc.m_eComponentType = EComponentType::MESH;
	tEffectMeshDesc.m_wstrComponentName = L"Effect Mesh Component";
	tEffectMeshDesc.m_eRenderType = m_tEffectMeshInfo.m_eRenderType;

	tEffectMeshDesc.m_eEffectType = EEffectType::EFFECTMESH;

	tEffectMeshDesc.m_wstrJsonFileName = spEffectObjectDesc->m_wstrJsonFileName;
	tEffectMeshDesc.m_wstrModelFilePath = L"";
	tEffectMeshDesc.m_matPivot = spEffectObjectDesc->m_matPivot;

	m_spEffectMesh = CEffectMeshComponent::CreateFromJson(make_shared<FEffectMeshComponentDesc>(tEffectMeshDesc), _wstrJsonFileName);
	AddComponent(L"Effect Mesh Component", m_spEffectMesh);

	return S_OK;
}

HRESULT ToolEffect::CEffectMeshObject::BeginPlay()
{
	if (FAILED(CEffectBase::BeginPlay())) { return E_FAIL; }

	return S_OK;
}

int32 ToolEffect::CEffectMeshObject::PreTick(float _fDeltaSeconds)
{
	switch (CEffectBase::PreTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return S_OK;
}

int32 ToolEffect::CEffectMeshObject::Tick(float _fDeltaSeconds)
{
	switch (CEffectBase::Tick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return S_OK;
}

int32 ToolEffect::CEffectMeshObject::PostTick(float _fDeltaSeconds)
{
	switch (CEffectBase::PostTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return S_OK;
}

HRESULT ToolEffect::CEffectMeshObject::EndPlay()
{
	if (FAILED(CEffectBase::EndPlay())) { return E_FAIL; }

	return S_OK;
}

void ToolEffect::CEffectMeshObject::Release()
{
	CEffectBase::Release();
}

uint32 ToolEffect::CEffectMeshObject::GetCurKeyFrameIndex()
{
	return m_spEffectMesh->GetCurKeyFrameIndex();
}

std::vector<FEffectMeshKeyFrame>& ToolEffect::CEffectMeshObject::GetKeyFrames()
{
	return m_spEffectMesh->GetKeyFrames();
}