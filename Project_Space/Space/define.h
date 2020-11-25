#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <iostream>
#include <Windows.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

typedef XMFLOAT2 Vec2;
//typedef XMFLOAT3 Vec3;
typedef XMFLOAT4 Vec4;
typedef XMMATRIX Matrix;

#include <vector>
#include <list>
#include <map>

// common header
#include "struct.h"

#define SINGLE(T) public : static T* GetInst()\
{\
static T instance;\
return &instance;\
}\
private: \
T(); \
~T();

#define DT CTimer::GetInst()->GetDeltaTime()

#define SAFE_RELEASE(p) if(p != nullptr) p->Release();
#define SAFE_DELETE(p) if(p != nullptr) delete p; p = nullptr;


