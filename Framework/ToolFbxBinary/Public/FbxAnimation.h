#pragma once

namespace FbxBinary
{
	class CModel;
	class CChannel;
	class CBone;

	class CAnimation final
	{
		/********************
			Ctor/Dtor
		********************/
	public:
		CAnimation() = default;
		virtual ~CAnimation() = default;

		/********************
			Get
		********************/
	public:

		/********************
			Events
		********************/
	public:
		/*
		m_fDuration	 : 키프레임 수 (float로 형변환)
		m_fTickPerSec : 틱퍼세컨드 (flost로 형변환)
		m_umapChannels : 해당 애니메이션이 가지고 있는 Channel 데이터묶음
		m_vecBonesRef : 모델이 들고있는 뼈들에 접근하기 편하도록 초기화 단계에서 _spModel->GetBones() 를 이용해 받아놓기
		*/
		static shared_ptr<CAnimation>	Create(const aiAnimation* _pAiAnimation, shared_ptr<CModel>_spModel);
		virtual void					Initialize(const aiAnimation* _pAiAnimation, shared_ptr<CModel>_spModel);

		/********************
			Methods
		********************/
	public:
		void SaveToDat(ofstream& _ofFile);

	private:

		/********************
			Data Members
		********************/
	private:
		wstring 				m_wstrAnimName = L"";
		float 					m_fDuration = 0.f;
		float 					m_fTickPerSec = 0.f;
		unordered_map<uint32, shared_ptr<CChannel>>			m_umapChannels;
	};
}