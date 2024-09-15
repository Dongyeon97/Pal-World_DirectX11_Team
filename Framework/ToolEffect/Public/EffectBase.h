#pragma once
#include "GameObject.h"

namespace ToolEffect
{
	class CEffectGroup;

	class CEffectBase abstract : public CGameObject
	{
	/********************
		Descriptor
	********************/
	public:
		struct FEffectBaseDesc : FGameObjectDesc
		{
			EEffectType m_eEffectType = EEffectType::ENUM_END;
		};

	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CEffectBase() = default;
		virtual ~CEffectBase() = default;
		
	/********************
		Events
	********************/
	public:
		virtual HRESULT Initialize(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc);

		virtual HRESULT BeginPlay() override;

		virtual int32 PreTick(float _fDeltaSeconds) override;

		virtual int32 Tick(float _fDeltaSeconds) override;

		virtual int32 PostTick(float _fDeltaSeconds) override;

		virtual HRESULT EndPlay() override;

		virtual void Release() override;
		
	/********************
		Methods
	********************/
	public:
		

	/********************
		Getter / Setter
	********************/
	public:
		void SetOwnerEffectGroup(std::shared_ptr<CEffectGroup> _spEffectGroup) { m_wpEffectGroup = _spEffectGroup; }

		EEffectType GetEffectType() { return m_eEffectType; }

		const Matrix& GetOriginMatrix() { return m_matOrigin; }

	/********************
		Data Members
	********************/
	protected:
		Matrix m_matOrigin = XMMatrixIdentity();

		EEffectType m_eEffectType = EEffectType::ENUM_END;

		std::weak_ptr<CEffectGroup> m_wpEffectGroup;
	};
}
