#include "pch.h"
#include "ColliderEffect.h"

#include "SphereComponent.h"
#include "BoxComponent.h"

shared_ptr<CColliderEffect> ToolEffect::CColliderEffect::Create(const shared_ptr<FGameObjectDesc>& _spGameObjectDesc)
{
	shared_ptr<CColliderEffect> spInstance = make_shared<CColliderEffect>();
	CHECKF(spInstance, L"Failed to Create : CColliderEffect");

	spInstance->Initialize(_spGameObjectDesc);

	return spInstance;
}
shared_ptr<CColliderEffect> ToolEffect::CColliderEffect::CreateFromJson(const shared_ptr<FGameObjectDesc>& _spGameObjectDesc, const wstring& _wstrJsonFileName)
{
	shared_ptr<CColliderEffect> spInstance = make_shared<CColliderEffect>();
	CHECKF(spInstance, L"Failed to CreateFromJson : CColliderEffect");

	spInstance->InitializeFromJson(_spGameObjectDesc, _wstrJsonFileName);

	return spInstance;
}

HRESULT ToolEffect::CColliderEffect::Initialize(const shared_ptr<FGameObjectDesc>& _spGameObjectDesc)
{
	if (FAILED(CEffectGroup::Initialize(_spGameObjectDesc))) { return E_FAIL; }

	InitializeColliderComponent(_spGameObjectDesc);

	return S_OK;
}

HRESULT ToolEffect::CColliderEffect::InitializeFromJson(const shared_ptr<FGameObjectDesc>& _spGameObjectDesc, const wstring& _wstrJsonFileName)
{
	if (FAILED(CEffectGroup::InitializeFromJson(_spGameObjectDesc, _wstrJsonFileName))) { return E_FAIL; }

	InitializeColliderComponent(_spGameObjectDesc);

	return S_OK;
}

HRESULT ToolEffect::CColliderEffect::BeginPlay()
{
	if (FAILED(CEffectGroup::BeginPlay())) { return E_FAIL; }

	return S_OK;
}

int32 ToolEffect::CColliderEffect::PreTick(float _fDeltaSeconds)
{
	switch (CEffectGroup::PreTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

int32 ToolEffect::CColliderEffect::Tick(float _fDeltaSeconds)
{
	switch (CEffectGroup::Tick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

int32 ToolEffect::CColliderEffect::PostTick(float _fDeltaSeconds)
{
	switch (CEffectGroup::PostTick(_fDeltaSeconds))
	{
	case -1:	return -1;
	default:	break;
	}

	return 0;
}

HRESULT ToolEffect::CColliderEffect::EndPlay()
{
	if (FAILED(CEffectGroup::EndPlay())) { return E_FAIL; }

	return S_OK;
}

void ToolEffect::CColliderEffect::Release()
{
	CEffectGroup::Release();
}

HRESULT ToolEffect::CColliderEffect::InitializeColliderComponent(const shared_ptr<FGameObjectDesc>& _spGameObjectDesc)
{
	shared_ptr<FColliderEffectDesc> spColliderEffectDesc = static_pointer_cast<FColliderEffectDesc>(_spGameObjectDesc);

	switch (spColliderEffectDesc->m_eColliderType)
	{
	case EColliderType::SPHERE:
	{
		FSphereComponentDesc tSphereDesc;

		// sphere component desc
		tSphereDesc.m_fRadius = spColliderEffectDesc->m_fRadius;

		// collider desc
		tSphereDesc.m_bGravity			= false;
		tSphereDesc.m_bTrigger			= true;
		tSphereDesc.m_eMobilityType		= EMobilityType::DYNAMIC;
		tSphereDesc.m_eComponentType	= EComponentType::SPHERE;
		tSphereDesc.m_eCollisionFlag	= spColliderEffectDesc->m_eCollisionFlag;
		tSphereDesc.m_eCollisionLayer	= spColliderEffectDesc->m_eCollisionLayer;
		tSphereDesc.m_fMass				= 0.f;
		tSphereDesc.m_vMaterial			= Vector3(0.f, 0.f, 0.f);

		// component desc
		tSphereDesc.m_wpOwnerGameObject = shared_from_this();
		tSphereDesc.m_eColliderType		= EColliderType::SPHERE;
		tSphereDesc.m_wstrComponentName = L"SphereCollider";
		tSphereDesc.m_eRenderType		= ERenderType::RENDER_DEBUG;

		m_spColliderComponent = CSphereComponent::Create(make_shared<FSphereComponentDesc>(tSphereDesc));
		AddComponent(L"SphereCollider", m_spColliderComponent);

		break;
	}
	case EColliderType::BOX:
	{
		FBoxComponentDesc tBoxComponentDesc;

		// box component desc
		tBoxComponentDesc.m_vExtents			= spColliderEffectDesc->m_vExtents;
		tBoxComponentDesc.m_vLocalPosition		= spColliderEffectDesc->m_vLocalPosition;
		tBoxComponentDesc.m_vLocalRotate		= spColliderEffectDesc->m_vLocalRotate;

		// collider component desc
		tBoxComponentDesc.m_bGravity			= false;
		tBoxComponentDesc.m_bTrigger			= true;
		tBoxComponentDesc.m_eMobilityType		= EMobilityType::DYNAMIC;
		tBoxComponentDesc.m_eColliderType		= EColliderType::BOX;
		tBoxComponentDesc.m_eCollisionFlag		= spColliderEffectDesc->m_eCollisionFlag;
		tBoxComponentDesc.m_eCollisionLayer		= spColliderEffectDesc->m_eCollisionLayer;
		tBoxComponentDesc.m_fDensity			= 1.0f;
		tBoxComponentDesc.m_fMass				= 0.0f;
		tBoxComponentDesc.m_iAxisLockType		= 0;
		tBoxComponentDesc.m_vMaterial			= Vector3(0.5f, 0.5f, 0.5f);

		// component desc
		tBoxComponentDesc.m_wpOwnerGameObject	= shared_from_this();
		tBoxComponentDesc.m_eComponentType		= EComponentType::BOX;
		tBoxComponentDesc.m_wstrComponentName	= L"BoxComponent";
		tBoxComponentDesc.m_eRenderType			= ERenderType::RENDER_DEBUG;

		m_spColliderComponent = CBoxComponent::Create(make_shared<FBoxComponentDesc>(tBoxComponentDesc));
		AddComponent(L"BoxComponent", m_spColliderComponent);

		break;
	}
	}

	return S_OK;
}
