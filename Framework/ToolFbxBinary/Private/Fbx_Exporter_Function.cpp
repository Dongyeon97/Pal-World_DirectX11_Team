#include "Fbx_Exporter_Defines.h"

namespace Fbx_Exporter
{
	std::string WConvertToString(std::wstring wstr)
	{
		std::string strConvertToString;
		strConvertToString.assign(wstr.begin(), wstr.end());
		return strConvertToString;
	}

	std::wstring ConvertToWstring(std::string str)
	{
		std::wstring strConvertToWstring;
		strConvertToWstring.assign(str.begin(), str.end());
		return strConvertToWstring;
	}
}