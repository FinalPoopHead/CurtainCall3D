#pragma once
#include <PrimitiveBatch.h>
#include <SimpleMath.h>
#include <VertexTypes.h>
#include <vector>

#include "../RocketCommon/ILineRenderer.h"

namespace Rocket::Core
{
	class LineRenderer : public Rocket::Core::ILineRenderer
	{
	private:
		struct Line
		{
			DirectX::XMVECTOR startPos;
			DirectX::XMVECTOR endPos;
			DirectX::XMVECTOR color;
		};

	public:
		// ILineRenderer을(를) 통해 상속됨
		virtual void DrawLine(Vector3 startPos, Vector3 endPos, Color color) override;
		virtual void DrawLine(Vector3 startPos, Vector3 direction, float length, Color color) override;

		void Flush();
		std::vector<Line>& GetLines();
			
	private:
		std::vector<Line> _lines;
	};
}
