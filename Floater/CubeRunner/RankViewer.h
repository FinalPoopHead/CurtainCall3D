#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class TextObject;

class RankViewer : public flt::GameObject
{
	struct RankData
	{
		RankData() : rank(0), name(""), score(0) {}
		RankData(int rank, std::string name, int score)
			: rank(rank), name(name), score(score) {}

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
