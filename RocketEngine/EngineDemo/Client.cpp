#include "Client.h"
#include "..\\RocketEngine\\RocketAPI.h"

#include "TestScene.h"

void Client::Initialize()
{
	TestScene testScene;
	testScene.Initialize();

	Rocket::LoadScene("TEST");
}
