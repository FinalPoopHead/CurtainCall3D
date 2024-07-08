#include "Player.h"
#include "../FloaterGameEngine/include/Input.h"
#include "Camera.h"
#include "Board.h"
#include "PlayerModel.h"


//TEST Include
#include "MainMenuScene.h"

Player::Player(Board* board)
	: camera(nullptr)
	, _model(nullptr)
	, _board(board)
	, _isGameOver(false)
	, _isCrushed(false)
	, _padIndex(-1)
	, _speed(10.0f)
{
	_model = flt::CreateGameObject<PlayerModel>(true);
	tr.AddChild(&_model->tr);
	_model->tr.SetRotation(0.0f, 180.0f, 0.0f);

	camera = flt::CreateGameObject<Camera>(true, this, _board);

	SetPositionToRatioPosition(0.5f, 0.75f);
}

void Player::OnEnable()
{
	//_padIndex = 0;
}

void Player::Update(float deltaSecond)
{
	if (flt::GetKeyDown(flt::KeyCode::backspace))
	{
		static MainMenuScene* scene = flt::CreateScene<MainMenuScene>();
		flt::SetScene(scene);
	}

	if (_isGameOver)
	{
		return;
	}

	flt::Vector4f pos = tr.GetWorldPosition();
	flt::Vector4f nextPosOffset{};

	int tileState = _board->QueryTileState(pos.x, pos.z);
	int nextTileState = _board->QueryNextTileState(pos.x, pos.z);
	int blocked = BLOCKED_TILE;

	_isCrushed = false;
	if ((tileState & blocked) && (nextTileState & blocked))
	{
		_isCrushed = true;
	}

	if ((tileState == (int)eTileStateFlag::NONE))
	{
		camera->TraceFalling();
		_fallSpeed += 9.8f * deltaSecond;
		if (_fallSpeed >= 30.0f)
		{
			_fallSpeed = 30.0f;
		}

		tr.AddWorldPosition(0.0f, -_fallSpeed * deltaSecond, 0.0f);
		return;
	}

	if (tileState & (int)eTileStateFlag::GENERATING)
	{
		nextPosOffset = { 0.0f, 0.0f, -30.0f * deltaSecond, 0.0f };
		auto nextPos = pos + nextPosOffset;
		tr.SetWorldPosition(nextPos);
		nextPosOffset = { 0.0f,0.0f,0.0f,0.0f };
		return;
	}

	flt::KeyData keyData = flt::GetKey(flt::KeyCode::w);
	if (keyData)
	{
		nextPosOffset += tr.WorldForward();
	}

	keyData = flt::GetKey(flt::KeyCode::s);
	if (keyData)
	{
		nextPosOffset += -tr.WorldForward();
	}

	keyData = flt::GetKey(flt::KeyCode::a);
	if (keyData)
	{
		nextPosOffset += -tr.WorldRight();
	}

	keyData = flt::GetKey(flt::KeyCode::d);
	if (keyData)
	{
		nextPosOffset += tr.WorldRight();
	}

	keyData = flt::GetKeyDown(flt::KeyCode::j);
	if (keyData && !_isCrushed)
	{
		if (!_board->IsMineSet())
		{
			_board->SetMine(pos.x, pos.z);
		}
		else
		{
			_board->DetonateMine();
		}
	}

	keyData = flt::GetKeyDown(flt::KeyCode::k);
	if (keyData && !_isCrushed)
	{
		_board->DetonateAdvantageMine();
	}

	flt::GamePadState state;
	bool isGamePadConnected = flt::GetGamePadState(_padIndex, &state);
	if (isGamePadConnected)
	{
		nextPosOffset.z += state.lStickY;
		nextPosOffset.x += state.lStickX;

		if (state.buttons & flt::GamePadState::ButtonFlag::UP)
		{
			nextPosOffset.z += 1.0f;
		}
		if (state.buttons & flt::GamePadState::ButtonFlag::DOWN)
		{
			nextPosOffset.z -= 1.0f;
		}
		if (state.buttons & flt::GamePadState::ButtonFlag::LEFT)
		{
			nextPosOffset.x -= 1.0f;
		}
		if (state.buttons & flt::GamePadState::ButtonFlag::RIGHT)
		{
			nextPosOffset.x += 1.0f;
		}

		if (state.buttonsDown & flt::GamePadState::ButtonFlag::A)
		{
			// 지뢰 설치
			if (!_board->IsMineSet())
			{
				_board->SetMine(pos.x, pos.z);
			}
			else
			{
				_board->DetonateMine();
			}
		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::B)
		{
			// 슈퍼 지뢰 폭파
			_board->DetonateAdvantageMine();
		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::X)
		{

		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::Y)
		{
			// 빨리감기
			_board->FastForward();
		}
		if(state.buttonsUp & flt::GamePadState::ButtonFlag::Y)
		{
			_board->EndFastForward();
		}
	}

	if (nextPosOffset.NormPow() > 1.0f)
	{
		nextPosOffset.Normalize();
	}

	nextPosOffset *= _speed * deltaSecond;
	flt::Vector4f nextPos = nextPosOffset + pos;

	tr.LookAt(nextPos);

	tileState = _board->QueryTileState(nextPos.x, pos.z);
	nextTileState = _board->QueryNextTileState(nextPos.x, pos.z);
	blocked = BLOCKED_TILE;

	// 좌 우 이동
	// 현재 상태에 이동 가능하거나 
	// 다음 상태에 이동 가능하면 이동 가능
	if ((tileState == (int)eTileStateFlag::NONE)
		|| ((tileState & blocked) && (nextTileState & blocked)))
	{
		// 이동 불가능할 경우에는 x값을 원래 값으로 되돌린다.
		//nextPos.x = pos.x;
		nextPosOffset.x = 0.0f;
	}

	tileState = _board->QueryTileState(pos.x, nextPos.z);
	nextTileState = _board->QueryNextTileState(pos.x, nextPos.z);

	// 상 하 이동
	// 현재 상태에 이동이 가능하거나 
	// 아래에 내려가는 경우에 한해서 다음 상태에 이동 가능하면 가능
	if ((tileState == (int)eTileStateFlag::NONE)
		|| ((tileState & blocked) && (nextTileState & blocked))
		|| ((tileState & blocked) && !(nextTileState & blocked) && nextPosOffset.z > 0))
	{
		// 이동 불가능할 경우에는 z값을 원래 값으로 되돌린다.
		//nextPos.z = pos.z;
		nextPosOffset.z = 0.0f;
	}

	if (nextPosOffset.NormPow() > 0)
	{
		_model->PlayWalk();
	}
	else
	{
		_model->PlayIdle();
	}
	nextPos = nextPosOffset + pos;
	tr.SetWorldPosition(nextPos);

	// 디버그용 코드
	keyData = flt::GetKey(flt::KeyCode::mouseLButton);
	if (keyData)
	{
		flt::Vector4f pos = tr.GetWorldPosition();
		int tileX = 0;
		int tileZ = 0;
		_board->ConvertToTileIndex(pos.x, pos.z, tileX, tileZ);

		float x = 0.0f;
		float z = 0.0f;

		_board->ConvertToTilePosition(tileX, tileZ, x, z);

		printf("%.3f, %.3f | %d, %d | %.3f, %.3f\n", pos.x, pos.z, tileX, tileZ, x, z);
	}

	if (isGamePadConnected)
	{
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::A)
		{
			flt::SetGamePadVibration(_padIndex, true, 1.0f, 1.0f);
			printf("A\n");
		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::B)
		{
			flt::SetGamePadVibration(_padIndex, false, 1.0f, 1.0f);
			printf("B\n");
		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::X)
		{
			printf("X\n");
		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::Y)
		{
			printf("Y\n");
		}

		if (state.buttonsUp & flt::GamePadState::ButtonFlag::A)
		{
			printf("A Up\n");
		}
		if (state.buttonsUp & flt::GamePadState::ButtonFlag::B)
		{
			printf("B Up\n");
		}
		if (state.buttonsUp & flt::GamePadState::ButtonFlag::X)
		{
			printf("X Up\n");
		}
		if (state.buttonsUp & flt::GamePadState::ButtonFlag::Y)
		{
			printf("Y Up\n");
		}
	}
}

void Player::SetGameOver()
{
	_isGameOver = true;
}

void Player::SetAlbedoPath(std::wstring path)
{
	_model->SetAlbedoPath(path);
}

void Player::SetPositionToRatioPosition(float ratioX, float ratioY)
{
	float x = 0.0f;
	float z = 0.0f;

	_board->GetRatioPosition(ratioX, ratioY, x, z);
	tr.SetPosition(x, 2.0f, z);
}

