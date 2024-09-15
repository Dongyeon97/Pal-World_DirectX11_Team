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

		// �浹 �̺�Ʈ ����ü
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

		// ���� �̺�Ʈ ����ü
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

		// �浹 ������ ����
		void ExtractCollisionData(const Json::Value& _notify);

		// ���� ������ ����
		void ExtractSoundData(const Json::Value& _notify);

		void WriteJson(const string& _strWriteFilePath);

	private:
		// Notifies���� ã�� �����ͷ� ã�ư���
		Json::Value GetHeaderData(const string& _strObjectName);

		// Header���� ã�� �����ͷ� ã�ư���
		Json::Value GetDeclareData(const string& _strObjectName);

	private:
		string m_strFileName = "";

		Json::Value m_Root;

		// �浹 �̺�Ʈ �����͸���Ʈ
		list<FCollisionNotifyData> m_lstCollisionNotifyDatas;

		// ���� �̺�Ʈ �����͸���Ʈ
		list<FSoundNotifyData> m_lstSoundNotifyDatas;

	};
}


