// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// Fbx_Exporter.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "pch.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.

string WConvertToString(wstring _wstr)
{
	string strConvertToString;
	strConvertToString.assign(_wstr.begin(), _wstr.end());
	return strConvertToString;
}
wstring ConvertToWstring(string _str)
{
	wstring strConvertToWstring;
	strConvertToWstring.assign(_str.begin(), _str.end());
	return strConvertToWstring;
}