#define FLT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS

#include "TestScene.h"
#include "../FloaterGameEngine/include/GameEngine.h"
#include "../FloaterPlatform/include/Platform.h"
#include "../FloaterRendererCommon/include/Transform.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/Timer.h"
#include <iostream>

#include "../FloaterUtil/include/ConvString.h"

#pragma region testInclude
//#include "../FloaterRendererCommon/include/IBuilder.h"
//#include "../FloaterRendererCommon/include/Resource.h"
//#include "../FloaterRendererCommon/include/ResourceMgr.h"
//#include "../FloaterUtil/TesterRBTree.h"
//
//#include "../FloaterUtil/TesterFixedSizeMemoryPool.h"
//#include "../FloaterUtil/include/Hash.h"
//
//#include "../FloaterRendererCommon/include/Camera.h"
//
//#include "../FloaterRendererCommon/include/ModelLoader.h"
//#include "../FloaterRendererCommon/CoordSystem.h"
//#include <unordered_map>
//#include <filesystem>
//
//#include "../FloaterMath/include/Common.h"
//#include "../FloaterMath/include/Vector2f.h"
//#include "../FloaterMath/include/Vector3f.h"
//
//#include <windows.h>
//
//#include "../FloaterGameEngine/include/Component.h"

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

struct Component1 : public flt::Component<Component1>
{
	int a;
};

struct Component2 : public flt::Component<Component2>
{
	int b;
};

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	//cpu 캐시 크기 가져오기
	std::cout << std::hardware_destructive_interference_size << std::endl;

	float t1 = 10.0f;
	float t2 = t1 + (flt::FLOAT_EPSILON * t1);

	
	//float t2 = t1 + 1.0e-08F;

	std::cout << (t1 == t2) << std::endl;

	flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	//pGameEngine->Initialize();
	TestScene testScene;
	pGameEngine->SetScene(&testScene);


	//int loopCount = 0;
	while (true)
	{
		pGameEngine->Update();
		//if (!platform.Update())
		//{
		//	break;
		//}

		//renderer->Render(0.1f);
		////fbxObject.transform.AddLocalRotation({ 0.0f, 1.0f, 0.0f }, 0.01f);
		//renderable.transform.AddLocalRotation({ 0.0f, 1.0f, 0.0f }, -0.1f);

		////fbxObject.transform.AddLocalPosition(0.0f, 0.0f, 0.01f);
		////renderable.transform.AddLocalPosition(0.0f, 0.0f, 0.01f);
		//{
		//	auto keyData = platform.GetKey(flt::KeyCode::mouseLButton);
		//	//if (keyData)
		//	//{
		//	//	//isShowCurser = !isShowCurser;
		//	//	//platform.ShowCursor(isShowCurser);
		//	//	//std::cout << "LL " << keyData.keyTime << " " << keyData.x << " " << keyData.y << std::endl;
		//	//}

		//	//keyData = platform.GetKey(flt::KeyCode::gpadA);
		//	//if (keyData)
		//	//{
		//	//	std::cout << "gpadA " << keyData.keyTime << std::endl;
		//	//}
		//	//keyData = platform.GetKey(flt::KeyCode::gpadB);
		//	//if (keyData)
		//	//{
		//	//	std::cout << "gpadB " << keyData.keyTime << std::endl;
		//	//}
		//	//keyData = platform.GetKey(flt::KeyCode::gpadX);
		//	//if (keyData)
		//	//{
		//	//	std::cout << "gpadX " << keyData.keyTime << std::endl;
		//	//}
		//	//keyData = platform.GetKey(flt::KeyCode::gpadY);
		//	//if (keyData)
		//	//{
		//	//	std::cout << "gpadY " << keyData.keyTime << std::endl;
		//	//}
		//	//keyData = platform.GetKey(flt::KeyCode::gpadLStickClick);
		//	//if (keyData)
		//	//{
		//	//	std::cout << "gpadLStickClick " << keyData.keyTime << std::endl;
		//	//}

		//	flt::GamePadState state;
		//	for (int i = 0; i < 16; ++i)
		//	{
		//		if (platform.GetGamePadState(i, &state))
		//		{
		//			printf("[%d] ", i);
		//			printf("Buttons=%s%s%s%s %s %s",
		//				(state.buttons & flt::GamePadState::A) ? "A" : " ",
		//				(state.buttons & flt::GamePadState::B) ? "B" : " ",
		//				(state.buttons & flt::GamePadState::X) ? "X" : " ",
		//				(state.buttons & flt::GamePadState::Y) ? "Y" : " ",
		//				(state.buttons & flt::GamePadState::BACK) ? "BACK" : "    ",
		//				(state.buttons & flt::GamePadState::START) ? "START" : "     ");
		//			printf("Dpad=%s%s%s%s ",
		//				(state.buttons & flt::GamePadState::UP) ? "U" : " ",
		//				(state.buttons & flt::GamePadState::DOWN) ? "D" : " ",
		//				(state.buttons & flt::GamePadState::LEFT) ? "L" : " ",
		//				(state.buttons & flt::GamePadState::RIGHT) ? "R" : " ");
		//			printf("Shoulders=%s%s ",
		//				(state.buttons & flt::GamePadState::LB) ? "L" : " ",
		//				(state.buttons & flt::GamePadState::RB) ? "R" : " ");
		//			printf("Thumb=%s%s ",
		//				(state.buttons & flt::GamePadState::LSTICK) ? "L" : " ",
		//				(state.buttons & flt::GamePadState::RSTICK) ? "R" : " ");
		//			printf("lStick=(% 0.3f,% 0.3f) ", (state.lStickX), (state.lStickY));
		//			printf("rStick=(% 0.3f,% 0.3f) ", (state.rStickX), (state.rStickY));
		//			printf("Trigger=(% 0.3f,% 0.3f) ", (state.lTrigger), (state.rTrigger));
		//			printf("\n");

		//			//platform.SetGamePadVibration(i, state.lTrigger, state.rTrigger);
		//			float vibPower = state.lStickX;
		//			platform.SetGamePadVibration(i, -vibPower, vibPower);
		//		}
		//	}

		//	keyData = platform.GetKey(flt::KeyCode::mouseRButton);
		//	float cameraSpeed = 0.2f;
		//	if (keyData)
		//	{
		//		keyData = platform.GetKey(flt::KeyCode::w);
		//		if (keyData)
		//		{
		//			std::cout << "pressed w" << std::endl;
		//			cameraTransform.AddLocalPosition(cameraTransform.Forward() * cameraSpeed);
		//		}
		//		keyData = platform.GetKey(flt::KeyCode::a);
		//		if (keyData)
		//		{
		//			cameraTransform.AddLocalPosition(cameraTransform.Right() * -cameraSpeed);
		//		}
		//		keyData = platform.GetKey(flt::KeyCode::s);
		//		if (keyData)
		//		{
		//			cameraTransform.AddLocalPosition(cameraTransform.Forward() * -cameraSpeed);
		//		}
		//		keyData = platform.GetKey(flt::KeyCode::d);
		//		if (keyData)
		//		{
		//			cameraTransform.AddLocalPosition(cameraTransform.Right() * cameraSpeed);
		//		}
		//		keyData = platform.GetKey(flt::KeyCode::q);
		//		if (keyData)
		//		{
		//			cameraTransform.AddWorldPosition(0.0f, -cameraSpeed, 0.0f);
		//		}
		//		keyData = platform.GetKey(flt::KeyCode::e);
		//		if (keyData)
		//		{
		//			cameraTransform.AddWorldPosition(0.0f, cameraSpeed, 0.0f);
		//		}

		//		keyData = platform.GetKey(flt::KeyCode::mouseRelativePos);
		//		if (keyData)
		//		{
		//			cameraTransform.AddWorldRotation({ 0.0f, 1.0f, 0.0f }, keyData.x * 0.01f);
		//			flt::Vector3f euler = cameraTransform.GetLocalRotation().GetEuler();
		//			euler.x = std::clamp(euler.x + flt::RadToDeg(keyData.y * 0.01f), -89.0f, 89.0f);
		//			cameraTransform.SetRotation(flt::Quaternion(euler));
		//		}
		//	}
		//}

		//if (!(loopCount < 10000))
		//{
		//	//__debugbreak();
		//}
		//loopCount++;
		//Sleep(1000);
	}

	pGameEngine->Finalize();

	//for (int i = 0; i < fbxObjectCount; ++i)
	//{
	//	renderer->DeregisterObject(objectIDs[i]);
	//	delete fbxObjects[i];
	//	delete fbxTransforms[i];
	//}

	//renderer->DeregisterObject(objectID1);
	////renderer->DeregisterObject(objectID2);
	//platform.DestroyRenderer(renderer);
	//platform.Finalize();

	return 0;
}
