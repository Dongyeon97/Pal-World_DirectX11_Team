#pragma once

namespace FbxBinary
{
	class CBone;
	class CModel;

	class CChannel final
	{
		/********************
			Struct
		********************/
	public:
		struct KEYFRAME
		{
			Vector3 vScale, vPosition;
			Quaternion qRotation;
			float fTime;
		};

		/********************
			Ctor/Dtor
		********************/
	public:
		CChannel() = default;
		virtual ~CChannel() = default;

		/********************
			Get
		********************/
	public:
		KEYFRAME	GetStartKeyFrame() { return m_vecKeyFrames[0]; }
		int32		GetNumKeyFrame() { return (int32)m_vecKeyFrames.size(); }
		int32		GetBoneIndex() { return m_iBoneIndex; }

		/********************
			Get
		********************/
	public:

		/********************
			Events
		********************/
	public:
		/*
		m_iBoneIndex : �ش� ä���� �����ϴ� ���� �ε��� -> assimp�Ľ� �ܰ迡�� channel�̸��� ���� ���� index�� ��Ī���ּ���
		m_vecKeyFrames : Ű������ ����ü ���� (Ű������ �ð�, ������, �����̼�, ������) -> assimp�Ľ� �ܰ迡�� ���� ������ ����ü�� �ѹ��� ���������� ������
		*/
		static std::shared_ptr<CChannel> Create(const aiNodeAnim* _pAiChannel, shared_ptr<CModel> _spModel);
		virtual void Initialize(const aiNodeAnim* _pAiChannel, shared_ptr<CModel> _spModel);

		/********************
			Methods
		********************/

		void SaveToDat(ofstream& _ofFile);
	private:

		/********************
			Data Members
		********************/
	private:
		wstring				m_wstrChannelName = L"";
		float				m_fLerpTime = 0.2f; // lerp�� ���Ǵ� �ð���
		int32				m_iBoneIndex = 0;	// bone�� �ε���

		vector<KEYFRAME>	m_vecKeyFrames;
	};
}