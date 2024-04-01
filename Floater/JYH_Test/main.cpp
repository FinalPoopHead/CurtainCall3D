#define FLT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS

#include "../FloaterPlatform/include/Platform.h"
#include "../FloaterRendererCommon/include/Transform.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/Timer.h"
#include <iostream>

#include "../FloaterUtil/include/ConvString.h"

#pragma region testInclude
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"
#include "../FloaterUtil/TesterRBTree.h"

#include "../FloaterUtil/TesterFixedSizeMemoryPool.h"
#include "../FloaterUtil/include/Hash.h"

#include "../FloaterRendererCommon/include/Camera.h"

#include "../FloaterRendererCommon/include/ModelLoader.h"
#include "../FloaterRendererCommon/CoordSystem.h"
#include <unordered_map>
#include <filesystem>

#include "../FloaterMath/include/Common.h"
#include "../FloaterMath/include/Vector2f.h"
#include "../FloaterMath/include/Vector3f.h"

#include <windows.h>

#include "../FloaterGameEngine/include/Component.h"

#pragma endregion


#pragma warning(push)
#pragma warning(disable: 4717)
void StackOverflow()
{
	int i = 10;
	StackOverflow();
	std::cout << i << std::endl;
}
#pragma warning (pop)

char TestGetAxis(const flt::Vector3f& vec)
{
	if (vec.x == 1.0f || vec.x == -1.0f)
	{
		return 'x';
	}
	else if (vec.y == 1.0f || vec.y == -1.0f)
	{
		return 'y';
	}
	else if (vec.z == 1.0f || vec.z == -1.0f)
	{
		return 'z';
	}
	else
	{
		return 'n';
	}
}

void TestVectorPrint(const flt::Vector3f& up, const flt::Vector3f& front, const flt::Vector3f& right)
{
	std::cout << "up : " << TestGetAxis(up)
		<< " front: " << TestGetAxis(front)
		<< " rignt: " << TestGetAxis(right) << std::endl;

	std::cout << "up : " << up.x + up.y + up.z
		<< " front: " << front.x + front.y + front.z
		<< " rignt: " << right.x + right.y + right.z << std::endl;

	std::cout << "--------------------------------" << std::endl;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	std::cout << "create rawScene" << std::endl;
	flt::RawScene rawScene;

#pragma region 테스트
	{
		using namespace flt;
		using namespace flt::test;

		Matrix4f mat
		{
			1.0f, 2.0f, 3.0f, 4.0f,
			5.0f, 6.0f, 7.0f, 8.0f,
			9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		Matrix4f mulMat = mat * mat;


		//TesterRBTree tester;
		//tester.Test();

		Quaternion q1{ 0.0f, 0.0f, 0.0f, 1.0f };
		q1.SetEuler(1.f, 2.f, 3.f);

		ModelLoader loader;
		//std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Ganondorf (TotK).fbx";
		std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying.fbx";
		//std::wstring filePath = L"..\\x64\\fbx\\Models\\A_TP_CH_Breathing.fbx";
		//std::wstring filePath = L"C:\\Users\\KOCCA56\\Desktop\\Bee.glb";
		std::wstring zUpYForward = L"..\\x64\\fbx\\Test\\ZY.fbx";
		std::wstring yUpZForward = L"..\\x64\\fbx\\Test\\YZ.fbx";
		std::wstring xUpYForward = L"..\\x64\\fbx\\Test\\XY.fbx";

		char buffer[1024];
		auto str = ToString(L"..\\x64\\fbx\\Test\\*.*");

		WIN32_FIND_DATAA findData;
		HANDLE handle = FindFirstFileA(str.c_str(), &findData);

		if (handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::cout << findData.cFileName << std::endl;
			} while (FindNextFileA(handle, &findData));
		}
		else
		{
			std::cout << "error" << std::endl;
		}

		GetFullPathNameA(str.c_str(), 1024, buffer, NULL);

		//std::filesystem::path currPath = std::filesystem::current_path();

		loader.Load(filePath, &rawScene);

		auto& node = rawScene.nodes[0];

		for (int i = 0; i < node->meshes.size(); ++i)
		{
			if (node->meshes[i].material.textures[0]->path == L"")
			{
				node->meshes[i].material.textures[0]->path = L"..\\x64\\fbx\\Models\\Textures\\T_TP_CH_Basic_001_001_D.png";
			}
		}

		//node->meshes[0].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\Npc_Ganondorf_Human_Face_1_Alb.png";
		//node->meshes[1].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\Npc_Ganondorf_Human_Metal_Alb.png";
		//node->meshes[2].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\Npc_Ganondorf_Human_Body_Dm_Alb.png";
		//node->meshes[3].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\Npc_Ganondorf_Human_Eyeball_Alb.png";
		//node->meshes[4].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\Npc_Ganondorf_Human_Face_2_Alb.png";
		//node->meshes[5].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\CmnTex_SecretStone_Dm_Ind.png";
		//node->meshes[6].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\Npc_Ganondorf_Human_Forehead_Alb.png";
		//node->meshes[7].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\Npc_Ganondorf_Human_Skin_Dm_Alb.png";
		//node->meshes[8].material.textures[0]->path = L"..\\x64\\fbx\\Ganondorf - 3d - model - dl\\textures\\Npc_Ganondorf_Human_Hair_Alb.png";
	}
#pragma endregion

	//auto adpaterTest = new IZeldaRendererAdapter();
	//adpaterTest->Initialize(1280, 720, false, NULL, false, 0.0f, 0.0f);

	std::cout << "Create flt::Platform" << std::endl;
	flt::Platform platform{ true };
	platform.Initialize(1280, 720, L"title", L".\\path");


	//std::cout << "Create renderer" << std::endl;
	//auto renderer = platform.CreateRenderer(flt::RendererType::ROCKET_DX11);
	auto renderer = platform.CreateRenderer(flt::RendererType::DX11);

	bool isShowCurser = true;

	bool isDraw = true;

	flt::Transform cameraTransform;
	flt::RendererObject cameraObject(cameraTransform, isDraw);
	cameraObject.camera = new flt::Camera(&cameraObject.transform);
	auto cameraID = renderer->RegisterObject(cameraObject);


	flt::RawNode cubeNode(L"testNode");
	cubeNode.transform.SetPosition(0.0f, 0.0f, 0.7f);
	cubeNode.transform.SetScale(1.0f, 1.0f, 1.0f);

	constexpr int fbxObjectCount = 10;
	flt::Transform* fbxTransforms[fbxObjectCount];
	flt::RendererObject* fbxObjects[fbxObjectCount];
	flt::HOBJECT objectIDs[fbxObjectCount];

	for (int i = 0; i < fbxObjectCount; ++i)
	{
		fbxTransforms[i] = new flt::Transform();
		fbxObjects[i] = new flt::RendererObject(*fbxTransforms[i], isDraw);
		fbxObjects[i]->node = rawScene.nodes[0];
		objectIDs[i] = renderer->RegisterObject(*fbxObjects[i]);

		float scale = 0.002f;
		fbxObjects[i]->transform.SetScale(scale, scale, scale);
		fbxObjects[i]->transform.SetPosition((float)((-(fbxObjectCount / 2) + i) * 4), 0.f, 1.f);
	}

	flt::Transform cubeTransform;
	flt::RendererObject renderable(cubeTransform, isDraw);
	renderable.node = &cubeNode;
	renderable.name = L"cube";
	auto objectID1 = renderer->RegisterObject(renderable);
	renderable.transform.SetPosition(0.0f, 0.0f, 0.0f);

	int loopCount = 0;
	while (true)
	{
		if (!platform.Update())
		{
			break;
		}

		renderer->Render(0.1f);
		//fbxObject.transform.AddLocalRotation({ 0.0f, 1.0f, 0.0f }, 0.01f);
		renderable.transform.AddLocalRotation({ 0.0f, 1.0f, 0.0f }, -0.1f);

		//fbxObject.transform.AddLocalPosition(0.0f, 0.0f, 0.01f);
		//renderable.transform.AddLocalPosition(0.0f, 0.0f, 0.01f);
		{
			auto keyData = platform.GetKey(flt::KeyCode::mouseLButton);
			if (keyData)
			{
				//isShowCurser = !isShowCurser;
				//platform.ShowCursor(isShowCurser);
				std::cout << "LL " << keyData.keyTime << " " << keyData.x << " " << keyData.y << std::endl;
			}
			keyData = platform.GetKey(flt::KeyCode::mouseRButton);
			float cameraSpeed = 0.2f;
			if (keyData)
			{
				keyData = platform.GetKey(flt::KeyCode::w);
				if (keyData)
				{
					std::cout << "pressed w" << std::endl;
					cameraTransform.AddLocalPosition(cameraTransform.Forward() * cameraSpeed);
				}
				keyData = platform.GetKey(flt::KeyCode::a);
				if (keyData)
				{
					cameraTransform.AddLocalPosition(cameraTransform.Right() * -cameraSpeed);
				}
				keyData = platform.GetKey(flt::KeyCode::s);
				if (keyData)
				{
					cameraTransform.AddLocalPosition(cameraTransform.Forward() * -cameraSpeed);
				}
				keyData = platform.GetKey(flt::KeyCode::d);
				if (keyData)
				{
					cameraTransform.AddLocalPosition(cameraTransform.Right() * cameraSpeed);
				}
				keyData = platform.GetKey(flt::KeyCode::q);
				if (keyData)
				{
					cameraTransform.AddWorldPosition(0.0f, -cameraSpeed, 0.0f);
				}
				keyData = platform.GetKey(flt::KeyCode::e);
				if (keyData)
				{
					cameraTransform.AddWorldPosition(0.0f, cameraSpeed, 0.0f);
				}

				keyData = platform.GetKey(flt::KeyCode::mouseRelativePos);
				if (keyData)
				{
					cameraTransform.AddWorldRotation({ 0.0f, 1.0f, 0.0f }, keyData.x * 0.01f);
					flt::Vector3f euler = cameraTransform.GetLocalRotation().GetEuler();
					euler.x = std::clamp(euler.x + flt::RadToDeg(keyData.y * 0.01f), -89.0f, 89.0f);
					cameraTransform.SetRotation(flt::Quaternion(euler));
				}
			}
		}

		if (!(loopCount < 10000))
		{
			__debugbreak();
		}
		loopCount++;
		//Sleep(1);
	}

	for (int i = 0; i < fbxObjectCount; ++i)
	{
		renderer->DeregisterObject(objectIDs[i]);
		delete fbxObjects[i];
		delete fbxTransforms[i];
	}

	renderer->DeregisterObject(objectID1);
	//renderer->DeregisterObject(objectID2);
	platform.DestroyRenderer(renderer);
	platform.Finalize();

	return 0;
}
