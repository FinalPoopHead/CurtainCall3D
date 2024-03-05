#include "LineRenderer.h"

namespace Rocket::Core
{
	void LineRenderer::DrawLine(Vector3 startPos, Vector3 endPos, Color color)
	{
		DirectX::XMFLOAT3 _startPos{ startPos.x, startPos.y,startPos.z };
		DirectX::XMFLOAT3 _endPos{ endPos.x, endPos.y, endPos.z };
		DirectX::XMFLOAT4 _color{ color.x, color.y, color.z, color.w };

		auto startVec = DirectX::XMLoadFloat3(&_startPos);
		auto endVec = DirectX::XMLoadFloat3(&_endPos);
		auto colorVec = DirectX::XMLoadFloat4(&_color);

		_lines.emplace_back(startVec, endVec, colorVec);
	}

	void LineRenderer::DrawLine(Vector3 startPos, Vector3 direction, float length, Color color)
	{
		DirectX::XMFLOAT3 _startPos{ startPos.x, startPos.y, startPos.z };
		DirectX::XMFLOAT3 _direction{ direction.x, direction.y, direction.z };
		DirectX::XMFLOAT4 _color{ color.x, color.y, color.z, color.w };

		auto dirVec = DirectX::XMLoadFloat3(&_direction);
		dirVec = DirectX::XMVector3Normalize(dirVec);
		dirVec = DirectX::XMVectorScale(dirVec, length);

		auto startVec = DirectX::XMLoadFloat3(&_startPos);
		auto endVec = DirectX::XMVectorAdd(startVec, dirVec);
		auto colorVec = DirectX::XMLoadFloat4(&_color);

		_lines.emplace_back(startVec, endVec, colorVec);
	}

	std::vector<Rocket::Core::LineRenderer::Line>& LineRenderer::GetLines()
	{
		return _lines;
	}

	void LineRenderer::Flush()
	{
		_lines.clear();
	}

}
