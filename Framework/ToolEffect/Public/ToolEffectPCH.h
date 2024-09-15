#pragma once

// Windows ���̺귯��
#define NOMINMAX
#include <Windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

// imgui ���̺귯��
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// assimp ���̺귯��
#include <postprocess.h>
#include <Importer.hpp>
#include <scene.h>

// SimpleMath
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

// ����� ���� ���̺귯��
#include "ToolEffectDefine.h"

// PhysX ���̺귯��
#define PVD_HOST "127.0.0.1"
#include <PxPhysicsAPI.h>
using namespace physx;

using namespace ToolEffect;