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
			wstring wstrObjectName = L"";	// 오브젝트 이름
			vector<wstring> vecMeshNames;	// 이 오브젝트가 사용하는 메쉬 이름
			Matrix matWorld;				// 이 오브젝트의 월드 행렬
		};
		struct ColliderInfo
		{
			EColliderType eColliderType = EColliderType::ENUM_END;	// 타입
			wstring wstrColliderName = L"";				// 이름
			wstring wstrParentBoneName = L"";			// 부모 본 이름
			Vector3 vCenter = Vector3::Zero;			// 중점 - 상대 좌표
			Vector3 vRotationYPR = Vector3::Zero;		// 회전 - YawPitchRoll 순서로 XYZ에 들어간다.
			float fRadius = 0;							// 반지름 - Sphere, Sphy
			float fLength = 0;							// 길이 - Sphy
		};
		struct MaterialInfo
		{
			wstring wstrMaterialName = L"";														// 이 메테리얼의 이름 - 공용
			wstring wstrShaderTag = L"";                                                        // 이 메테리얼이 사용하는 쉐이더 타입 - 공용
			unordered_map<wstring, wstring> umapTextures;										// 텍스처 용도, 텍스처 경로 - 공용
			unordered_map<wstring, float> umapScalars;											// 스칼라 용도, 스칼라 값
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
		wstring                                         m_wstrFilePath = L"";                   // 모델 경로 - 파싱용
		wstring                                         m_wstrModelName = L"";                  // 모델 이름 - 공용

		EModelType                                      m_eModelType = EModelType::ENUM_END;	// 모델 타입 - 공용

		vector<shared_ptr<CBone>>                       m_vecBones;                             // 뼈 - 공용
		vector<shared_ptr<CMeshBuffer>>                 m_vecMeshBuffers;                       // 메시 버퍼 - 공용
		unordered_map<wstring, shared_ptr<CAnimation>>	m_umapAnimations;                       // 애니메이션 - 공용
		vector<ObjectInfo>				                m_vecObjects;
		vector<ColliderInfo>							m_vecColliders;
		vector<MaterialInfo>                            m_vecMaterials; // 메테리얼 정보

	public:
		static vector<wstring>                          m_vecReservedMeshes;                    // 생성 예약된 메쉬 목록 - 중복 로드 방지용
	};
}