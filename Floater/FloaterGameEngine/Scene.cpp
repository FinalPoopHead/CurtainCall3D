#include "./include/internal/Scene.h"


flt::Scene::Scene() :
	_gameObjects(),
	_gameObjectsToEnable(),
	_componentsToEnable()
{
}

flt::Scene::~Scene()
{

}

void flt::Scene::Initialize()
{

}

void flt::Scene::Finalize()
{

}

//void flt::Scene::CreateGameObject(GameObject* gameObject)
//{
//	_gameObjects.emplace_back(gameObject);
//	if (gameObject->_isEnable)
//	{
//		_gameObjectsToEnable.emplace_back(gameObject, gameObject->_isEnable);
//
//		for (auto& component : gameObject->_components)
//		{
//			if (component == nullptr)
//			{
//				continue;
//			}
//
//			_componentsToEnable.emplace_back(component, true);
//		}
//	}
//}

std::vector<flt::GameObject*> flt::Scene::GetGameObjects(const std::wstring& name) const
{
	std::vector<GameObject*> result;
	for (const auto& object : _gameObjects)
	{
		if (object->name == name)
		{
			result.emplace_back(object);
		}
	}

	return result;
}

void flt::Scene::CallCollisionEvent()
{
	//bool nextFlag = !_collisionFlag;
	//for (auto& collisionPair : _collisionPairs)
	//{
	//	// TODO : 충돌한 페이를 구분하기 위한 키값을 만들어야 함.
	//	//uint64 key = collisionPair.obj1;
	//	auto iter = _collisionSet.find(collisionPair);
	//	if (iter == _collisionSet.end())
	//	{
	//		for (auto& component : collisionPair.obj1->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionEnter(collisionPair.collider2);
	//		}

	//		for (auto& component : collisionPair.obj2->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionEnter(collisionPair.collider1);
	//		}

	//		collisionPair.flag = nextFlag;
	//		_collisionSet.insert(collisionPair);
	//	}
	//}

	//for (auto& collisionPair : _collisionSet)
	//{
	//	if (collisionPair.flag == _collisionFlag)
	//	{
	//		for (auto& component : collisionPair.obj1->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionStay(collisionPair.collider2);
	//		}
	//		for (auto& component : collisionPair.obj2->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionStay(collisionPair.collider1);
	//		}
	//		_collisionSet.erase(collisionPair);
	//	}
	//	else
	//	{
	//		for (auto& component : collisionPair.obj1->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionExit(collisionPair.collider2);
	//		}
	//		for (auto& component : collisionPair.obj2->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionExit(collisionPair.collider1);
	//		}

	//		_collisionSet.erase(collisionPair);
	//	}
	//}

	//_collisionFlag = nextFlag;
	//_collisionPairs.clear();
}

void flt::Scene::Collision(GameObject* gameObject)
{

}

void flt::Scene::DestroyGameObject(GameObject& gameObject)
{
	ASSERT(false, "Not implemented yet");
}

void flt::Scene::PrePhysicsUpdate()
{
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		//object->FixedUpdate();

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->PrePhysics();
		}
	}
}

void flt::Scene::PostPhysicsUpdate()
{
	// Collider Event
	CallCollisionEvent();
	
	// postPhysics
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->PostPhysics();
		}
	}

	// FixedUpdate
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->FixedUpdate();
		}
	}
}

void flt::Scene::Update(float deltaSecond)
{
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		object->PreUpdate(deltaSecond);

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->PreUpdate(deltaSecond);
		}
	}

	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		object->Update(deltaSecond);

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->Update(deltaSecond);
		}
	}

	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		object->PostUpdate(deltaSecond);

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->PostUpdate(deltaSecond);
		}
	}
}

void flt::Scene::EndRender()
{
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->EndDraw();
		}
	}
}

void flt::Scene::StartFrame()
{
	for (auto& [object, isEnable] : _gameObjectsToEnable)
	{
		//이미 이 전 상태와 같다면 패스.
		if (object->_isEnable == isEnable)
		{
			continue;
		}

		object->_isEnable = isEnable;

		if (isEnable)
		{
			// 게임 오브젝트가 활성화 되면서 컴포넌츠들 활성화.
			for (auto& component : object->_components)
			{
				if (component == nullptr)
				{
					continue;
				}

				if (component->_isEnable)
				{
					component->OnEnable();
				}
			}
		}
		else
		{
			// 게임 오브젝트가 활성화 되면서 컴포넌츠들 활성화.
			for (auto& component : object->_components)
			{
				if (component == nullptr)
				{
					continue;
				}

				if (component->_isEnable)
				{
					component->OnDisable();
				}
			}
		}
	}

	_gameObjectsToEnable.clear();

	for (auto& [component, isEnable] : _componentsToEnable)
	{
		if (component->_isEnable != isEnable)
		{
			component->_isEnable = isEnable;
			if (isEnable)
			{
				component->OnEnable();
			}
			else
			{
				component->OnDisable();
			}
		}
	}

	_componentsToEnable.clear();
}
