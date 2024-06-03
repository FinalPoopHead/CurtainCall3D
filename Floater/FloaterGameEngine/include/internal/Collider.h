#pragma once

namespace flt
{
	class GameObject;

	class Collider
	{
	public:
		Collider(GameObject* gameObject);

		GameObject* _gameObject;
	};
}
