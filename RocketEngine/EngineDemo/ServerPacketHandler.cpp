#include "pch.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "GameManager.h"
#include "PlayerInfo.h"
#include "PlayerMove.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
	{
		std::cout << "Failed Login" << std::endl;
		return false;
	}

	std::cout << "S_Login ID : " << pkt.player().id() << std::endl;
	std::cout << std::endl;

	auto& gameManager = GameManager::Instance();
	gameManager.myId = pkt.player().id();

	auto& lobbyManager = LobbyManager::Instance();
	lobbyManager.SetID(pkt.player().id());

	return true;
}

bool Handle_S_CREATE_ROOM(PacketSessionRef& session, Protocol::S_CREATE_ROOM& pkt)
{
	auto& lobbyManager = LobbyManager::Instance();

	if (pkt.success() == false)
	{
		lobbyManager.FailCreateRoom();

		// TODO �� ���� ���� â ����
		std::cout << "Failed Create Room" << std::endl;

		return false;
	}

	std::cout << "Create Room : " << pkt.roominfo().roomcode() << std::endl;

	lobbyManager.SetRoomCode(pkt.roominfo().roomcode());
	lobbyManager.SetRoundKillCount(pkt.roominfo().roundkill());
	lobbyManager.SetRoundTime(pkt.roominfo().roundtime());

	lobbyManager.SetIndex(1);
	lobbyManager.SetIsHost(true);


	return true;
}

bool Handle_S_ENTER_ROOM(PacketSessionRef& session, Protocol::S_ENTER_ROOM& pkt)
{
	auto& lobbyManager = LobbyManager::Instance();
	auto& gameManager = GameManager::Instance();

	if (pkt.success() == false)
	{
		lobbyManager.FailJoinRoom();
		// �� ���� ����
		// TODO ���� ���� â ����
		std::cout << "Failed ENTER_ROOM" << std::endl;

		return false;
	}

	std::cout << "S_ENTER_ROOM" << std::endl;

	lobbyManager.SuccessJoinRoom();

	lobbyManager.SetRoomCode(pkt.roominfo().roomcode());
	lobbyManager.SetRoundKillCount(pkt.roominfo().roundkill());
	lobbyManager.SetRoundTime(pkt.roominfo().roundtime());

	// TODO �� ���������� ���� �޾ƾ���
	std::cout << "S_ENTER_ROOM" << std::endl;

	for (auto& player : pkt.players())
	{
		if (player.id() == lobbyManager.GetID())
		{
			lobbyManager.SetIndex(player.index());
			gameManager.SetMyPlayer(player.index());
		}

		lobbyManager.EnterPlayer(player.index(), player.id());
		gameManager.EnterPlayer(player);
	}

	return true;
}

bool Handle_S_ANOTHER_ENTER_ROOM(PacketSessionRef& session, Protocol::S_ANOTHER_ENTER_ROOM& pkt)
{
	auto& gameManager = GameManager::Instance();
	auto& lobbyManager = LobbyManager::Instance();
	auto player = pkt.player();

	if (lobbyManager.GetID() != player.id())
	{
		lobbyManager.EnterPlayer(player.index(), player.id());
		gameManager.EnterPlayer(player);
	}

	return true;
}

bool Handle_S_ANOTHER_LEAVE_ROOM(PacketSessionRef& session, Protocol::S_ANOTHER_LEAVE_ROOM& pkt)
{
	auto& lobbyManger = LobbyManager::Instance();
	auto& gameManger = GameManager::Instance();

	auto player = pkt.player();

	lobbyManger.LeavePlayer(player.index());
	gameManger.LeavePlayer(player);

	return true;
}

bool Handle_S_ROOM_SETTING(PacketSessionRef& session, Protocol::S_ROOM_SETTING& pkt)
{
	std::cout << "S_ROOM_SETTING" << std::endl;
	auto roomInfo = pkt.roominfo();

	auto& lobbyManager = LobbyManager::Instance();

	if (lobbyManager.GetRoomCode() != roomInfo.roomcode())
		return false;

	lobbyManager.SetRoundKillCount(roomInfo.roundkill());
	lobbyManager.SetRoundTime(roomInfo.roundtime());

	return true;
}

bool Handle_S_ROOM_START(PacketSessionRef& session, Protocol::S_ROOM_START& pkt)
{
	std::cout << "S_ROOM_START" << std::endl;
	auto roomInfo = pkt.roominfo();

	auto& lobbyManager = LobbyManager::Instance();
	auto& gameManager = GameManager::Instance();

	if (lobbyManager.GetRoomCode() != roomInfo.roomcode())
		return false;

	lobbyManager.ReceiveGameStart(roomInfo.roundkill(), roomInfo.roundtime());

	for (auto& player : pkt.players())
	{
// 		if (lobbyManager.GetID() == player.id())
// 		{
// 			continue;
// 		}

		gameManager.OnStartGame(player.index());

	}

	for (int i = 0; i < pkt.spawnpoint().size(); i++)
	{
		auto playerInfo = gameManager.GetPlayerInfo(pkt.players()[i].index());
		gameManager.SpawnPlayer(playerInfo->gameObject, pkt.spawnpoint(i));

		playerInfo->stats.ableJumpCount = -1;
	}


	// Todo room ���� ���� �߰�
	std::cout << "Room State : " << (int32)pkt.roominfo().state() << endl;


	GameManager::Instance().SetRoundInfo(pkt.roundinfo().state(), pkt.roundinfo().time());


	return true;
}

bool Handle_S_ROUND_START(PacketSessionRef& session, Protocol::S_ROUND_START& pkt)
{
	// Todo �÷��̾� ���� �����ϰ� �����ϱ�
	// Todo ��Ʋ ���� ���� �� 
	// �÷��̾� ������ ��Ÿ�� ����Ʈ �ε���
	auto& gameManager = GameManager::Instance();
	gameManager.SetRoundInfo(pkt.roundinfo().state(), pkt.roundinfo().time());

	for (int i = 0; i < pkt.players_size(); i++)
	{
		auto& player = pkt.players(i);
		auto playerInfo = gameManager.GetPlayerInfo(player.id());
		playerInfo->ReadPlayerInfo(player);
	}

	auto myPlayerInfo = gameManager.GetPlayerInfo(gameManager.myId);
	myPlayerInfo->currentAmmoCount = myPlayerInfo->stats.bulletCapacity;

	RocketEngine::PlayAudioOnce("../Sound/RoundStart&End/round_gamestart.wav", 1.0f);

	return true;
}

bool Handle_S_SHOP_START(PacketSessionRef& session, Protocol::S_SHOP_START& pkt)
{
	// Todo ���� 3�� �ð�
	GameManager::Instance().SetRoundInfo(pkt.roundinfo().state(), pkt.roundinfo().time());



	return true;
}

bool Handle_S_END_GAME(PacketSessionRef& session, Protocol::S_END_GAME& pkt)
{
	GameManager::Instance().SetRoundInfo(pkt.roundinfo().state(), pkt.roundinfo().time());

	RocketEngine::PlayAudioOnce("../Sound/RoundStart&End/round_end.wav", 1.0f);
	// Todo ���� ���� ���� ����
	GameManager::Instance().PopUpRoundOver();

	return true;
}

bool Handle_S_RESULT_START(PacketSessionRef& session, Protocol::S_RESULT_START& pkt)
{
	GameManager::Instance().SetRoundInfo(pkt.roundinfo().state(), pkt.roundinfo().time());

	RocketEngine::PlayAudioOnce("../Sound/ResultBGM/result_bgm.wav", 1.0f);

	// 3�� ��ٷȴٰ� ���â ���� ��� BGM ���
	// Todo ���� ���â ����
	// Todo ���ӸŴ��� �ʱ�ȭ�ϱ�

	// Todo time ������ GameManager InitRound() �����Ű��
	auto& gameManager = GameManager::Instance();

	int count = 0;

	for (auto& info : pkt.players())
	{
		uint32 index = info.roundrank() - 1;

		switch (info.index())
		{

			case 1:
				gameManager.SetRankProfile("..\\UI\\portrait\\portrait_watermelon(100x100).png", index);
				break;
			case 2:
				gameManager.SetRankProfile("..\\UI\\portrait\\portrait_kiwi(100x100).png", index);
				break;
			case 3:
				gameManager.SetRankProfile("..\\UI\\portrait\\portrait_carrot(100x100).png", index);
				break;
			case 4:
				gameManager.SetRankProfile("..\\UI\\portrait\\portrait_banana(100x100).png", index);
				break;
			default:
				break;
		}
		gameManager.SetRankKill(std::to_string(info.kill()), index);
		gameManager.SetRankDeath(std::to_string(info.death()), index);
		gameManager.SetRankGold(std::to_string(info.gold()), index);

		count++;
	}
	gameManager.ShowResult();
	gameManager.DisableUnused(count);

	return true;
}

bool Handle_S_PLAYERS_TRANSFORM(PacketSessionRef& session, Protocol::S_PLAYERS_TRANSFORM& pkt)
{
	auto& gameManager = GameManager::Instance();

	for (auto& playerTransform : pkt.playerstransform())
	{
		auto player = gameManager.GetPlayerInfo(playerTransform.id());
		if (player->IsMine() == true)
			continue;

		auto playerMove = player->gameObject->GetComponent<PlayerMove>();

		auto position = playerTransform.transform().position();
		playerMove->remotePosition.x = position.x();
		playerMove->remotePosition.y = position.y();
		playerMove->remotePosition.z = position.z();

		auto quaternion = playerTransform.transform().quaternion();
		playerMove->remoteQuaternion.w = quaternion.w();
		playerMove->remoteQuaternion.x = quaternion.x();
		playerMove->remoteQuaternion.y = quaternion.y();
		playerMove->remoteQuaternion.z = quaternion.z();
	}

	return true;
}

bool Handle_S_PLAYER_ANIM_STATE(PacketSessionRef& session, Protocol::S_PLAYER_ANIM_STATE& pkt)
{
	auto& gameManager = GameManager::Instance();
	auto player = gameManager.GetPlayerInfo(pkt.player());

	if (gameManager.myId != pkt.player().id())
	{
		player->SetAnimState(pkt.state());
	}
	//player->animState = pkt.state();

	return true;
}

bool Handle_S_SELECT_PARTS(PacketSessionRef& session, Protocol::S_SELECT_PARTS& pkt)
{
	if (pkt.success() == false)
	{
		return false;
	}

	GameManager::Instance().GetPlayerInfo()->SetCurrentGold(pkt.gold());
	GameManager::Instance().SuccessPurchase(pkt.index());
	
	return true;
}

bool Handle_S_SELECT_PARTS_LIST(PacketSessionRef& session, Protocol::S_SELECT_PARTS_LIST& pkt)
{
	auto& gameManager = GameManager::Instance();

	gameManager.selectPartsList.clear();

	for (auto parts : pkt.partslist().parts())
	{
		gameManager.selectPartsList.push_back(std::make_pair(parts.parts(), parts.grade()));
	}
	gameManager.UpdatePartsInfo();

	gameManager.GetPlayerInfo()->SetCurrentGold(pkt.gold());
	// Todo �޾����� ȭ�鿡 ��������

	return true;
}

bool Handle_S_SPAWN_POINT(PacketSessionRef& session, Protocol::S_SPAWN_POINT& pkt)
{
	auto& gameManager = GameManager::Instance();
	auto player = gameManager.GetPlayerInfo(pkt.player());

	player->SetCurrentHP(pkt.healthpoint());
	player->ReadPlayerInfo(pkt.playerinfo());
	player->isDie = false;

	gameManager.SpawnPlayer(player->gameObject, pkt.spawnpointindex());

	return true;
}

bool Handle_S_ATTACKED_PLAYER(PacketSessionRef& session, Protocol::S_ATTACKED_PLAYER& pkt)
{
	// Todo ������ ���� ü�¸� ������ɵ�
	// Todo ü�� ���̴� ������ �ֳ�? �׷� ���⼭ �ϸ� �ȵ�

	auto& gameManager = GameManager::Instance();

	// �� ���
	auto attackPlayer = gameManager.GetPlayerInfo(pkt.attackplayer());
	auto transform = pkt.transform();
	auto position = transform.position();
	auto quaternion = transform.quaternion();

	// Todo �� ��ġ���� �߻��ϴ� �Լ� �������
	attackPlayer->gameObject->GetComponent<PlayerMove>()->ShootGun(
		attackPlayer, { position.x(),position.y(),position.z() }, { quaternion.w(), quaternion.x(),quaternion.y(), quaternion.z() });


	// �������� ������ ��� X
	if (pkt.attackpoint() != Protocol::ATTACK_POINT_NONE)
	{
		// ���� ��� 
		auto hitPlayer = gameManager.GetPlayerInfo(pkt.attackedplayer());
		hitPlayer->SetCurrentHP(pkt.attackedplayerhealthpoint());
		hitPlayer->OnHit();
	}

	return true;
}

bool Handle_S_DEATH_PLAYER(PacketSessionRef& session, Protocol::S_DEATH_PLAYER& pkt)
{
	// ���� �׿����� ų�α׸� ��쳪?
	auto& gameManager = GameManager::Instance();

	// �� ���
	auto attackPlayer = gameManager.GetPlayerInfo(pkt.attackplayer());
	auto transform = pkt.transform();
	auto position = transform.position();
	auto quaternion = transform.quaternion();
	// Todo �� ��ġ���� �߻��ϴ� �Լ� �������
	attackPlayer->gameObject->GetComponent<PlayerMove>()->ShootGun(
		attackPlayer, { position.x(),position.y(),position.z() }, { quaternion.w(), quaternion.x(),quaternion.y(), quaternion.z() });


	attackPlayer->kill++;
	gameManager.SetPlayerKillCountText(attackPlayer->kill, attackPlayer->playerIndex);

	auto hitPlayer = gameManager.GetPlayerInfo(pkt.attackedplayer());
	hitPlayer->death++;

	hitPlayer->currentHealthPoint = 0;
	hitPlayer->isDie = true;
	hitPlayer->SetCurrentHP(0);
	hitPlayer->SetAnimState(Protocol::ANIM_DIE);

	if (gameManager.myId == hitPlayer->playerId)
	{
		gameManager.OnDie();
	}

	// Todo
	// ���ó������
	// �׾����� ȭ���� ���� = �輺�� ���ȭ��

	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	return true;
}
