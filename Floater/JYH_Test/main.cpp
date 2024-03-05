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

#include "../ZeldaGraphicsAdapter/include/IZeldaRendererAdapter.h"

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

int main()
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	flt::RawScene rawScene;

#pragma region 테스트
	{
		using namespace flt;
		using namespace flt::test;

		//TesterRBTree tester;
		//tester.Test();

		Quaternion q1{ 0.0f, 0.0f, 0.0f, 1.0f };
		q1.SetEuler(1.f, 2.f, 3.f);

		Matrix4f m1;

		ModelLoader loader;
		//std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Ganondorf (TotK).fbx";
		std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying.fbx";
		std::wstring zUpYForward = L"..\\x64\\fbx\\Test\\ZY.fbx";
		std::wstring yUpZForward = L"..\\x64\\fbx\\Test\\YZ.fbx";
		std::wstring xUpYForward = L"..\\x64\\fbx\\Test\\XY.fbx";

		char buffer[1024];
		auto str = ConvertToString(L"..\\x64\\fbx\\Test\\*.*");

		WIN32_FIND_DATAA findData;
		HANDLE handle = FindFirstFileA(str.c_str(), &findData);

		if (handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::cout << findData.cFileName << std::endl;
			} while (FindNextFileA(handle, &findData));
		}

		GetFullPathNameA(str.c_str(), 1024, buffer, NULL);

		//std::filesystem::path currPath = std::filesystem::current_path();

		//loader.Load(L"..\\x64\\fbx\\PBR_BasicShader.blend");


		loader.Load(filePath, &rawScene);
		//loader.Load(zUpYForward);
		//loader.Load(yUpZForward);
		//loader.Load(xUpYForward);

	}
#pragma endregion

	//auto adpaterTest = new IZeldaRendererAdapter();
	//adpaterTest->Initialize(1280, 720, false, NULL, false, 0.0f, 0.0f);

	flt::Platform platform{ true };
	platform.Initialize(1280, 720, L"title", L".\\path");

	auto renderer = platform.CreateRenderer(flt::RendererType::DX11);

	bool isShowCurser = true;

	bool isDraw = true;

	flt::RawNode cameraNode(L"testCamera");
	cameraNode.transform.SetPosition(0.0f, 0.0f, 0.0f);
	cameraNode.transform.SetScale(1.0f, 1.0f, 1.0f);
	cameraNode.transform.SetRotation(0.0f, 0.0f, 0.0f);
	cameraNode.camera = new flt::Camera(&cameraNode.transform);

	flt::RendererObject cameraObject(cameraNode, isDraw, L"testCamera");
	auto cameraID = renderer->RegisterObject(cameraObject);


	flt::RawNode cubeNode(L"testNode");
	cubeNode.transform.SetPosition(0.0f, 0.0f, 0.7f);
	cubeNode.transform.SetScale(0.3f, 0.3f, 0.3f);

	flt::RendererObject fbxObject(*rawScene.nodes[1], isDraw, L"test1");
	auto objectID0 = renderer->RegisterObject(fbxObject);
	rawScene.nodes[1]->transform.SetScale(1.f, 1.f, 1.f);
	rawScene.nodes[1]->transform.SetPosition(0.f, 0.f, 30.f);

	flt::RendererObject renderable(cubeNode, isDraw, L"cube");
	auto objectID1 = renderer->RegisterObject(renderable);
	cubeNode.transform.SetPosition(0.0f, 0.0f, 0.0f);

	while (true)
	{
		if (!platform.Update())
		{
			break;
		}

		renderer->Render(1.0f);
		rawScene.nodes[1]->transform.AddLocalRotation({ 0.0f, 1.0f, 0.0f }, 0.01f);
		cubeNode.transform.AddLocalRotation({ 0.0f, 1.0f, 0.0f }, -0.1f);
		{
			auto keyData = platform.GetKey(flt::KeyCode::mouseLButton);
			if (keyData)
			{
				//isShowCurser = !isShowCurser;
				//platform.ShowCursor(isShowCurser);
				std::cout << "LL " << keyData.keyTime << " " << keyData.x << " " << keyData.y << std::endl;
			}
			keyData = platform.GetKey(flt::KeyCode::mouseRButton);
			float cameraSpeed = 0.1f;
			if (keyData)
			{
				keyData = platform.GetKey(flt::KeyCode::w);
				if (keyData)
				{
					std::cout << "pressed w" << std::endl;
					cameraNode.transform.AddLocalPosition(cameraNode.transform.Forward() * cameraSpeed);
				}
				keyData = platform.GetKey(flt::KeyCode::a);
				if (keyData)
				{
					cameraNode.transform.AddLocalPosition(cameraNode.transform.Right() * -cameraSpeed);
				}
				keyData = platform.GetKey(flt::KeyCode::s);
				if (keyData)
				{
					cameraNode.transform.AddLocalPosition(cameraNode.transform.Forward() * -cameraSpeed);
				}
				keyData = platform.GetKey(flt::KeyCode::d);
				if (keyData)
				{
					cameraNode.transform.AddLocalPosition(cameraNode.transform.Right() * cameraSpeed);
				}
				keyData = platform.GetKey(flt::KeyCode::q);
				if (keyData)
				{
					cameraNode.transform.AddWorldPosition(0.0f, cameraSpeed, 0.0f);
				}
				keyData = platform.GetKey(flt::KeyCode::e);
				if (keyData)
				{
					cameraNode.transform.AddWorldPosition(0.0f, -cameraSpeed, 0.0f);
				}

				keyData = platform.GetKey(flt::KeyCode::mouseRelativePos);
				if (keyData)
				{
					cameraNode.transform.AddWorldRotation({ 0.0f, 1.0f, 0.0f }, keyData.x * 0.01f);
					flt::Vector3f euler = cameraNode.transform.GetLocalRotation().GetEuler();
					euler.x = std::clamp(euler.x + flt::RadToDeg(keyData.y * 0.01f), -89.0f, 89.0f);
					cameraNode.transform.SetRotation(flt::Quaternion(euler));
				}
			}
		}

		Sleep(10);
	}

	renderer->DeregisterObject(objectID0);

	//renderer->DeregisterObject(objectID1);
	//renderer->DeregisterObject(objectID2);
	platform.DestroyRenderer(renderer);
	platform.Finalize();

	return 0;
}
