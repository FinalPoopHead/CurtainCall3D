#pragma once
#include "./include/internal/Gameobject.h"
#include "./include/internal/Collider.h"


namespace flt
{
	class CollisionPair
	{
	public:
		CollisionPair() : CollisionPair(nullptr, nullptr, nullptr, nullptr) {}
		CollisionPair(flt::GameObject* obj1, flt::Collider* collider1, flt::GameObject* obj2, flt::Collider* collider2) :
			obj1(obj1), collider1(collider1), obj2(obj2), collider2(collider2)
		{
		}

		bool operator==(const CollisionPair& other) const
		{
			return (obj1 == other.obj1 && obj2 == other.obj2) || (obj1 == other.obj2 && obj2 == other.obj1);
		}

		bool operator<(const CollisionPair& other) const
		{
			if (obj1 == other.obj1)
			{
				return obj2 < other.obj2;
			}
			return obj1 < other.obj1;
		}

	public:
		struct HashFunction
		{
			size_t operator()(const CollisionPair& pair) const;
		};

	public:

		flt::GameObject* obj1;
		flt::Collider* collider1;
		flt::GameObject* obj2;
		flt::Collider* collider2;
		bool flag = 0;
	};
}

