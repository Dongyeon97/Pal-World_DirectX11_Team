#pragma once

namespace FbxBinary
{
	class CBone;
	class CModel;
	class CMeshBuffer final
	{
		/********************
			Ctor/Dtor
		********************/
	public:
		CMeshBuffer() = default;
		CMeshBuffer(const CMeshBuffer& rhs) = default;
		virtual ~CMeshBuffer() = default;

		/********************
			Get
		********************/
	public:
		int32 GetNumBones() { return (int32)m_vecBoneIndices.size(); }
		int32 GetMaterialIndex() { return m_iMaterialIndex; }
		const wstring& GetMeshName() { return m_wstrMeshName; }

		/********************
			Events
		********************/
	public:
		static shared_ptr<CMeshBuffer> Create(EModelType _eModelType, const aiMesh* _pAiMesh, shared_ptr<CModel>(_pModel));
		virtual HRESULT Initialize(EModelType _eModelType, const aiMesh* _pAiMesh, shared_ptr<CModel>(_spModel));
		void SaveToDat(ofstream& _ofFile, shared_ptr<CModel>(_pModel));

	private:
		void InitAnimMesh(const aiMesh* _pAiMesh, shared_ptr<CModel> _spModel);
		void InitNonAnimMesh(const aiMesh* _pAiMesh);
		void InitLandscapeMesh(const aiMesh* _pAiMesh);
		void InitCollisionMesh(const aiMesh* _pAiMesh);

		/********************
			Data Members
		********************/
	private:
		wstring								                            m_wstrMeshName;
		int32															m_iMaterialIndex = -1;
		bool  		                                                    m_bNeedToCreateMeshDat = false;

		uint32  	                                                    m_iNumVertices = 0;
		uint32  	                                                    m_iNumIndices = 0;

		vector<uint32> 						                            m_vecBoneIndices;		// 메쉬가 사용하는 뼈 인덱스들
		vector<wstring> 						                        m_vecBoneNames;
		vector<Matrix> 						                            m_vecOffsetMatrices;

		vector<FPositionNormalTexCoord2TangentBlendIndexBlendWeight>	m_vecAnimBurffer;
		vector<FPositionNormalTexCoord2Tangent>                         m_vecNonAnimBurffer;
		vector<FPositionNormalTexCoord2TexCoord2Tangent>				m_vecLandscapeBurffer;

		vector<uint32> 						                            m_vecIndexBuffer;
	};
}