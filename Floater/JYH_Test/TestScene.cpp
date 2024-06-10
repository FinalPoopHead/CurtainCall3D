#include "TestScene.h"
#include "TestGameObejct.h"
#include "GlideObject.h"

// 오브젝트 배치하는 코드가 들어가야 함.
void TestScene::Initialize()
{
	//TestGameObejct* testObject = new TestGameObejct();
	//AddGameObject(testObject);

	GlideObject* glideObject = flt::CreateGameObject<GlideObject>(true);
	glideObject->tr.SetPosition(0.0f, 2.0f, -5.0f);
	glideObject->tr.LookAt(0.0f, 0.0f, 0.0f);

	GlideObject* glideObject2 = flt::CreateGameObject<GlideObject>(true);
	glideObject2->tr.SetPosition(0.0f, 2.0f, -10.0f);
	glideObject2->tr.LookAt(0.0f, 0.0f, 0.0f);

	/*TestGameObejct* tObject = flt::CreateGameObject<TestGameObejct>(true);
	tObject->tr.SetPosition(0.0f, 201.0f, 0.0f);*/

	//TestGameObejct* tObject2 = flt::CreateGameObject<TestGameObejct>(true);
	//tObject2->tr.SetPosition(0.0f, 0.0f, 0.0f);

	flt::GameObject* tObject3 = flt::CreateGameObject<flt::GameObject>(true);
	flt::RendererComponent* tRenderer = tObject3->AddComponent<flt::RendererComponent>(true);
	tRenderer->SetImage(L"..\\Resources\\abcd.jpg");
	tObject3->tr.SetPosition(0.0f, 0.0f, 0.0f);
}
 
void TestScene::Finalize()
{

}
