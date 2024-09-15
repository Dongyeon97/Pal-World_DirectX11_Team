#include "pch.h"
#include "NotifyData.h"
#include "StringUtility.h"

shared_ptr<CNotifyData> AnimNotifyExtractor::CNotifyData::Create(string& _strJsonFilePath)
{
	shared_ptr<CNotifyData> spInstance = make_shared<CNotifyData>();
	spInstance->Initialize(_strJsonFilePath);

	return spInstance;
}

void AnimNotifyExtractor::CNotifyData::Initialize(string& _strJsonFilePath)
{
	ifstream inputFile(_strJsonFilePath);
	if (inputFile.fail())
	{
		assert("Invalid File Path");
		inputFile.close();
		return;
	}

	filesystem::path filePath(_strJsonFilePath);
	m_strFileName = filePath.filename().string();
	inputFile >> m_Root;

	// 정의부의 Notify데이터 긁어오기
	Json::Value DefinePart = m_Root[0];

	for (auto NotifyIter = DefinePart["Properties"]["Notifies"].begin(); NotifyIter != DefinePart["Properties"]["Notifies"].end(); ++NotifyIter)
	{
		string NotifyName = (*NotifyIter)["NotifyName"].asString();

		//  충돌관련 Notify
		if ("PalAttackCollision" == NotifyName ||
			"PalAttackCollision_Sphere" == NotifyName)
			ExtractCollisionData(*NotifyIter);
		else if ("AkEvent_C" == NotifyName)
			ExtractSoundData(*NotifyIter);
	}

	inputFile.close();
}

void AnimNotifyExtractor::CNotifyData::ExtractCollisionData(const Json::Value& _notify)
{
	FCollisionNotifyData tData;
	tData.m_fNotifyDuration = _notify["Duration"].asFloat();
	tData.m_fNotifyStartTime = _notify["LinkValue"].asFloat();
	tData.m_fNotifyEndTime = tData.m_fNotifyStartTime + tData.m_fNotifyDuration;

	Json::Value HeaderData;

	if (!_notify["Notify"].isNull())
	{
		string strObjname = _notify["Notify"]["ObjectName"].asString();
		HeaderData = GetHeaderData(strObjname);
	}
	else if(!_notify["NotifyStateClass"].isNull())
	{
		string strObjname = _notify["NotifyStateClass"]["ObjectName"].asString();
		HeaderData = GetHeaderData(strObjname);
	}
	else
	{
		assert("Header Data Not Exsist");
		return;
	}
		
	tData.m_strAttachBoneName = HeaderData["Properties"]["attachBoneName"].asString();

	// 충돌체 정보
	Json::Value ShapeComponentValue = HeaderData["Properties"]["ShapeComponents"];
	for (auto ShapeComponetIter = ShapeComponentValue.begin(); ShapeComponetIter != ShapeComponentValue.end(); ++ShapeComponetIter)
	{
		FColliderOffsetData tOffsetData;

		string ShapeComponentName = (*ShapeComponetIter)["ObjectName"].asString();
		Json::Value ColliderData = GetDeclareData(ShapeComponentName);

		tOffsetData.m_fSphereRadius = ColliderData["Properties"]["SphereRadius"].asFloat();
		tOffsetData.m_vRelativeLocation.x = ColliderData["Properties"]["RelativeLocation"]["X"].asFloat();
		tOffsetData.m_vRelativeLocation.y = ColliderData["Properties"]["RelativeLocation"]["Y"].asFloat();
		tOffsetData.m_vRelativeLocation.z = ColliderData["Properties"]["RelativeLocation"]["Z"].asFloat();
		
		tData.m_lstColOffsetDatas.push_back(tOffsetData);
	}

	m_lstCollisionNotifyDatas.push_back(tData);
}

void AnimNotifyExtractor::CNotifyData::ExtractSoundData(const Json::Value& _notify)
{
	FSoundNotifyData tData;
	tData.m_fNotifyDuration = _notify["Duration"].asFloat();
	tData.m_fNotifyStartTime = _notify["LinkValue"].asFloat();
	tData.m_fNotifyEndTime = tData.m_fNotifyStartTime + tData.m_fNotifyDuration;

	Json::Value HeaderData;

	if (!_notify["Notify"].isNull())
	{
		string strObjname = _notify["Notify"]["ObjectName"].asString();
		HeaderData = GetHeaderData(strObjname);
	}
	else if (!_notify["NotifyStateClass"].isNull())
	{
		string strObjname = _notify["NotifyStateClass"]["ObjectName"].asString();
		HeaderData = GetHeaderData(strObjname);
	}
	else
	{
		assert("Header Data Not Exsist");
		return;
	}

	// 사운드 정보가 들어있는 파일의 경로 읽어오기
	string strDeclareFilePath = HeaderData["Properties"]["Event"]["ObjectPath"].asString();
	int iDotPos = strDeclareFilePath.find('.');
	strDeclareFilePath = "../../Resource/Models/Materials/" + strDeclareFilePath.substr(0, iDotPos) + ".json";

	// 사운드 정보가 있는 폴더에 있는 json파일 열기
	ifstream ifDeclareDat(strDeclareFilePath);
	Json::Value DeclareValue;
	ifDeclareDat >> DeclareValue;

	// AkAudioEventData 에서 사운드파일 이름(숫자)을 추출해오기
	for (auto AudioDataIter = DeclareValue.begin(); AudioDataIter != DeclareValue.end(); ++AudioDataIter)
	{
		if ((*AudioDataIter)["Type"].asString() == "AkAudioEventData")
		{
			Json::Value mediaList = (*AudioDataIter)["Properties"]["MediaList"];

			for (auto MediaIter = mediaList.begin(); MediaIter != mediaList.end(); ++MediaIter)
			{
				string strSoundID = (*MediaIter)["ObjectName"].asString();

				int soundIDStart = strSoundID.find('\'');
				int soundIDEnd = strSoundID.find('\'', soundIDStart+1);

				strSoundID = strSoundID.substr(soundIDStart + 1, soundIDEnd - soundIDStart - 1);

				tData.m_lstSoundID.push_back(strSoundID);
			}
		}
	}

	m_lstSoundNotifyDatas.push_back(tData);
}

void AnimNotifyExtractor::CNotifyData::WriteJson(const string& _strWriteFilePath)
{
	string strWriteFilePath = _strWriteFilePath + '/' + m_strFileName;
	ofstream ostream(strWriteFilePath);
	if (ostream.fail())
	{
		assert("invalid output filePath");
		ostream.close();
		return;
	}

	// 파싱
	Json::Value OutPut;
	
	// 충돌이벤트 데이터 기록
	for (auto& tCollisionDat : m_lstCollisionNotifyDatas)
	{
		Json::Value ColDat;

		// 디폴트 데이터
		ColDat["Type"]	= "Collision";
		ColDat["Duration"] = tCollisionDat.m_fNotifyDuration;
		ColDat["StartTime"] = tCollisionDat.m_fNotifyStartTime;
		ColDat["EndTime"] = tCollisionDat.m_fNotifyEndTime;

		// 충돌 이벤트관련 데이터
		ColDat["AttachBoneName"] = tCollisionDat.m_strAttachBoneName;
		
		for (auto& ColliderOffsetData : tCollisionDat.m_lstColOffsetDatas)
		{
			Json::Value ColliderInfo;

			ColliderInfo["SphereRadius"] = ColliderOffsetData.m_fSphereRadius;
			ColliderInfo["vRelativeLocationX"] = ColliderOffsetData.m_vRelativeLocation.x;
			ColliderInfo["vRelativeLocationY"] = ColliderOffsetData.m_vRelativeLocation.y;
			ColliderInfo["vRelativeLocationZ"] = ColliderOffsetData.m_vRelativeLocation.z;

			ColDat["OffsetDataArray"].append(ColliderInfo);
		}

		OutPut.append(ColDat);
	}

	// 사운드이벤트 데이터 기록
	for (auto& tSoundDat : m_lstSoundNotifyDatas)
	{
		Json::Value SoundDat;

		// 디폴트 데이터
		SoundDat["Type"] = "Sound";
		SoundDat["Duration"] = tSoundDat.m_fNotifyDuration;
		SoundDat["StartTime"] = tSoundDat.m_fNotifyStartTime;
		SoundDat["EndTime"] = tSoundDat.m_fNotifyEndTime;

		// 사운드 이벤트관련 데이터
		for (auto& SoundID : tSoundDat.m_lstSoundID)
		{
			SoundDat["SoundIDArray"].append(SoundID);
		}

		OutPut.append(SoundDat);
	}

	// 기록된 데이터 파싱
	ostream << OutPut;

	ostream.close();
}

Json::Value AnimNotifyExtractor::CNotifyData::GetHeaderData(const string& _strObjectName)
{
	Json::Value result;

	int iTypeEnd = _strObjectName.find('\'');
	string strType = _strObjectName.substr(0, iTypeEnd);

	int iNameStart = _strObjectName.find(':',iTypeEnd);
	int iNameEnd = _strObjectName.find('\'', iNameStart);
	string strName = _strObjectName.substr(iNameStart + 1, iNameEnd - iNameStart - 1);

	auto DataIter = m_Root.begin();
	++DataIter;
	for (; DataIter != m_Root.end(); ++DataIter)
	{
		if ((*DataIter)["Type"].asString() == strType)
		{
			if ((*DataIter)["Name"].asString() == strName.c_str())
				return (*DataIter);
		}
	}

	assert("Header Value Not exsist");

	return result;
}

Json::Value AnimNotifyExtractor::CNotifyData::GetDeclareData(const string& _strObjectName)
{
	Json::Value result;

	int iTypeEnd = _strObjectName.find('\'');
	string strType = _strObjectName.substr(0, iTypeEnd);

	int iOuterStart = _strObjectName.find(':', iTypeEnd);
	int iOuterEnd = _strObjectName.find('.', iOuterStart);
	string strOuter = _strObjectName.substr(iOuterStart + 1, iOuterEnd - iOuterStart - 1);

	int iNameStart = _strObjectName.find('.', iOuterEnd);
	int iNameEnd = _strObjectName.find('\'', iNameStart);
	string strName = _strObjectName.substr(iNameStart + 1, iNameEnd - iNameStart - 1);

	auto DataIter = m_Root.begin();
	++DataIter;
	for (; DataIter != m_Root.end(); ++DataIter)
	{
		if ((*DataIter)["Type"].asString() == strType)
		{
			if ((*DataIter)["Outer"].asString() == strOuter)
			{
				if ((*DataIter)["Name"].asString() == strName)
					return (*DataIter);
			}
		}
	}

	assert("Declare Value Not exsist");

	return result;
}
