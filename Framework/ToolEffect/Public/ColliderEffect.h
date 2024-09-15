#pragma once
#include "EffectGroup.h"

namespace Engine
{
	class CSphereComponent;
	class CBoxComponent;
}

namespace ToolEffect
{
	struct FColliderEffectDesc : public CEffectGroup::FEffectGroupDesc
	{
		EColliderType m_eColliderType = EColliderType::ENUM_END;

		ECollisionFlag m_eCollisionFlag = ECollisionFlag::ENUM_END;

		ECollisionLayer m_eCollisionLayer = ECollisionLayer::ENUM_END;

		// Sphere
		float m_fRadius = 0.f;

		// Box
		Vector3 m_vExtents = Vector3(0.f);
		Vector3 m_vLocalPosition = Vector3(0.f);
		Vector3 m_vLocalRotate = Vector3(0.f);
	};

	class CColliderEffect final : public CEffectGroup
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CColliderEffect() = default;
		virtual ~CColliderEffect() = default;
		
	/********************
		Events
	********************/
	public:
		static std::shared_ptr<CColliderEffect> Create(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc);
		static std::shared_ptr<CColliderEffect> CreateFromJson(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc, const std::wstring& _wstrJsonFileName);

	public:
		virtual HRESULT Initialize(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc);
		HRESULT InitializeFromJson(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc, const std::wstring& _wstrJsonFileName);

		virtual HRESULT BeginPlay() override;

		virtual int32 PreTick(float _fDeltaSeconds) override;

		virtual int32 Tick(float _fDeltaSeconds) override;

		virtual int32 PostTick(float _fDeltaSeconds) override;

		virtual HRESULT EndPlay() override;

		virtual void Release() override;
		
	public:
		HRESULT InitializeColliderComponent(const shared_ptr<FGameObjectDesc>& _spGameObjectDesc);
	/********************
		Methods
	********************/
	public:


	/********************
		Getter / Setter
	********************/
	public:


	/********************
		Data Members
	********************/
	private:
		std::shared_ptr<CColliderComponent> m_spColliderComponent = nullptr;
	};
}