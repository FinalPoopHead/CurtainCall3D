#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class TextObject;

class RankViewer : public flt::GameObject
{
	struct RankData
	{
		int rank;
		std::string name;
		int score;
	};
public:
	RankViewer();
	virtual ~RankViewer() {}

protected:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void UpdateRankText();


private:
	std::vector<RankData> _rankData;

	TextObject* _rankingPanel;
	std::vector<TextObject*> _rankText;
	std::vector<TextObject*> _rankNameText;
	std::vector<TextObject*> _rankScoreText;
};
