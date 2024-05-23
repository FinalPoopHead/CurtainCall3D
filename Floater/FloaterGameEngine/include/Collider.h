#pragma once

namespace flt
{
	class GameObject;

	class Collider
	{
	public:
		Collider(GameObject* gameObject);

	private:
		GameObject* _gameObject;
	};
}
