#pragma once
#include "Singleton.h"
#include "MathHeader.h"

#include <vector>
#include <string>

namespace Rocket::Core
{
	struct DebugText
	{
		Vector2 centerPos;
		std::string content;
		float size;
	};

	struct DebugBox
	{
		Matrix worldTM;
		Vector3 widthHeightDepth;
		bool isWire;
		Color color;
	};

	struct DebugLine
	{
		Vector3 beginPoint;
		Vector3 endPoint;
		Color color;
	};

	struct Debug2DBox
	{
		Vector2 LT;
		Vector2 RB;
		Color color;
	};

}

namespace Rocket::Core
{
	class DebugSystem : public Singleton<DebugSystem>
	{
		friend Singleton;
	private:
		DebugSystem();

	public:
		void Initialize();
		void Finalize();

	public:
		void CheckDebugMode();

	public:
		void DrawDebugText(Vector2 centerPos, std::string content, float size);
		void DrawDebugBox(Matrix worldTM, Vector3 widthHeightDepth = { 1.0f,1.0f,1.0f }, bool isWire = true, Color color = { 1.0f,0.0f,0.0f,1.0f });
		void DrawDebugLine(Vector3 beginPoint, Vector3 endPoint, Color color = { 1.0f,0.0f,0.0f,1.0f });
		void DrawDebug2DBox(Vector2 LT, Vector2 RB, Color color = { 1.0f,0.0f,0.0f,1.0f });

	public:
		std::vector<DebugText>& GetTextQueue();
		std::vector<DebugBox>& GetBoxQueue();
		std::vector<DebugLine>& GetLineQueue();
		std::vector<Debug2DBox>& Get2DBoxQueue();

	public:
		void FlushBoxQueue();
		void FlushTextQueue();
		void FlushLineQueue();
		void Flush2DBoxQueue();

	private:
		std::vector<DebugText> _textQueue;
		std::vector<DebugBox> _boxQueue;
		std::vector<DebugLine> _lineQueue;
		std::vector<Debug2DBox> _2DBoxQueue;

	public:
		void SetDebugMode(bool isDebugMode);
		bool IsDebugMode() const;

	private:
		bool _isDebugMode;
	};
}
