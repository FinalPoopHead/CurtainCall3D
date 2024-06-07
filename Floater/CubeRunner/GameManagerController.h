#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class GameManger;

class GameManagerController : public flt::Component<GameManagerController>
{
public:
	GameManagerController();
	~GameManagerController();

public:
	virtual void OnCreate() override;		// 사실상 초기화
	virtual void OnDestroy() override;		// 사실상 해제	
	virtual void Update(float deltaSecond) override;

private:
	//GameManager& _gameManager;
};
