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
				void UpdateCurParticleInfo();	 // ���� ���õ� ��ƼŬ�� ParticleInfo ����
				void CreateParticle();			 // ��ƼŬ ��ü ����
				void CreateParticleFromJson();	 // ��ƼŬ ��ü ���� from JSON
				void SaveParticleToJSON();		 // ��ƼŬ ��ü ���� to JSON
												 
			void EffectMeshTab();				 
				void EffectMeshInfo();			 // EffectMeshInfo
				void UpdateCurEffectMeshInfo();	 // ���� ���õ� ����Ʈ �޽��� EffectMeshInfo ����
				void CreateEffectMesh();		 // ����Ʈ �޽� ��ü ����
				void CreateEffectMeshFromJson(); // ����Ʈ �޽� ��ü ���� from JSON
				void SaveEffectMeshToJSON(const FEffectMeshInfo& _tEffectMeshInfo, const string& _strObjectName);	 // ����Ʈ �޽� ��ü ���� to JSON

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
	#pragma region ������ ������
		uint16 m_iEffectShaderSelector = 0;

		uint16 m_iDiffuseTextureSelector = 0;
		uint16 m_iNoiseTextureSelector = 0;
		uint16 m_iMaskTextureSelector = 0;
		uint16 m_iDistortionTextureSelector = 0;
		uint16 m_iDissolveTextureSelector = 0;
	#pragma endregion

	private:
	#pragma region ��ƼŬ
		/* ��ƼŬ ���� */
		FParticleInfo m_tParticleInfo;

		/* Ÿ�� */
		int32 m_iParticleRenderType = (int32)ERenderType::RENDER_BLEND;
		int32 m_iParticleEffectType = (int32)EEffectType::PARTICLE;
		int32 m_iParticleType = (int32)EParticleType::SPREAD;

		/* ��ü �̸� */
		string m_strParticleObjectName = "";
		char m_chParticleObjectName[MAX_PATH];

		/* ���̴� �̸� */
		char m_chParticleShaderName[MAX_PATH];

		/* ��ǻ�� */
		char m_chParticleDiffuseName[MAX_PATH];

		/* ���̽� ���� */
		char m_chParticleJsonFileName[MAX_PATH];
	#pragma endregion

	private:
	#pragma region ����Ʈ �޽�
		/* ����Ʈ �޽� ���� */
		FEffectMeshInfo m_tEffectMeshInfo;

		/* ���� Ÿ�� */
		int32 m_iEffectMeshRenderType = (int32)ERenderType::RENDER_BLEND;

		/* ����Ʈ Ÿ�� */
		int32 m_iEffectMeshEffectType = (int)EEffectType::EFFECTMESH;

		/* ��ü �̸� */
		string m_strEffectMeshObjectName = "";
		char m_chEffectMeshObjectName[MAX_PATH];

		/* ���̴� �̸� */
		char m_chEffectMeshShaderName[MAX_PATH];

		/* ��ǻ�� */
		char m_chEffectMeshDiffuseName[MAX_PATH];

		/* ������ */
		char m_chEffectMeshNoiseName[MAX_PATH];

		/* ����ũ */
		char m_chEffectMeshMaskName[MAX_PATH];

		/* ����� */
		char m_chEffectMeshDistortionName[MAX_PATH];

		/* ������ */
		char m_chEffectMeshDissolveName[MAX_PATH];

		/* ���̽� ���� */
		char m_chEffectMeshJsonFileName[MAX_PATH];
	#pragma endregion

	private:
	#pragma region �ν��Ͻ� �޽�
		/* �ν��Ͻ� �޽� - ����Ʈ�޽� Info */
		FEffectMeshInfo m_tInstanceEffectMeshEffectMeshInfo;

		/* �ν��Ͻ� �޽� Info */
		FInstanceEffectMeshInfo m_tInstanceEffectMeshInfo;

		/* �ν��Ͻ� �޽� ��ƼŬ Ÿ�� */
		int32 m_iInstanceEffectMeshParticleType = (int32)EParticleType::SPREAD;

		/* ���� Ÿ�� */
		int32 m_iInstanceEffectMeshRenderType = (int32)ERenderType::RENDER_BLEND;

		/* ����Ʈ Ÿ�� */
		int32 m_iInstanceEffectMeshEffectType = (int)EEffectType::InstanceEffectMesh;

		/* ��ü �̸� */
		string m_strInstanceEffectMeshObjectName = "";
		char m_chInstanceEffectMeshObjectName[MAX_PATH];

		/* ���̴� �̸� */
		char m_chInstanceEffectMeshShaderName[MAX_PATH];

		/* ��ǻ�� */
		char m_chInstanceEffectMeshDiffuseName[MAX_PATH];

		/* ������ */
		char m_chInstanceEffectMeshNoiseName[MAX_PATH];

		/* ����ũ */
		char m_chInstanceEffectMeshMaskName[MAX_PATH];

		/* ����� */
		char m_chInstanceEffectMeshDistortionName[MAX_PATH];

		/* ������ */
		char m_chInstanceEffectMeshDissolveName[MAX_PATH];

		/* ���̽� ���� */
		char m_chInstanceEffectMeshJsonFileName[MAX_PATH];
	#pragma endregion
	};
}
