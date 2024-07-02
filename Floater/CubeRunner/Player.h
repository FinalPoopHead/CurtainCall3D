#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Board;
class Camera;
class PlayerModel;

class Player : public flt::GameObject
{
public:
	Player(Board* board);
	~Player() {}

protected:
	virtual void OnEnable() override;
	virtual void Update(float deltaSecond) override;

public:
	void SetPadIndex(int padIndex) { _padIndex = padIndex; }
	void SetGameOver();
	void SetAlbedoPath(std::wstring path);
	void SetPositionToRatioPosition(float ratioX, float ratioY);

public:
	Camera* camera;

private:
	PlayerModel* _model;
	Board* _board;
	bool _isGameOver;
	bool _isCrushed;
	int _padIndex;
	float _speed;
};
