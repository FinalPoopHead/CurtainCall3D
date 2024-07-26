#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class MineCone : public flt::GameObject
{
public:
	MineCone(GameObject* parentMine, const std::wstring& texturePath);

protected:
	virtual void Update(float deltaTime) override;
	virtual void OnEnable() override;
private:
	float _moveSpeed;
	float _acceleration;
	float _coneHeight;
};
