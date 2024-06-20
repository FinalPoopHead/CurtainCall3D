#include "Player.h"
#include "../FloaterGameEngine/include/Input.h"
#include "Camera.h"
#include "Board.h"


//TEST Include
#include "MainMenuScene.h"

Player::Player(Board* board)
	: _board(board)
	, _isGameOver(false)
	, _isCrushed(false)
	, _padIndex(-1)
	, _speed(10.0f)
{
	//AddComponent<flt::CameraComponent>(true);
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);

	std::wstring filePath = L"..\\Resources\\Models\\Rob02.fbx";
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02Yellow_AlbedoTransparency.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02_Normal.dds", flt::RawMaterial::TextureType::NORMAL);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02White_MetallicSmoothness.dds", flt::RawMaterial::TextureType::METALLIC);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02White_Roughness.png", flt::RawMaterial::TextureType::ROUGHNESS);
	//renderer->SetFilePath(L"../Resources/Models/cube.fbx");

	renderer->PlayAnimation(0, true);

	Camera* camera = flt::CreateGameObject<Camera>(true, this, _board);

	float x = 0.0f;
	float z = 0.0f;

	_board->ConvertToTilePosition(2, 0, x, z);
	tr.SetPosition(x, 2.0f, z);
}

void Player::OnEnable()
{
	_padIndex = 0;
}

void Player::Update(float deltaSecond)
{
	if (_isGameOver)
	{
		return;
	}

	flt::Vector4f pos = tr.GetWorldPosition();
	flt::Vector4f nextPosOffset{};

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

	// TODO : 현재 깔린상태가 아니라 내가 갈곳을 못 갈 경우에 _isCrushed를 true로 설정되므로 의도치 않게 동작을 못하게 된다.
	//			따라서 잠시 취소
	_isCrushed = false; //임시
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
	}

	if (nextPosOffset.NormPow() > 1.0f)
	{
		nextPosOffset.Normalize();
	}

	nextPosOffset *= _speed * deltaSecond;
	flt::Vector4f nextPos = nextPosOffset + pos;

	tr.LookAt(nextPos);

	int tileState = _board->QueryTileState(nextPos.x, pos.z);
	int nextTileState = _board->QueryNextTileState(nextPos.x, pos.z);
	int blocked = BLOCKED_TILE;

	_isCrushed = false;

	// 좌 우 이동
	// 현재 상태에 이동 가능하거나 
	// 다음 상태에 이동 가능하면 이동 가능
	if ((tileState == (int)TileStateFlag::None)
		|| ((tileState & blocked) && (nextTileState & blocked)))
	{
		// 이동 불가능할 경우에는 x값을 원래 값으로 되돌린다.
		nextPos.x = pos.x;
		_isCrushed = true;
	}

	tileState = _board->QueryTileState(pos.x, nextPos.z);
	nextTileState = _board->QueryNextTileState(pos.x, nextPos.z);

	// 상 하 이동
	// 현재 상태에 이동이 가능하거나 
	// 아래에 내려가는 경우에 한해서 다음 상태에 이동 가능하면 가능
	if ((tileState == (int)TileStateFlag::None)
		|| ((tileState & blocked) && (nextTileState & blocked))
		|| ((tileState & blocked) && !(nextTileState & blocked) && nextPosOffset.z > 0))
	{
		// 이동 불가능할 경우에는 z값을 원래 값으로 되돌린다.
		nextPos.z = pos.z;
		_isCrushed = true;
	}

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

	if (flt::GetKeyDown(flt::KeyCode::enter))
	{
		static MainMenuScene* scene = flt::CreateScene<MainMenuScene>();
		flt::SetScene(scene);
	}
}

void Player::SetGameOver()
{
	_isGameOver = true;
}

