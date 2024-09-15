#pragma once

namespace AnimNotifyExtractor
{
	class CNotifyData
	{
	public:
		enum NotifyType
		{
			Collision,
			Sound,
			END
		};

	public:
		struct FNotifyData
		{
			float m_fNotifyDuration = 0.f;
			float m_fNotifyStartTime = 0.f;
			float m_fNotifyEndTime = 0.f;

			NotifyType m_eNotifyType = END;
		};

		// 충돌 이벤트 구조체
		struct FColliderOffsetData
		{
			float m_fSphereRadius = 0.f;
			Vector3 m_vRelativeLocation = Vector3(0.f, 0.f, 0.f);
		};

		struct FCollisionNotifyData : public FNotifyData
		{
			FCollisionNotifyData() { m_eNotifyType = Collision; }

			string m_strAttachBoneName = "";
			list<FColliderOffsetData> m_lstColOffsetDatas;
		};

		// 사운드 이벤트 구조체
		struct FSoundNotifyData : public FNotifyData
		{
			FSoundNotifyData() { m_eNotifyType = Sound; }

			list<string> m_lstSoundID;
		};

	public:
		CNotifyData() = default;
		~CNotifyData() = default;

	public:
		static shared_ptr<CNotifyData> Create(string& _strJsonFilePath);

		void Initialize(string& _strJsonFilePath);

		// 충돌 데이터 추출
		void ExtractCollisionData(const Json::Value& _notify);

		// 사운드 데이터 추출
		void ExtractSoundData(const Json::Value& _notify);

		void WriteJson(const string& _strWriteFilePath);

	private:
		// Notifies에서 찾은 데이터로 찾아가기
		Json::Value GetHeaderData(const string& _strObjectName);

		// Header에서 찾은 데이터로 찾아가기
		Json::Value GetDeclareData(const string& _strObjectName);

	private:
		string m_strFileName = "";

		Json::Value m_Root;

		// 충돌 이벤트 데이터리스트
		list<FCollisionNotifyData> m_lstCollisionNotifyDatas;

		// 사운드 이벤트 데이터리스트
		list<FSoundNotifyData> m_lstSoundNotifyDatas;

	};
}


