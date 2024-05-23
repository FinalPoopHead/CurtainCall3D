#include "TestScene.h"
#include "TestGameObejct.h"
#include "GlideObject.h"

// 오브젝트 배치하는 코드가 들어가야 함.
void TestScene::Initialize()
{
	//TestGameObejct* testObject = new TestGameObejct();
	//AddGameObject(testObject);

	GlideObject* glideObject = CreateGameObject<GlideObject>();
	glideObject->tr.SetPosition(0.0f, 10.0f, -50.0f);

	TestGameObejct* tObject = CreateGameObject<TestGameObejct>();
	tObject->tr.SetPosition(0.0f, 10.0f, 0.0f);
}

void TestScene::Finalize()
{

}
