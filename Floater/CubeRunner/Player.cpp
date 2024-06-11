#include "Player.h"
#include "../FloaterGameEngine/include/Input.h"
#include "Camera.h"
#include "Board.h"


Player::Player(Board* board)
	: _board(board)
	, _isGameOver(false)
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
	flt::Vector4f nextPos{};

	flt::KeyData keyData = flt::GetKey(flt::KeyCode::i);
	if (keyData)
	{
		nextPos += tr.WorldForward();
	}

	keyData = flt::GetKey(flt::KeyCode::k);
	if (keyData)
	{
		nextPos += -tr.WorldForward();
	}

	keyData = flt::GetKey(flt::KeyCode::j);
	if (keyData)
	{
		nextPos += -tr.WorldRight();
	}

	keyData = flt::GetKey(flt::KeyCode::l);
	if (keyData)
	{
		nextPos += tr.WorldRight();
	}

	keyData = flt::GetKeyDown(flt::KeyCode::spacebar);
	if (keyData)
	{
		bool isSetSuccess = _board->SetMine(pos.x, pos.z);

		if(!isSetSuccess)
		{
			_board->DetonateMine();
		}
	}

	keyData = flt::GetKeyDown(flt::KeyCode::enter);
	if (keyData)
	{
		_board->DetonateAdvantageMine();
	}
	
	flt::GamePadState state;
	if (flt::GetGamePadState(_padIndex, &state))
	{
		nextPos.z += state.lStickY;
		nextPos.x += state.lStickX;
	}

	if (state.buttons & flt::GamePadState::ButtonFlag::UP)
	{
		nextPos.z += 1.0f;
	}
	if (state.buttons & flt::GamePadState::ButtonFlag::DOWN)
	{
		nextPos.z -= 1.0f;
	}
	if (state.buttons & flt::GamePadState::ButtonFlag::LEFT)
	{
		nextPos.x -= 1.0f;
	}
	if (state.buttons & flt::GamePadState::ButtonFlag::RIGHT)
	{
		nextPos.x += 1.0f;
	}

	if (nextPos.NormPow() > 1.0f)
	{
		nextPos.Normalize();
	}

	nextPos *= _speed * deltaSecond;
	nextPos += pos;

	tr.LookAt(nextPos);

	int tileState = _board->QueryTileState(nextPos.x, pos.z);
	int blocked = BLOCKED_TILE;
	if (tileState == (int)TileStateFlag::None || (tileState & blocked) != 0)
	{
		nextPos.x = pos.x;
		
	}

	tileState = _board->QueryTileState(pos.x, nextPos.z);
	if (tileState == (int)TileStateFlag::None || (tileState & blocked) != 0)
	{
		nextPos.z = pos.z;
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

void Player::SetGameOver()
{
	_isGameOver = true;
}

