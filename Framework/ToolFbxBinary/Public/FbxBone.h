#pragma once

namespace FbxBinary
{
	class CModel;
	class CBone final
	{
		/********************
			Ctor/Dtor
		********************/
	public:
		CBone() = default;
		virtual ~CBone() = default;

		/********************
			Get
		********************/
	public:
		wstring GetBoneName() { return m_wstrBoneName; }
		int32 GetParentBoneIndex() { return m_iParentBoneIndex; }
		int32 GetMyIndex() { return m_iMyIndex; }
		const list<int32>& GetChildBoneIndices() { return m_lstChildBoneIndices; }
		void AddChildIndex(int32 _iChildIndex) { m_lstChildBoneIndices.push_back(_iChildIndex); }
		void SetIndex(int32 _iIndex) { m_iMyIndex = _iIndex; }
		/********************
			Set
		********************/
		void SetTransformationMatrix(const Matrix& _matTransformation) { m_matTransformation = _matTransformation; }

		/********************
			Events
		********************/
	public:
		/*
		m_strName : 본의 이름(필수)

		m_iParentBoneIndex(부모의 본 인덱스),	m_iMyIndex(내 본 인덱스), m_lstChildBoneIndices(자식의 본 인덱스들list)

		m_matInitialTransformation : 초기상태의 상태행렬, 초기화 단계에서
		 (m_matInitialTransformation = m_matTransformation = 파싱상태행렬 데이터) 의 형태로 초기화
		*/
		static shared_ptr<CBone> Create(const aiNode* _pAiNode, int32 _iParentBoneIndex);
		virtual void Initialize(const aiNode* _pAiNode, int32 _iParentBoneIndex);
		void SaveToDat(ofstream& _ofFile);

	private:

		/********************
			Data Members
		********************/
	private:
		wstring			m_wstrBoneName = L"";

		int32			m_iParentBoneIndex;
		int32			m_iMyIndex;
		list<int32>		m_lstChildBoneIndices;

		Matrix			m_matTransformation;			// 부모기준으로 표현된 자신만의 상태행렬
	};
}


