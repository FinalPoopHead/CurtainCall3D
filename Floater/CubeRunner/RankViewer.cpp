#include "RankViewer.h"
#include "TextObject.h"
#include <fstream>

RankViewer::RankViewer()
	: _rankData()
	, _rankingPanel(nullptr)
	, _rankText()
	, _rankNameText()
	, _rankScoreText()
{

	std::wstring bigFontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_55.spritefont";
	flt::Vector4f whiteColor = { 1.0f,1.0f,1.0f,1.0f };

	_rankingPanel = flt::CreateGameObject<TextObject>(false);
	_rankingPanel->SetOffsetPosition({ 0.5f,0.1f });
	_rankingPanel->SetText(L"B E S T    C U B E    R U N N E R S");
	_rankingPanel->SetFont(bigFontPath);
	_rankingPanel->SetTextColor(whiteColor);
	_rankingPanel->SetTextAlignment(eTextAlignment::CENTER);

	float rankingOffsetY = 100.0f;
	float rankX = -550.0f;
	float nameX = -400.0f;
	float scoreX = 550.0f;

	for (int i = 0; i < 10; ++i)
	{
		auto rankText = flt::CreateGameObject<TextObject>(false);
		rankText->SetParent(_rankingPanel);
		rankText->SetPosition({ rankX, rankingOffsetY * (i + 1) });
		rankText->SetFont(bigFontPath);
		rankText->SetTextColor(whiteColor);
		rankText->SetTextAlignment(eTextAlignment::LEFT);
		_rankText.push_back(rankText);

		auto rankNameText = flt::CreateGameObject<TextObject>(false);
		rankNameText->SetParent(_rankingPanel);
		rankNameText->SetPosition({ nameX, rankingOffsetY * (i + 1) });
		rankNameText->SetFont(bigFontPath);
		rankNameText->SetTextColor(whiteColor);
		rankNameText->SetTextAlignment(eTextAlignment::LEFT);
		_rankNameText.push_back(rankNameText);

		auto rankScoreText = flt::CreateGameObject<TextObject>(false);
		rankScoreText->SetParent(_rankingPanel);
		rankScoreText->SetPosition({ scoreX, rankingOffsetY * (i + 1) });
		rankScoreText->SetFont(bigFontPath);
		rankScoreText->SetTextColor(whiteColor);
		rankScoreText->SetTextAlignment(eTextAlignment::RIGHT);
		_rankScoreText.push_back(rankScoreText);
	}

	UpdateRankText();
}

void RankViewer::OnEnable()
{
	for(auto& rankText : _rankText)
	{
		rankText->Enable();
	}

	for(auto& rankNameText : _rankNameText)
	{
		rankNameText->Enable();
	}

	for(auto& rankScoreText : _rankScoreText)
	{
		rankScoreText->Enable();
	}

	_rankingPanel->Enable();
}

void RankViewer::OnDisable()
{
	for (auto& rankText : _rankText)
	{
		rankText->Disable();
	}

	for (auto& rankNameText : _rankNameText)
	{
		rankNameText->Disable();
	}

	for (auto& rankScoreText : _rankScoreText)
	{
		rankScoreText->Disable();
	}

	_rankingPanel->Disable();
}

void RankViewer::UpdateRankText()
{
	std::string str_buf;
	std::fstream fs;

	fs.open("../Resources/RankData/Ranking.csv", std::ios::in);

	while (!fs.eof())
	{
		getline(fs, str_buf);

		if (fs.eof())
		{
			break;
		}

		std::istringstream iss(str_buf);
		std::string token;

		int rank;
		std::string name;
		int score;

		getline(iss, token, ',');
		rank = std::stoi(token);

		getline(iss, token, ',');
		name = token;

		getline(iss, token, ',');
		score = std::stoi(token);

		RankData rankData;
		rankData.rank = rank;
		rankData.name = name;
		rankData.score = score;

		_rankData.push_back(rankData);
	}
	fs.close();

	for (int i = 0; i < _rankData.size(); ++i)	
	{
		// TODO : 이건 하드코딩이다
		if (i >= 10)
		{
			break;
		}

		std::wstring name = flt::ToWstring(_rankData[i].name);
		_rankText[i]->SetText(std::to_wstring(_rankData[i].rank));
		_rankNameText[i]->SetText(name);
		_rankScoreText[i]->SetText(std::to_wstring(_rankData[i].score));
	}
}
