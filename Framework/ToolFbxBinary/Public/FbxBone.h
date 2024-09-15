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
		m_strName : ���� �̸�(�ʼ�)

		m_iParentBoneIndex(�θ��� �� �ε���),	m_iMyIndex(�� �� �ε���), m_lstChildBoneIndices(�ڽ��� �� �ε�����list)

		m_matInitialTransformation : �ʱ������ �������, �ʱ�ȭ �ܰ迡��
		 (m_matInitialTransformation = m_matTransformation = �Ľ̻������ ������) �� ���·� �ʱ�ȭ
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

		Matrix			m_matTransformation;			// �θ�������� ǥ���� �ڽŸ��� �������
	};
}


