#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class GameManagerController : public flt::Component<GameManagerController>
{
public:
	GameManagerController();
	~GameManagerController();

public:
	virtual void OnEnable() override;		// 사실상 초기화
	virtual void Update(float deltaSecond) override;
};
