#pragma once

// Windows ���̺귯��
#define NOMINMAX
#include <Windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

// ImGui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
// #include <ImGuizmo.h>

// JSONCPP ���̺귯��
#include <forwards.h>
#include <json.h>

// RapidJSON
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "prettywriter.h"

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

// ����� ���� ���̺귯��
#include "ToolUIDefine.h"
#include "ToolUIEnum.h"
#include "ToolUIStruct.h"
using namespace ToolUI;