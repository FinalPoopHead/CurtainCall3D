﻿#include "Player.h"
#include "../FloaterGameEngine/include/Input.h"
#include "Camera.h"
#include "Board.h"


Player::Player(Board* board)
	: _board(board)
	, _padIndex(-1)
	, _speed(10.0f)
{
	//AddComponent<flt::CameraComponent>(true);
	flt:: RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	
	std::wstring filePath = L"..\\Resources\\Models\\Rob02.fbx";
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02Yellow_AlbedoTransparency.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02_Normal.dds", flt::RawMaterial::TextureType::NORMAL);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02White_MetallicSmoothness.dds", flt::RawMaterial::TextureType::METALLIC);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02White_Roughness.png", flt::RawMaterial::TextureType::ROUGHNESS);
	//renderer->SetFilePath(L"../Resources/Models/cube.fbx");

	Camera* camera = flt::CreateGameObject<Camera>(true, this);

	float x = 0.0f;
	float z = 0.0f;

	_board->ConvertToTilePosition(2, 0, x, z);
	tr.SetPosition(x, 5.0f, z);
}

void Player::OnEnable()
{
	_padIndex = 0;
}

void Player::Update(float deltaSecond)
{
	flt::Vector4f pos = tr.GetWorldPosition();
	flt::Vector4f nextPos = pos;

	flt::KeyData keyData = flt::GetKey(flt::KeyCode::i);
	if (keyData)
	{
		nextPos += tr.WorldForward() * _speed * deltaSecond;
	}

	keyData = flt::GetKey(flt::KeyCode::k);
	if (keyData)
	{
		nextPos += -tr.WorldForward() * _speed * deltaSecond;
	}

	keyData = flt::GetKey(flt::KeyCode::j);
	if (keyData)
	{
		nextPos += -tr.WorldRight() * _speed * deltaSecond;
	}

	keyData = flt::GetKey(flt::KeyCode::l);
	if (keyData)
	{
		nextPos += tr.WorldRight() * _speed * deltaSecond;
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
		nextPos.z += state.lStickY * _speed * deltaSecond;
		nextPos.x += state.lStickX * _speed * deltaSecond;
	}

	tr.LookAt(nextPos);

	int tileState = _board->QueryTileState(nextPos.x, nextPos.z);
	int blocked = BLOCKED_TILE;
	if (tileState != (int)TileStateFlag::None && (tileState & blocked) == 0)
	{
		tr.SetWorldPosition(nextPos);
	}




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

