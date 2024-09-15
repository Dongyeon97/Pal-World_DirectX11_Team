#pragma once
#include "Window.h"

namespace ToolEffect
{
	class CToolEffectLevel;
	class CObjectList;
	class CEffectGroupWindow;

	class CInspector final : public CWindow
	{
	/********************
		Ctor/Dtor
	********************/
	public:
		explicit CInspector() = default;
		virtual ~CInspector() = default;

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
	public:
		void RemoveKeyFrame(EEffectType _eEffectType, int32 _iIndex);
		void SetParticleKeyFrameValue(const FParticleKeyFrame& _tParticleKeyFrame, int32 _iIndex);
		void SetEffectMeshKeyFrameValue(const FEffectMeshKeyFrame& _tEffectMeshKeyFrame, int32 _iIndex);
		void SetInstanceEffectMeshKeyFrameValue(const FEffectMeshKeyFrame& _tEffectMeshKeyFrame, int32 _iIndex);

	private:
		void EffectTab();
			void ParticleTab();
				void ParticleInfo();			 // ParticleInfo
				void UpdateCurParticleInfo();	 // 현재 선택된 파티클의 ParticleInfo 갱신
				void CreateParticle();			 // 파티클 객체 생성
				void CreateParticleFromJson();	 // 파티클 객체 생성 from JSON
				void SaveParticleToJSON();		 // 파티클 객체 저장 to JSON
												 
			void EffectMeshTab();				 
				void EffectMeshInfo();			 // EffectMeshInfo
				void UpdateCurEffectMeshInfo();	 // 현재 선택된 이펙트 메시의 EffectMeshInfo 갱신
				void CreateEffectMesh();		 // 이펙트 메시 객체 생성
				void CreateEffectMeshFromJson(); // 이펙트 메시 객체 생성 from JSON
				void SaveEffectMeshToJSON(const FEffectMeshInfo& _tEffectMeshInfo, const string& _strObjectName);	 // 이펙트 메시 객체 저장 to JSON

			void InstanceEffectMeshTab();
				void InstanceEffectMeshsEffectMeshInfo();
				void InstanceEffectMeshInfo();		 // InstanceEffectMeshInfo
				void UpdateCurInstanceEffectMeshInfo();
				void CreateInstanceEffectMesh();
				void CreateInstanceEffectMeshFromJson();
				void SaveInstanceEffectMeshToJSON(const FEffectMeshInfo& _tEffectMeshInfo, const FInstanceEffectMeshInfo& _tInstanceEffectMeshInfo, const string& _strObjectName);
				
		void SelectEffectMesh(EEffectType _eEffectType);
		void SelectEffectShader(EEffectType _eEffectType);
		void SelectDiffuseTexture(EEffectType _eEffectType);
		void SelectNoiseTexture(EEffectType _eEffectType);
		void SelectMaskTexture(EEffectType _eEffectType);
		void SelectDistortionTexture(EEffectType _eEffectType);
		void SelectDissolveTexture(EEffectType _eEffectType);

	/********************
		Getter/Setter
	********************/
	public:
	#pragma region Default
		void SetCurLevel(std::shared_ptr<CToolEffectLevel> _spLevel) { m_wpToolEffectLevel = _spLevel; }
		void SetObjectList(std::shared_ptr<CObjectList> _spObjectList) { m_wpObjectList = _spObjectList; }
		void SetEffectGroupWindow(std::shared_ptr<CEffectGroupWindow> _spEffectGroupWindow) { m_wpEffectGroupWindow = _spEffectGroupWindow; }
	#pragma endregion
		/* ParticleInfo */
		void SetParticleInfo(FParticleInfo _tParticleInfo) { m_tParticleInfo = _tParticleInfo; }
		FParticleInfo& GetParticleInfo() { return m_tParticleInfo; }

		/* EffectMeshInfo */
		void SetEffectMeshInfo(FEffectMeshInfo _tEffectMeshInfo) { m_tEffectMeshInfo = _tEffectMeshInfo; }
		FEffectMeshInfo& GetEffectMeshInfo() { return m_tEffectMeshInfo; }

		/* InstanceEffectMeshInfo */
		void SetInstanceEffectMeshEffectMeshInfo(FEffectMeshInfo _tInstanceEffectMeshEffectMeshInfo) { m_tInstanceEffectMeshEffectMeshInfo = _tInstanceEffectMeshEffectMeshInfo; }
		FEffectMeshInfo& GetInstanceEffectMeshEffectMeshInfo() { return m_tInstanceEffectMeshEffectMeshInfo; }

		void SetInstanceEffectMeshInfo(FInstanceEffectMeshInfo _tInstanceEffectMeshInfo) { m_tInstanceEffectMeshInfo = _tInstanceEffectMeshInfo; }
		FInstanceEffectMeshInfo& GetInstanceEffectMeshInfo() { return m_tInstanceEffectMeshInfo; }

		void SetParticleRenderType(ERenderType _eRenderType) { m_iParticleRenderType = (int32)_eRenderType; }
		void SetParticleType(EParticleType _eParticleType) { m_iParticleType = (int32)_eParticleType; }
		void SetEffectMeshRenderType(ERenderType _eRenderType) { m_iEffectMeshRenderType = (int32)_eRenderType; }
		void SetInstanceEffectMeshRenderType(ERenderType _eRenderType) { m_iInstanceEffectMeshRenderType = (int32)_eRenderType; }

	/********************
		Data Members
	********************/
	private:
	#pragma region Default
		bool m_bInspector = true;

		std::weak_ptr<CObjectList> m_wpObjectList;
		std::weak_ptr<CEffectGroupWindow> m_wpEffectGroupWindow;
	#pragma endregion

	private:
	#pragma region 아이템 셀렉터
		uint16 m_iEffectShaderSelector = 0;

		uint16 m_iDiffuseTextureSelector = 0;
		uint16 m_iNoiseTextureSelector = 0;
		uint16 m_iMaskTextureSelector = 0;
		uint16 m_iDistortionTextureSelector = 0;
		uint16 m_iDissolveTextureSelector = 0;
	#pragma endregion

	private:
	#pragma region 파티클
		/* 파티클 정보 */
		FParticleInfo m_tParticleInfo;

		/* 타입 */
		int32 m_iParticleRenderType = (int32)ERenderType::RENDER_BLEND;
		int32 m_iParticleEffectType = (int32)EEffectType::PARTICLE;
		int32 m_iParticleType = (int32)EParticleType::SPREAD;

		/* 객체 이름 */
		string m_strParticleObjectName = "";
		char m_chParticleObjectName[MAX_PATH];

		/* 셰이더 이름 */
		char m_chParticleShaderName[MAX_PATH];

		/* 디퓨즈 */
		char m_chParticleDiffuseName[MAX_PATH];

		/* 제이슨 파일 */
		char m_chParticleJsonFileName[MAX_PATH];
	#pragma endregion

	private:
	#pragma region 이펙트 메시
		/* 이펙트 메시 정보 */
		FEffectMeshInfo m_tEffectMeshInfo;

		/* 렌더 타입 */
		int32 m_iEffectMeshRenderType = (int32)ERenderType::RENDER_BLEND;

		/* 이펙트 타입 */
		int32 m_iEffectMeshEffectType = (int)EEffectType::EFFECTMESH;

		/* 객체 이름 */
		string m_strEffectMeshObjectName = "";
		char m_chEffectMeshObjectName[MAX_PATH];

		/* 셰이더 이름 */
		char m_chEffectMeshShaderName[MAX_PATH];

		/* 디퓨즈 */
		char m_chEffectMeshDiffuseName[MAX_PATH];

		/* 노이즈 */
		char m_chEffectMeshNoiseName[MAX_PATH];

		/* 마스크 */
		char m_chEffectMeshMaskName[MAX_PATH];

		/* 디스토션 */
		char m_chEffectMeshDistortionName[MAX_PATH];

		/* 디졸브 */
		char m_chEffectMeshDissolveName[MAX_PATH];

		/* 제이슨 파일 */
		char m_chEffectMeshJsonFileName[MAX_PATH];
	#pragma endregion

	private:
	#pragma region 인스턴스 메시
		/* 인스턴스 메시 - 이펙트메시 Info */
		FEffectMeshInfo m_tInstanceEffectMeshEffectMeshInfo;

		/* 인스턴스 메시 Info */
		FInstanceEffectMeshInfo m_tInstanceEffectMeshInfo;

		/* 인스턴스 메시 파티클 타입 */
		int32 m_iInstanceEffectMeshParticleType = (int32)EParticleType::SPREAD;

		/* 렌더 타입 */
		int32 m_iInstanceEffectMeshRenderType = (int32)ERenderType::RENDER_BLEND;

		/* 이펙트 타입 */
		int32 m_iInstanceEffectMeshEffectType = (int)EEffectType::InstanceEffectMesh;

		/* 객체 이름 */
		string m_strInstanceEffectMeshObjectName = "";
		char m_chInstanceEffectMeshObjectName[MAX_PATH];

		/* 셰이더 이름 */
		char m_chInstanceEffectMeshShaderName[MAX_PATH];

		/* 디퓨즈 */
		char m_chInstanceEffectMeshDiffuseName[MAX_PATH];

		/* 노이즈 */
		char m_chInstanceEffectMeshNoiseName[MAX_PATH];

		/* 마스크 */
		char m_chInstanceEffectMeshMaskName[MAX_PATH];

		/* 디스토션 */
		char m_chInstanceEffectMeshDistortionName[MAX_PATH];

		/* 디졸브 */
		char m_chInstanceEffectMeshDissolveName[MAX_PATH];

		/* 제이슨 파일 */
		char m_chInstanceEffectMeshJsonFileName[MAX_PATH];
	#pragma endregion
	};
}
