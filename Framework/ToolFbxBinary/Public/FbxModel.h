#pragma once

namespace FbxBinary
{
	class CMeshBuffer;
	class CBone;
	class CMaterial;
	class CAnimation;

	class CModel final : public enable_shared_from_this<CModel>
	{
	public:
		struct ObjectInfo
		{
			wstring wstrObjectName = L"";	// ������Ʈ �̸�
			vector<wstring> vecMeshNames;	// �� ������Ʈ�� ����ϴ� �޽� �̸�
			Matrix matWorld;				// �� ������Ʈ�� ���� ���
		};
		struct ColliderInfo
		{
			EColliderType eColliderType = EColliderType::ENUM_END;	// Ÿ��
			wstring wstrColliderName = L"";				// �̸�
			wstring wstrParentBoneName = L"";			// �θ� �� �̸�
			Vector3 vCenter = Vector3::Zero;			// ���� - ��� ��ǥ
			Vector3 vRotationYPR = Vector3::Zero;		// ȸ�� - YawPitchRoll ������ XYZ�� ����.
			float fRadius = 0;							// ������ - Sphere, Sphy
			float fLength = 0;							// ���� - Sphy
		};
		struct MaterialInfo
		{
			wstring wstrMaterialName = L"";														// �� ���׸����� �̸� - ����
			wstring wstrShaderTag = L"";                                                        // �� ���׸����� ����ϴ� ���̴� Ÿ�� - ����
			unordered_map<wstring, wstring> umapTextures;										// �ؽ�ó �뵵, �ؽ�ó ��� - ����
			unordered_map<wstring, float> umapScalars;											// ��Į�� �뵵, ��Į�� ��
		};

	public:
		CModel() = default;
		virtual ~CModel() = default;

	public:
		static shared_ptr<CModel> Create(EModelType _eModelType, const wstring _wstrModelFilePath);
		HRESULT	Initialize(EModelType _eModelType, const wstring _wstrModelFilePath);
		void SaveToDat();

	public:
		const wstring& GetModelPath() { return m_wstrFilePath; }
		const wstring& GetModelName() { return m_wstrModelName; }
		const EModelType& GetModelType() { return m_eModelType; }
		int32 GetBoneIndex(const wstring& _wstrBoneName);
		const vector<shared_ptr<CBone>>& GetBones() { return m_vecBones; }
		int32 GetNumMeshes() { return (int32)m_vecMeshBuffers.size(); }
		int32 GetNumMaterial() { return m_vecMaterials.size(); }
		MaterialInfo GetMaterialInfo(int32 _iIndex);

	private:
		HRESULT InitBones(const aiNode* _pAiNode, int32 _iParentBoneIndex);
		HRESULT InitMeshes();
		HRESULT InitAnimations();
		HRESULT InitObjects(const aiNode* _pAiNode, Matrix _matCombined);
		HRESULT InitColliders();
		HRESULT InitMaterials();
		HRESULT InitMaterials_Map();
		HRESULT InitMaterials_Extern();

	private:
		const aiScene* m_pAiScene = {};
		Assimp::Importer m_Importer;

	private:
		wstring                                         m_wstrFilePath = L"";                   // �� ��� - �Ľ̿�
		wstring                                         m_wstrModelName = L"";                  // �� �̸� - ����

		EModelType                                      m_eModelType = EModelType::ENUM_END;	// �� Ÿ�� - ����

		vector<shared_ptr<CBone>>                       m_vecBones;                             // �� - ����
		vector<shared_ptr<CMeshBuffer>>                 m_vecMeshBuffers;                       // �޽� ���� - ����
		unordered_map<wstring, shared_ptr<CAnimation>>	m_umapAnimations;                       // �ִϸ��̼� - ����
		vector<ObjectInfo>				                m_vecObjects;
		vector<ColliderInfo>							m_vecColliders;
		vector<MaterialInfo>                            m_vecMaterials; // ���׸��� ����

	public:
		static vector<wstring>                          m_vecReservedMeshes;                    // ���� ����� �޽� ��� - �ߺ� �ε� ������
	};
}