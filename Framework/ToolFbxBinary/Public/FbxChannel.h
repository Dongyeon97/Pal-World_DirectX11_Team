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
		m_iBoneIndex : 해당 채널이 공유하는 뼈의 인덱스 -> assimp파싱 단계에서 channel이름이 같은 본의 index를 매칭해주세요
		m_vecKeyFrames : 키프레임 구조체 묶음 (키프레임 시간, 포지션, 로테이션, 스케일) -> assimp파싱 단계에서 같은 형태의 구조체로 한번에 저장했을때 편했음
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
		float				m_fLerpTime = 0.2f; // lerp에 사용되는 시간값
		int32				m_iBoneIndex = 0;	// bone의 인덱스

		vector<KEYFRAME>	m_vecKeyFrames;
	};
}