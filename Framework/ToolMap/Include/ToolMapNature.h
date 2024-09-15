#pragma once

#include "GameObject.h"

namespace Engine
{
	class CMeshComponent;
	class CCameraComponent;
	class CInputManager;
	class CPickManager;
	class CModel;
	class CMeshBuffer;
}

namespace ToolMap
{
	class CToolMapNature : public CGameObject
	{
	public:
		struct FToolMapNatureDesc : public  FGameObjectDesc
		{
			int m_iIndex = 0;

			wstring m_wstrModelFilePath = L"";

			//// 초기 크기, 기본 크기는 (1, 1, 1)
			//Vector3 m_vInitScale = Vector3(1.f, 1.f, 1.f);

			//// 초기 회전, 기본 회전은 0도
			//Vector3 m_vInitRotate = Vector3(0.f, 0.f, 0.f);

			//// 초기 위치, 기본 위치는 (0, 0, 0)
			//Vector3 m_vInitPosition = Vector3(0.f, 0.f, 0.f);

			//// 게임 오브젝트 이름
			//std::wstring m_wstrGameObjectName = L"";

			//// 게임 오브젝트 활성화 여부
			//bool m_bActive = true;
		};
		/********************
			Ctor/Dtor
		********************/
	public:
		explicit CToolMapNature() = default;

		virtual ~CToolMapNature() = default;

		/********************
			Events
		********************/
	public:
		static std::shared_ptr<CToolMapNature> Create(const std::shared_ptr<FToolMapNatureDesc>& _spObjectDesc);

		virtual HRESULT Initialize(const std::shared_ptr<FToolMapNatureDesc>& _spObjectDesc);

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
		//void SetSpawnPosition(const Vector3& _vSpawnPosition) { m_vSpawnPosition = _vSpawnPosition; }

		//const Vector3& GetSpawnPosition() const { return m_vSpawnPosition; }

		void SetSpawnYDegree(float _fYDegree) { m_fSpawnYDegree = _fYDegree; }

		float GetSpawnYDegree() const { return m_fSpawnYDegree; }
		
		Matrix GetMatrix() const { return m_mMatrix; }

	protected:
		//Vector3 m_vSpawnPosition = Vector3::Zero;

		int32 m_iSpawnCellIndex = -1;

		float m_fSpawnYDegree = 0.0f;

		int32 m_iModelIndex = 0;

		Matrix m_mMatrix;

		bool m_bSelect = false;

	private:
		shared_ptr<CMeshComponent> m_spMeshComponent = nullptr;
		shared_ptr<CCameraComponent> m_spCameraComponent = nullptr;
		shared_ptr<CInputManager> m_spInputManager = nullptr;
		shared_ptr<CPickManager> m_spPickManager = nullptr;
		wstring m_wstrModelFilePath = L"";
	};
}
