#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include <string>

class UnityLoadScene : public flt::Scene
{
public:
	UnityLoadScene(const std::wstring& jsonPath);
	virtual ~UnityLoadScene() {}

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

private:
	void LoadUnityJson();

private:
	std::wstring _jsonPath;
};

