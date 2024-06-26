﻿#include "UnityLoadScene.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495)
#include "../External/include/rapidjson/document.h"
#pragma warning(pop)
#include "../FloaterUtil/include/ConvString.h"
#include <filesystem>
#include <fstream>
#include <iostream>

#include "GlideObject.h"
//#include "../FloaterGameEngine/include/internal/BuiltinComponent.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"

// 테스트 include
#include "RotateComponent.h"


UnityLoadScene::UnityLoadScene(const std::wstring& jsonPath) : _jsonPath(jsonPath)
{
}

void UnityLoadScene::Initialize()
{
	//----- 테스트 코드 -----
	//{
	//	flt::GameObject* testObject = CreateGameObject<flt::GameObject>();

	//	std::wstring filePath = L"..\\x64\\fbx\\SM_Box_Cargo.fbx";

	//	flt::RendererComponent* renderer = testObject->AddComponent<flt::RendererComponent>();

	//	flt::ModelLoader loader;
	//	flt::RawScene rawScene;
	//	loader.Load(filePath, &rawScene);

	//	flt::RawNode* node = rawScene.nodes[0];

	//	renderer->SetRawNode(node);
	//}
	//----------------------

	LoadUnityJson();

	GlideObject* glideObject = flt::CreateGameObject<GlideObject>(true);
	//glideObject->tr.SetPosition(0.0f, 50.0f, -50.0f);
	glideObject->tr.SetPosition(0.0f, 250.0f, -250.0f);
}

void UnityLoadScene::Finalize()
{

}

struct MyHandler
{

};

void UnityLoadScene::LoadUnityJson()
{
	ASSERT(std::filesystem::exists(_jsonPath), "File not found");

	std::ifstream inputFile(_jsonPath);
	ASSERT(inputFile.is_open(), "fail open file");

	rapidjson::Document document;

	std::stringstream buffer;
	buffer << inputFile.rdbuf();
	std::string josnData = buffer.str();
	inputFile.close();
	document.Parse(josnData.c_str());
	ASSERT(document.HasParseError() == rapidjson::kParseErrorNone, "Parse error");

	if (document.IsObject())
	{
		if (document.HasMember("Name"))
		{
			std::cout << "Name: " << document["Name"].GetString() << std::endl;
		}
	}
	else if (document.IsArray())
	{
		for (auto& v : document.GetArray())
		{
			flt::GameObject* gameObject = flt::CreateGameObject<flt::GameObject>(true);
			//gameObject->AddComponent<RotateComponent>();

			if (v.HasMember("ID"))
			{
				int id = v["ID"].GetInt();
				gameObject->name = flt::ToWstring(id);
			}

			if (v.HasMember("Name"))
			{
				std::string fileName = v["Name"].GetString();
				fileName += ".fbx";
				std::wstring filePath = L"..\\x64\\fbx\\";
				filePath += flt::ToWstring(fileName);

				flt::RendererComponent* renderer = gameObject->AddComponent<flt::RendererComponent>(true);

				//flt::ModelLoader loader;
				//flt::RawScene rawScene;
				//loader.Load(filePath, &rawScene);
				//flt::RawNode* node = rawScene.nodes[0];

				//renderer->SetRawNode(node);
				renderer->SetFilePath(filePath);
			}

			if (v.HasMember("Position"))
			{
				auto& pos = v["Position"];

				gameObject->tr.SetPosition(-pos["x"].GetFloat(), pos["y"].GetFloat(), -pos["z"].GetFloat());
				//gameObject->tr.SetPosition(pos["x"].GetFloat(), pos["y"].GetFloat(), pos["z"].GetFloat());
			}

			if (v.HasMember("Rotation"))
			{
				auto& rot = v["Rotation"];

				gameObject->tr.SetRotation(rot["x"].GetFloat(), rot["y"].GetFloat(), rot["z"].GetFloat(), rot["w"].GetFloat());
				//gameObject->tr.AddLocalRotation({ 0.0f, 1.0f, 0.0f }, flt::PI<float>);
				//gameObject->tr.SetRotation(rot["x"].GetFloat(), rot["y"].GetFloat(), rot["z"].GetFloat(), rot["w"].GetFloat());
			}

			if (v.HasMember("Scale"))
			{
				auto& scl = v["Scale"];

				gameObject->tr.SetScale(scl["x"].GetFloat(), scl["y"].GetFloat(), scl["z"].GetFloat());
			}

			if (v.HasMember("ColliderType"))
			{
				int colliderType = v["ColliderType"].GetInt();

				if (colliderType == 1) // Box
				{
					flt::BoxColliderComponent* collider = gameObject->AddComponent<flt::BoxColliderComponent>(true);

					if (v.HasMember("BoxColliderSize"))
					{
						auto& size = v["BoxColliderSize"];
						collider->SetSize({size["x"].GetFloat(), size["y"].GetFloat(), size["z"].GetFloat()});
					}

					if (v.HasMember("ColliderCenter"))
					{
						auto& center = v["ColliderCenter"];
						float x = center["x"].GetFloat();
						float y = center["y"].GetFloat();
						float z = center["z"].GetFloat();

						collider->SetOffset({-center["x"].GetFloat(), center["y"].GetFloat(), -center["z"].GetFloat()});
					}
				}
				else if (colliderType == 2) // Sphere
				{
					//gameObject->AddComponent<flt::SphereCollider>();
				}
			}


		}

		// 계층구조 연결해주기
		for (auto& v : document.GetArray())
		{
			if (!v.HasMember("Name"))
			{
				continue;
			}

			int id = v["ID"].GetInt();
			std::wstring name = flt::ToWstring(id);

			if (!v.HasMember("ParentID"))
			{
				continue;
			}

			int parentId = v["ParentID"].GetInt();
			std::wstring parentName = flt::ToWstring(parentId);

			if (parentId == 0)
			{
				continue;
			}

			auto parent = GetGameObjects(parentName);
			ASSERT(parent.size() == 1, "Parent not found");
			auto child = GetGameObjects(name);
			ASSERT(child.size() == 1, "Child not found");

			parent[0]->tr.AddChild(&child[0]->tr);
		}
	}
	else
	{
		std::cout << "Invalid json file" << std::endl;
	}
}
