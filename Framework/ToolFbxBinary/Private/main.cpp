#include "pch.h"
#include <iostream>
#include <cstdlib>
#include "FbxModel.h"
using namespace std;

int main()
{
	vector<shared_ptr<FbxBinary::CModel>> vecModels;
	wcout << L"Loading Fbx Files" << endl;
	/*

	wcout << L"-----------------------------------------------------" << endl;
	wcout << L"Loading Anim Models" << endl;
	for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(L"../../Resource/Models/Fbx/Anim"))
	{
		if (!filesystem::is_regular_file(fsEntry) || fsEntry.path().extension().generic_string() != ".fbx") { continue; }
		wstring wstrFilePath = fsEntry.path().generic_wstring();
		wcout << "Current : " << wstrFilePath << endl;
		vecModels.push_back(CModel::Create(EModelType::ANIM, wstrFilePath));
	}
	
	 wcout << L"-----------------------------------------------------" << endl;
	 wcout << L"Loading NonAnim Models" << endl;
	 for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(L"../../Resource/Models/Fbx/NonAnim"))
	 {
	 	if (!filesystem::is_regular_file(fsEntry) || fsEntry.path().extension().generic_string() != ".fbx") { continue; }
	 	wstring wstrFilePath = fsEntry.path().generic_wstring();
	 	wcout << "Current : " << wstrFilePath << endl;
	 	vecModels.push_back(CModel::Create(EModelType::NONANIM, wstrFilePath));
	 }
	 wcout << L"-----------------------------------------------------" << endl;
	 wcout << L"Loading Landscape Models" << endl;
	 for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(L"../../Resource/Models/Fbx/Map_Landscape"))
	 {
	 	if (!filesystem::is_regular_file(fsEntry) || fsEntry.path().extension().generic_string() != ".fbx") { continue; }
	 
	 	wstring wstrFilePath = fsEntry.path().generic_wstring();
	 	wcout << "Current : " << wstrFilePath << endl;
	 	vecModels.push_back(CModel::Create(EModelType::MAP_LANDSCAPE, wstrFilePath));
	 }
	 
	 wcout << L"-----------------------------------------------------" << endl;
	 wcout << L"Loading DefaultObject Models" << endl;
	 for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(L"../../Resource/Models/Fbx/Map_DefaultObject"))
	 {
	 	if (!filesystem::is_regular_file(fsEntry) || fsEntry.path().extension().generic_string() != ".fbx") { continue; }
	 	wstring wstrFilePath = fsEntry.path().generic_wstring();
	 	wcout << "Current : " << wstrFilePath << endl;
	 	vecModels.push_back(CModel::Create(EModelType::MAP_OBJECT, wstrFilePath));
	 }
	 
	 wcout << L"-----------------------------------------------------" << endl;
	 wcout << L"Loading DefaultObject Collision Models" << endl;
	 for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(L"../../Resource/Models/Fbx/Map_DefulatObject_Collision"))
	 {
	 	if (!filesystem::is_regular_file(fsEntry) || fsEntry.path().extension().generic_string() != ".fbx") { continue; }
	 	wstring wstrFilePath = fsEntry.path().generic_wstring();
	 	wcout << "Current : " << wstrFilePath << endl;
	 	vecModels.push_back(CModel::Create(EModelType::MAP_OBJECT_COLLISION, wstrFilePath));
	 }*/
	 
	 wcout << L"-----------------------------------------------------" << endl;
	 wcout << L"Loading Extern Models" << endl;
	 for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(L"../../Resource/Models/Fbx/Extern"))
	 {
	 	if (!filesystem::is_regular_file(fsEntry) || fsEntry.path().extension().generic_string() != ".fbx") { continue; }
	 	wstring wstrFilePath = fsEntry.path().generic_wstring();
	 	wcout << "Current : " << wstrFilePath << endl;
	 	vecModels.push_back(CModel::Create(EModelType::EXTERN, wstrFilePath));
	 }

	wcout << L"Convering Fbx Files to Bin" << endl;
	wcout << L"-----------------------------------------------------" << endl;

	for (auto& spModel : vecModels)
	{
		spModel->SaveToDat();
		spModel = nullptr;
	}

	wcout << L"Done" << endl;
	wcout << L"Converted : " << vecModels.size() << endl;
	wcout << L"-----------------------------------------------------" << endl;
}