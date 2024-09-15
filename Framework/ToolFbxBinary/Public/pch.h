#pragma once
#pragma warning(disable:4251)

// Windows 라이브러리
#include <Windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

// Direct3D 라이브러리
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>

#define INITGUID
#include <d3d11shader.h>

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <ScreenGrab.h>

#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
using namespace DirectX;

// C++ 표준 라이브러리
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <utility>
#include <functional>
#include <memory>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <cstdarg>
using namespace std;

// C 라이브러리
#include <cassert>
#include <cmath>
#include <ctime>

// 사용자 정의 헤더 파일
#include "FbxBinaryType.h"
#include "FbxBinaryDefine.h"
#include "FbxBinaryEnum.h"
#include "FbxBinaryStruct.h"
using namespace FbxBinary;

#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
using namespace DirectX;

string WConvertToString(wstring _wstr);
wstring ConvertToWstring(string _str);