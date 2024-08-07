﻿#pragma once
#include "GameObject.h"
#include "Component.h"
#include "../../../FloaterUtil/include/FLTween.h"
#include "../../CollisionPair.h"
#include "../../../FloaterUtil/include/Timer.h"
#include "../../../FloaterUtil/include/SparseSet.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <set>
#include <string>

namespace flt
{
	template<GameObjectDerived T, typename... TArgs>
	T* CreateGameObject(bool isEnabled, TArgs&&... args);

	class Scene
	{
		friend class GameObject;
		friend class GameEngine;
		friend class ComponentBase;

		template<GameObjectDerived T, typename... TArgs>
		friend T* flt::CreateGameObject(bool isEnabled, TArgs&&... args);

	public:
		Scene();
		~Scene();

		virtual void Initialize() {}
		virtual void Finalize() {}

		//[[deprecated("Use template CreateGameObject instead")]] void CreateGameObject(GameObject* gameObject);
		void PrePhysicsUpdate();
		void PostPhysicsUpdate();
		void Update(float deltaSecond);
		void PreRender();
		void PostRender();
		void StartFrame();
		void EndFrame();

		void StartScene();
		void EndScene();

		void AddTween(IFLTween* tween);
		void AddPosTween(FLTween<Vector4f>* tween, Transform* tr);
		void AddScaleTween(FLTween<Vector4f>* tween, Transform* tr);
		void AddRotTween(FLTween<Quaternion>* tween, Transform* tr);
		void StartTween(IFLTween* tween);
		void StopTween(IFLTween* tween);
		void ReleaseTween(IFLTween* tween);
		bool IsActiveTween(IFLTween* tween);

		//std::vector<GameObject*> GetGameObjects() const { return _gameObjects; }
		std::vector<GameObject*> GetGameObjects(const std::wstring& name) const;

	private:
		void AddEnableGameObject(GameObject* gameObject, bool isEnable);
		//void AddEnableComponent(ComponentBase* component, bool isEnable);
		void AddDestroyGameObject(GameObject* gameObject);

		void TweenUpdate(float deltaSecond);
		void DeleteTweensDeferred();
		void ClearTweens();

	private:
		template<GameObjectDerived T, typename... TArgs>
		T* InstantiateGameObject(bool isEnabled, TArgs&&... args);

		void CallCollisionEvent();

		void DestroyGameObjectRecursive(GameObject* gameObject);

	private:
		SparseSet<GameObject*> _gameObjects;
		SparseSet<GameObject*> _activeGameObjects;
		std::unordered_map<GameObject*, bool> _stagingActiveGameObjects;
		std::vector<GameObject*> _gameObjectsToCreate;
		std::vector<GameObject*> _gameObjectsToDestroy;
		std::vector<GameObject*> _gameObjectsToDelete;

		std::vector<CollisionPair> _collisionPairs;
		//std::unordered_set<CollisionPair> _collisionSet;
		bool _collisionFlag;
		bool _isActive;

		SparseSet<IFLTween*> _tweens;
		SparseSet<std::pair<FLTween<Vector4f>*, Transform*>> _posTweens;
		SparseSet<std::pair<FLTween<Vector4f>*, Transform*>> _scaleTweens;
		SparseSet<std::pair<FLTween<Quaternion>*, Transform*>> _rotTweens;

		enum class TweenType
		{
			none,
			pos,
			scale,
			rot
		};
		struct TweenInfo
		{
			TweenType type;
			uint32 sparseIndex;
			Transform* tr;
		};

		std::unordered_map<IFLTween*, TweenInfo> _tweenMap;
		std::vector<IFLTween*> _tweensToDelete;
	};

	template<GameObjectDerived T, typename... TArgs>
	T* Scene::InstantiateGameObject(bool isEnabled, TArgs&&... args)
	{
		GameObject* gameObject = new T(std::forward<TArgs>(args)...);
		gameObject->_scene = this;
		gameObject->_isEnable = isEnabled;

		_gameObjectsToCreate.emplace_back(gameObject);

		return static_cast<T*>(gameObject);
	}

	template <typename T>
	concept SceneDerived = requires(T a)
	{
		std::is_base_of_v<flt::Scene, T>;
	};
}
