// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// Fbx_Exporter.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "pch.h"

// TODO: �ʿ��� �߰� �����
// �� ������ �ƴ� STDAFX.H���� �����մϴ�.

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