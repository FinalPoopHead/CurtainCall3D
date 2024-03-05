#pragma once

namespace Rocket
{
	class Scene;
}

class TestScene
{
public:
	TestScene();

public:
	void Initialize();

public:
	Rocket::Scene* scene;

private:
	//RocketAPILoader& _rocket;
};
