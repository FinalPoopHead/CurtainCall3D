#pragma once

namespace RocketEngine
{
	class Scene;
}

class GameWaitingSceneView
{
public:
	GameWaitingSceneView();

public:
	void Initialize();
public:
	RocketEngine::Scene* scene;
};

