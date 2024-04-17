#include "TestScene.h"
#include "TestGameObejct.h"

void TestScene::Initialize()
{
	//TestGameObejct* testObject = new TestGameObejct();
	//AddGameObject(testObject);

	AddGameObject<TestGameObejct>();
}

void TestScene::Finalize()
{

}
