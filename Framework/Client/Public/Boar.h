#pragma once
#include "Monster.h"

namespace Engine
{
    class CSphereComponent;
    class CColliderComponent;
}

namespace Client
{
	/********************
		Descriptor
	********************/
	struct FBoarDesc : public FMonsterDesc
	{
	};

    class CBoar final : public CMonster
    {
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CBoar() = default;

		virtual ~CBoar() = default;
		
	/********************
		Events
	********************/
	public:
		static std::shared_ptr<CBoar> Create(const std::shared_ptr<FBoarDesc>& _spBoarDesc);

		virtual HRESULT Initialize(const std::shared_ptr<FGameObjectDesc>& _spGameObjectDesc) override;

	public:
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
		void InitBoar(const std::shared_ptr<FBoarDesc>& _spBoarDesc);

		void InitMeshComponent();

		void InitColliderComponent();

		void InitBehaviorTreeComponent(const std::shared_ptr<FBoarDesc>& _spBoarDesc);

	private:
		void InitializeWildPalBT(const std::shared_ptr<FBoarDesc> _spBoarDesc, bool _bInit = true);

		void InitializePartnerPalBT(const std::shared_ptr<FBoarDesc> _spBoarDesc, bool _bInit = true);

		void InitializeSlavePalBT(const std::shared_ptr<FBoarDesc> _spBoarDesc, bool _bInit = true);

		virtual HRESULT ReInitializePalBT(EMonsterAIType _eMonsterAIType) override;

	public:
		virtual void LookAt(const Vector3& _vPosition) override;

		virtual void Turn(const Vector3& _vAxis, float _fAngularSpeed) override;

		virtual void MoveForward(float _fSpeed) override;

		virtual void MoveBackward(float _fSpeed) override;

		virtual void MoveRight(float _fSpeed) override;

		virtual void MoveLeft(float _fSpeed) override;

		virtual void MoveUp(float _fSpeed) override;

		virtual void MoveDown(float _fSpeed) override;

		virtual void SetPosition(const Vector3& _vPosition) override;

		virtual const std::optional<Vector3> GetPosition() const override;

		virtual void ChangeMouth(EPalMouthType _ePalMouthType) override;

	public:
		virtual void UpdateBlackboardValues() override;

		virtual void OnTriggerEnter(CComponent* _pSrcComponent, CComponent* _pDstComponent) override;

		virtual void OnCollisionEnter(CComponent* _pSrcComponent, CComponent* _pDstComponent) override;
		
	/********************
		Getter/Setter
	********************/
	public:
		virtual void SetAllyCollisionLayer() override;

		virtual std::shared_ptr<CColliderComponent> GetBodyColliderComponent() override;

		virtual void SetLinearVelocity(const PxVec3& _vLinearVelocity) override;

		virtual void SetAngularVelocity(const PxVec3& _vLinearVelocity) override;

	/********************
		Data Members
	********************/
	private:
		std::shared_ptr<CSphereComponent> m_spBodySphereComponent = nullptr;

		std::shared_ptr<CSphereComponent> m_spAttackSphereComponent = nullptr;
    };
}
