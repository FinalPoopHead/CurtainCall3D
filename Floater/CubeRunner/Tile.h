#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Tile : public flt::GameObject
{
public:
	Tile();
	virtual ~Tile();

	virtual void OnCreate() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;

	void EnableMine();
	void DisableMine();

	void EnableDetonated();
	void DisableDetonated();

	void EnableAdvantageMine();
	void DisableAdvantageMine();

public:
	flt::GameObject* _cube;				// 현재 타일에 있는 큐브
	flt::GameObject* _mine;				// 지뢰
	flt::GameObject* _advantageMine;	// 어드밴 지뢰
	flt::GameObject* _detonated;		// 폭파예정 지뢰
};
