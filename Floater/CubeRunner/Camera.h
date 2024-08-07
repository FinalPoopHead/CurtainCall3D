﻿#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include <functional>

enum class eCameraState
{
	NONE
	, TRACEPLAYER
	, LOOKGENERATING
	, FALL
	, TWEEN
};

class Player;
class Board;
class ShakeComponent;
class PlayerModel;

class Camera : public flt::GameObject
{
public:
	Camera(Player* player, Board* board);

	void StopCamera();
	void TracePlayer();
	void LookGenerating();
	void TraceFalling();
	void TweenMove(flt::Vector3f targetPos, float time, std::function<float(float)> ease = [](float t) { return t; });
	void TweenRotate(flt::Quaternion targetRot, float time, std::function<float(float)> ease = [](float t) { return t; });
	flt::Vector3f ToScreenSpace(flt::Vector3f pos);

	void SetCameraIndex(uint32 index);

	ShakeComponent* GetShakeComponent() { return _shakeComp; }

protected:
	virtual void PostUpdate(float deltaSecond) override;

private:
	flt::Quaternion CalcTargetRotation();
	flt::Vector3f CalcTargetPosition();

	bool CheckCameraMove();
	void StartCameraMove();
	void UpdateCameraMove(float deltaSecond);

private:
	flt::CameraComponent* _cameraComp;
	ShakeComponent* _shakeComp;
	Player* _player;
	Board* _board;
	float _height;
	float _playHeight;
	float _lookZOffset;
	float _posZOffset;
	float _playerDistance;
	float _lookDegree;
	float _movSpeed;
	float _rotSpeed;
	bool _isCameraDropCheck = false;

	eCameraState _state;

	flt::Quaternion _currRotation;
	flt::Vector3f _currPosition;

	// Tween
	bool _isTweenMove;
	flt::Vector3f _startPosition;
	flt::Vector3f _targetPosition;
	float _rotateTime;
	float _rotateTimeElapsed;
	std::function<float(float)> _rotateEase;

	bool _isTweenRotate;
	flt::Quaternion _startRotation;
	flt::Quaternion _targetRotation;
	float _moveTime;
	float _moveTimeElapsed;
	std::function<float(float)> _moveEase;
};
