#pragma once

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wincodec.h>

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "SimpleMath.h"

using Vector2	 = DirectX::SimpleMath::Vector2;
using Vector3	 = DirectX::SimpleMath::Vector3;
using Vector4	 = DirectX::SimpleMath::Vector4;
using Quaternion = DirectX::SimpleMath::Quaternion;
using Matrix	 = DirectX::SimpleMath::Matrix;
using Color		 = DirectX::SimpleMath::Color;

#include "./System/Context.h"
#include "./Engine.h"

//Utilities
#include "./Utility/FamilyTypeID.h"
#include "./Utility/Handle.h"
#include "./Utility/FileStream.h"
#include "./Utility/Geometry.h"
#include "./Utility/ChunkAllocator.h"

//Graphics
#include "D3D11/ConstantBuffer.h"
#include "D3D11/DepthStencilState.h"
#include "D3D11/RasterizerState.h"
#include "D3D11/BlendState.h"
#include "D3D11/Shader.h"
#include "D3D11/InputLayout.h"

//System
#include "./System/Time.h"
#include "./System/Window.h"
#include "./System/Input.h"
#include "./System/Graphics.h"
#include "./System/ResourceManager.h"
#include "./System/ComponentManager.h"
#include "./System/SceneManager.h"
#include "./System/Renderer.h"

//Resource
#include "./Resource/Resource.h"
#include "./Resource/Texture.h"
#include "./Resource/Mesh.h"

//Core
#include "./Core/Scene.h"
#include "./Core/GameObject.h"

//Component
#include "./Core/Component/Component.h"
#include "./Core/Component/Transform.h"
#include "./Core/Component/Camera.h"
#include "./Core/Component/MeshRenderer.h"