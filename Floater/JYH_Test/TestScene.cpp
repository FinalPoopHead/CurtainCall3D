#include "TestScene.h"
#include "TestGameObejct.h"
#include "GlideObject.h"


void TestScene::Initialize()
{
	//TestGameObejct* testObject = new TestGameObejct();
	//AddGameObject(testObject);

	GlideObject* glideObject = CreateGameObject<GlideObject>();
	CreateGameObject<TestGameObejct>();

	glideObject->tr.SetPosition(0.0f, 1000.0f, -400.0f);
}

void TestScene::Finalize()
{

}
