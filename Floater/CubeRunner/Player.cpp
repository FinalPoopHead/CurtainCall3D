#include "Player.h"
#include "../FloaterGameEngine/include/Input.h"
#include "Camera.h"
#include "Board.h"
#include "PlayerModel.h"


//TEST Include
#include "MainMenuScene.h"

Player::Player(Board* board) :
	camera(nullptr)
	, _soundComponent()
	, _soundIndex()
	, _model(nullptr)
	, _board(board)
	, _state(ePlayerState::PLAYING)
	, _padIndex(-1)
	, _speed(10.0f)
	, _isPlayWalkSound(false)
{
	std::wstring path = L"../Resources/Sound/";
	_soundComponent = AddComponent<flt::SoundComponent>(true);
	_soundComponent->AddSound(path + L"PlayerWalk.mp3");

	int index = 0;
	_soundIndex = std::unordered_map<std::string, int>();
	_soundIndex["PlayerWalk"] = index++;

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
	flt::GamePadState state;
	bool isGamePadConnected = flt::GetGamePadState(_padIndex, &state);
	if (isGamePadConnected)
	{
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::BACK)
		{
			static MainMenuScene* scene = flt::CreateScene<MainMenuScene>();
			flt::SetScene(scene);
		}
		if(state.buttonsDown & flt::GamePadState::ButtonFlag::START)
		{
			PauseResumeGame();
		}
	}

	if (flt::GetKeyDown(flt::KeyCode::backspace))
	{
		static MainMenuScene* scene = flt::CreateScene<MainMenuScene>();
		flt::SetScene(scene);
	}
	if (flt::GetKeyDown(flt::KeyCode::spacebar))
	{
		PauseResumeGame();
	}

	switch (_state)
	{
	case ePlayerState::PLAYING:
		break;
	case ePlayerState::CRUSHED:
		// TODO : 깔렸을때 자동으로 빨리감기해서 굴려버려야함.
		break;
	case ePlayerState::PUSHEDOUT:
	{
		flt::Vector4f pos = tr.GetWorldPosition();
		flt::Vector4f nextPosOffset{};
		nextPosOffset = { 0.0f, 0.0f, -30.0f * deltaSecond * _board->GetBattleSpeed(), 0.0f };
		auto nextPos = pos + nextPosOffset;
		tr.SetWorldPosition(nextPos);
		nextPosOffset = { 0.0f,0.0f,0.0f,0.0f };
	}
		break;
	case ePlayerState::FALLING:
	{
		if (tr.GetWorldPosition().y < -300.0f)
		{
			camera->StopCamera();
			_state = ePlayerState::GAMEOVER;
			_board->OnEndPlayerFalling();
			return;
		}

		_model->PlayAnimation(5, false);

		camera->TraceFalling();
		_fallSpeed += 9.8f * deltaSecond;
		if (_fallSpeed >= 60.0f)
		{
			_fallSpeed = 60.0f;
		}

		tr.AddWorldPosition(0.0f, -_fallSpeed * deltaSecond, 0.0f);
	}
		return;
		break;
	case ePlayerState::GAMEOVER:
		return;
		break;
	default:
		break;
	}

	_state = ePlayerState::PLAYING;

	flt::Vector4f pos = tr.GetWorldPosition();
	flt::Vector4f nextPosOffset{};

	int tileState = _board->QueryTileState(pos.x, pos.z);
	int nextTileState = _board->QueryNextTileState(pos.x, pos.z);
	int blocked = BLOCKED_TILE;

	if ((tileState & blocked) && (nextTileState & blocked))
	{
		_state = ePlayerState::CRUSHED;
		return;
	}

	if ((tileState == (int)eTileStateFlag::NONE))
	{
		_board->OnStartPlayerFall();
		_state = ePlayerState::FALLING;
		return;
	}

	if (tileState & (int)eTileStateFlag::GENERATING)
	{
		_state = ePlayerState::PUSHEDOUT;
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
	if (keyData)
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
	if (keyData)
	{
		_board->DetonateAdvantageMine();
	}

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

	nextPosOffset *= _speed * deltaSecond * _board->GetBattleSpeed();
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

	std::string soundName = "PlayerWalk";
	if (nextPosOffset.NormPow() > 0)
	{
		_model->PlayWalk();

		if (!_soundComponent->isPlay(_soundIndex[soundName]))
		{
			_soundComponent->Play(_soundIndex[soundName]);
			std::cout << "Play Walk Sound\n";
		}
		// TODO : 현재 Play 중인지 아닌지 체크해서 재생시켜야함.
// 		_soundComponent->Play(_soundIndex["PlayerWalk"]);
// 		_soundComponent->Play(_soundIndex["PlayerWalk1"]);
	}
	else
	{
		_model->PlayIdle();
	}
	nextPos = nextPosOffset + pos;
	tr.SetWorldPosition(nextPos);

	if (isGamePadConnected)
	{
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::A)
		{

			printf("A\n");
		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::B)
		{

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

void Player::SetPadVibration(bool isRightMotor, float motorPower, float duration)
{

	flt::SetGamePadVibration(_padIndex, isRightMotor, motorPower, duration);
}

void Player::PauseResumeGame()
{
	float timeScale = flt::GetTimeScale();
	if (timeScale > 0)
	{
		flt::SetTimeScale(0.0f);
	}
	else
	{
		flt::SetTimeScale(1.0f);
	}
}

