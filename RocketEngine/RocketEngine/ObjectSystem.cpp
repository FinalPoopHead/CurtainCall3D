#include <algorithm>
#include <cassert>

#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "ResourceSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "DynamicModelRenderer.h"
#include "MeshRenderer.h"
#include "../RocketCommon/RawModelStruct.h"

namespace Rocket::Core
{
	ObjectSystem::ObjectSystem()
	{

	}

	void ObjectSystem::Initialize()
	{
		_enableList.clear();
		_disableList.clear();
	}

	void ObjectSystem::Finalize()
	{
		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Finalize();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->Finalize();
		}

		for (auto& iter : SceneSystem::Instance().GetAllScenes())
		{
			for (auto& gameObject : iter.second->GetOriginalList())
			{
				gameObject->Finalize();
			}
		}
	}

	void ObjectSystem::StartCurrentScene()
	{
		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		//assert(currentScene);

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Start();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			if (!staticComponent->_isStarted)
			{
				staticComponent->Start();
				staticComponent->_isStarted = true;
			}
		}

		currentScene->Start();
	}

	void ObjectSystem::UpdateCurrentScene()
	{
		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Update();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->Update();
		}

		currentScene->Update();
	}

	void ObjectSystem::LateUpdateCurrentScene()
	{
		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->LateUpdate();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->LateUpdate();
		}

		currentScene->LateUpdate();
	}

	void ObjectSystem::UpdateScene(Rocket::Scene* scene)
	{

	}

	void ObjectSystem::FlushEnable()
	{
		for (auto obj : _enableList)
		{
			obj->FlushEnable();
		}

		_enableList.clear();
	}

	void ObjectSystem::FlushDisable()
	{
		for (auto obj : _disableList)
		{
			obj->FlushDisable();
		}

		_disableList.clear();
	}

	void ObjectSystem::AddEnable(Rocket::GameObject* obj)
	{
		_enableList.push_back(obj);
	}

	void ObjectSystem::AddDisable(Rocket::GameObject* obj)
	{
		_disableList.push_back(obj);
	}

	Rocket::GameObject* ObjectSystem::CreateObject(const std::string& objName)
	{
		Rocket::GameObject* gameObject = new Rocket::GameObject(objName);
		return gameObject;
	}

	Rocket::GameObject* ObjectSystem::CreateStaticObject(const std::string& objName)
	{
		Rocket::GameObject* gameObject = new Rocket::GameObject(objName);
		_staticObjList.push_back(gameObject);
		return gameObject;
	}

	Rocket::GameObject* ObjectSystem::CreateModelObject(const std::string& fileName)
	{
		RawModel* rawModel = ResourceSystem::Instance().GetModel(fileName);
		Rocket::GameObject* resultGameObject = nullptr;

		if (0 < rawModel->animations.size())
		{
			// 애니메이션이 있다면 (dynamic)
			Rocket::DynamicModelRenderer* modelRenderer = nullptr;
			resultGameObject = CreateDynamicModelObjectRecur(rawModel->rootNode, &modelRenderer);
			modelRenderer->LoadModel(fileName);	// 구조가 다 만들어지고 나서 LoadModel을 해야 Transform 계층이 적용된다.
		}
		else
		{
			// 애니메이션이 없다면 (static)
			resultGameObject = CreateStaticMeshObjectRecur(rawModel->rootNode);
		}

		resultGameObject->objName = fileName;

		return resultGameObject;
	}

	Rocket::GameObject* ObjectSystem::CreateStaticMeshObjectRecur(RawNode* node)
	{
		Rocket::GameObject* gameObject = new Rocket::GameObject(node->name);

		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		node->transformMatrix.Decompose(scale, rotation, position);
		gameObject->transform.SetLocalPosition(position);
		gameObject->transform.SetLocalRotation(rotation);
		gameObject->transform.SetLocalScale(scale);

		int index = 0;	//이름이 겹치는 경우를 대비.

		for (auto& rawMesh : node->meshes)
		{
			Rocket::MeshRenderer* meshRenderer = gameObject->AddComponent<Rocket::MeshRenderer>();
			meshRenderer->SetMesh(rawMesh->name + std::to_string(index));
			index++;
			// TODO : Texture도 자동으로 같이 Set 해주면 좋을 거 같긴 한데..
		}

		for (auto& child : node->children)
		{
			Rocket::GameObject* childObject = CreateStaticMeshObjectRecur(child);
			childObject->transform.SetParent(gameObject);
		}

		return gameObject;
	}

	Rocket::GameObject* ObjectSystem::CreateDynamicModelObjectRecur(RawNode* node, Rocket::DynamicModelRenderer** outModelRenderer)
	{
		Rocket::GameObject* gameObject = new Rocket::GameObject(node->name);

		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		node->transformMatrix.Decompose(scale, rotation, position);
		gameObject->transform.SetLocalPosition(position);
		gameObject->transform.SetLocalRotation(rotation);
		gameObject->transform.SetLocalScale(scale);

		if (0 < node->meshes.size())
		{
			*outModelRenderer = gameObject->AddComponent<Rocket::DynamicModelRenderer>();
		}

		for (auto& child : node->children)
		{
			Rocket::GameObject* childObject = CreateDynamicModelObjectRecur(child, outModelRenderer);
			childObject->transform.SetParent(gameObject);
		}

		return gameObject;
	}

	void ObjectSystem::AddStaticComponent(Rocket::Component* component)
	{
		if (std::find(_staticComponentList.begin(), _staticComponentList.end(), component) != _staticComponentList.end())
		{
			return;
		}

		_staticComponentList.push_back(component);
	}
}
