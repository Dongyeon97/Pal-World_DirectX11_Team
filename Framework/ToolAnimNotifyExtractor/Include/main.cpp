#include "pch.h"
#include "NotifyData.h"

string g_strOutputPath = "../../Resource/Models/AnimNotify";

void ExtractJson(const string& _strRootFile);

int main()
{
	string strRootFilePath = "../../Resource/Models/Materials/Pal/Content/Pal/Animation";

	ExtractJson(strRootFilePath);
}

void ExtractJson(const string& _strRootFile)
{
	filesystem::path path(_strRootFile);
	filesystem::directory_iterator iter(path);

	while (iter != filesystem::end(iter))
	{
		if (iter->is_regular_file())
		{
			if (iter->path().extension() == ".json")
			{
				string filePath = iter->path().string();

				cout << "Start Parsing : " << filePath << endl;				
				shared_ptr<CNotifyData> NotifyData = CNotifyData::Create(filePath);
				
				if (!NotifyData)
					assert("Error");

				NotifyData->WriteJson(g_strOutputPath);

				NotifyData.reset();

				cout << "EndParsing : " << filePath << endl;
				cout << "---------------------------------------------" << endl;
			}
		}
		else if (iter->is_directory())
		{
			ExtractJson(iter->path().string());
		}

		++iter;
	}

}
