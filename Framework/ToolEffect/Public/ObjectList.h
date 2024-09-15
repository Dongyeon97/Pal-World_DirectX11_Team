#pragma once
#include "Window.h"

namespace Engine
{
	class CGameObject;
}

namespace ToolEffect
{
	class CToolEffectLevel;
	class CInspector;
	class CEffectGroupWindow;

	class CEffectBase;
	class CParticleObject;
	class CEffectMeshObject;
	class CInstanceEffectMeshObject;

	class CObjectList final : public CWindow
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CObjectList() = default;
		virtual ~CObjectList() = default;

	/********************
		Events
	********************/
	public:
		virtual HRESULT Initialize();
		virtual HRESULT BeginPlay();
		virtual int32 PreTick(float _fDeltaSeconds);
		virtual int32 Tick(float _fDeltaSeconds);
		virtual int32 PostTick(float _fDeltaSeconds);
		virtual int32 Render();
		virtual int32 EndPlay();
		virtual void Release();

	/********************
		Methods
	********************/
	private:
		HRESULT LoadParticlesFromPath();
		HRESULT LoadEffectMeshsFromPath();
		HRESULT LoadInstanceEffectMeshsFromPath();

	private:
		void UpdateObjectList();
			void UpdateParticleTab();
				void UpdateParticleList();
				void UpdateSelectedParticleKeyFrameList();
				void UpdateParticleKeyFrames();
				void AddParticleKeyFrame();
				void RemoveParticle();
				void RemoveParticleKeyFrame();
				void UpdateCurParticleKeyFrame();

		void UpdateEffectMeshTab();
			void UpdateEffectMeshList();
				void UpdateSelectedEffectMeshKeyFrameList();
				void UpdateEffectMeshKeyFrames();
				void AddEffectMeshKeyFrame();
				void RemoveEffectMesh();
				void RemoveEffectMeshKeyFrame();
				void UpdateCurEffectMeshKeyFrame();

		void UpdateInstanceEffectMeshTab();
			void UpdateInstanceEffectMeshList();
				void UpdateSelectedInstanceEffectMeshKeyFrameList();
				void UpdateInstanceEffectMeshKeyFrames();
				void AddInstanceEffectMeshKeyFrame();
				void RemoveInstanceEffectMesh();
				void RemoveInstanceEffectMeshKeyFrame();
				void UpdateCurInstanceEffectMeshKeyFrame();

		void UpdateObjectSelector(const unordered_map<wstring, shared_ptr<CGameObject>>& _umapObjects);
		void UpdateEasingTypeSelector(int32* _pSelector, EEasingType* _eEasingType, int32 iIndex);
		void PauseEffect();
		void AddEffectToGroup();
		void NonActive();
		void Reset();

	/********************
		Getter/Setter
	********************/
	public:
	#pragma region Default
		void SetCurLevel(std::shared_ptr<CToolEffectLevel> _spLevel) { m_wpToolEffectLevel = _spLevel; }
		void SetInspector(std::shared_ptr<CInspector> _spInspector) { m_wpInspector = _spInspector; }
		void SetEffectGroupWindow(std::shared_ptr<CEffectGroupWindow> _spEffectGroupWindow) { m_wpEffectGroupWindow = _spEffectGroupWindow; }
		void SetCurEffectType(EEffectType _eEffectType) { m_eCurEffectType = _eEffectType; }
		void SetCurObject(std::shared_ptr<CEffectBase> _spEffectBase) { m_wpSelectedObject = _spEffectBase; }
	#pragma endregion

	public:
		std::shared_ptr<CEffectBase> GetEffectBase();
		std::shared_ptr<CParticleObject> GetParticleObject();
		std::shared_ptr<CEffectMeshObject> GetEffectMeshObject();
		std::shared_ptr<CInstanceEffectMeshObject> GetInstanceEffectMeshObject();

	/********************
		Data Members
	********************/
	private:
		std::wstring m_wstrParticlePath = L"../../Resource/Effects/Particle/";
		std::wstring m_wstrEffectMeshPath = L"../../Resource/Effects/EffectMesh/";
		std::wstring m_wstrInstanceEffectMeshPath = L"../../Resource/Effects/InstanceEffectMesh/";

	private:
	#pragma region Default 
		bool m_bObjectList = false;

		EEffectType m_eCurEffectType = EEffectType::PARTICLE;

		std::weak_ptr<CInspector> m_wpInspector;
		std::weak_ptr<CEffectGroupWindow> m_wpEffectGroupWindow;
	#pragma endregion

	private:
	#pragma region 셀렉터
		/* 리스트에서 선택된 오브젝트 */
		std::weak_ptr<CEffectBase> m_wpSelectedObject;

		uint16 m_iObjectSelector = 0;
		uint16 m_iParticleKeyFrameSelector = 0;
		uint16 m_iEffectMeshKeyFrameSelector = 0;
		uint16 m_iInstanceEffectMeshKeyFrameSelector = 0;
	#pragma endregion

	private:
	#pragma region 키프레임
		uint16 m_iSelectedKeyFrameIndex = 0;

	private:
		FParticleKeyFrame m_tParticleKeyFrame;

		int32 m_iParticleColor1EasingType			= (int32)EEasingType::EASE_LINEAR;
		int32 m_iParticleColor2EasingType			= (int32)EEasingType::EASE_LINEAR;
		int32 m_iParticleColor3EasingType			= (int32)EEasingType::EASE_LINEAR;
		int32 m_iParticleSizeEasingType				= (int32)EEasingType::EASE_LINEAR;
		int32 m_iParticleSpeedEasingType			= (int32)EEasingType::EASE_LINEAR;

		FEffectMeshKeyFrame m_tEffectMeshKeyFrame;

		int32 m_iEffectMeshColor1EasingType			= (int32)EEasingType::EASE_LINEAR;
		int32 m_iEffectMeshColor2EasingType			= (int32)EEasingType::EASE_LINEAR;
		int32 m_iEffectMeshColor3EasingType			= (int32)EEasingType::EASE_LINEAR;
		int32 m_iEffectMeshScaleEasingType			= (int32)EEasingType::EASE_LINEAR;
		int32 m_iEffectMeshRotationEasingType		= (int32)EEasingType::EASE_LINEAR;
		int32 m_iEffectMeshTranslationEasingType	= (int32)EEasingType::EASE_LINEAR;

		FEffectMeshKeyFrame m_tInstanceEffectMeshKeyFrame;

		int32 m_iInstanceEffectMeshColor1EasingType		= (int32)EEasingType::EASE_LINEAR;
		int32 m_iInstanceEffectMeshColor2EasingType		= (int32)EEasingType::EASE_LINEAR;
		int32 m_iInstanceEffectMeshColor3EasingType		= (int32)EEasingType::EASE_LINEAR;
		int32 m_iInstanceEffectMeshScaleEasingType		= (int32)EEasingType::EASE_LINEAR;
		int32 m_iInstanceEffectMeshRotationEasingType		= (int32)EEasingType::EASE_LINEAR;
		int32 m_iInstanceEffectMeshTranslationEasingType	= (int32)EEasingType::EASE_LINEAR;
	#pragma endregion
	};
}

