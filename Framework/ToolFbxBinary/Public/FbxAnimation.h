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
		m_fDuration	 : Ű������ �� (float�� ����ȯ)
		m_fTickPerSec : ƽ�ۼ����� (flost�� ����ȯ)
		m_umapChannels : �ش� �ִϸ��̼��� ������ �ִ� Channel �����͹���
		m_vecBonesRef : ���� ����ִ� ���鿡 �����ϱ� ���ϵ��� �ʱ�ȭ �ܰ迡�� _spModel->GetBones() �� �̿��� �޾Ƴ���
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