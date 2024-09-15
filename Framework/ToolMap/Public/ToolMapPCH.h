#pragma once

// Windows ���̺귯��
#define NOMINMAX
#include <Windows.h>
#include <wrl.h>
#include <tuple>
using namespace Microsoft::WRL;

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imGuizmo.h>

// JSONCPP ���̺귯��
#include <forwards.h>
#include <json.h>

// RapidJSON
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "prettywriter.h"

#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTKd.lib")

// ����� ���� ���̺귯��
#include "ToolMapDefine.h"
#include "ToolMapEnum.h"
#include "ToolMapStruct.h"
using namespace ToolMap;