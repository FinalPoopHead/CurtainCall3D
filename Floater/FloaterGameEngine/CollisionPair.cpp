#include "CollisionPair.h"
#include "../FloaterUtil/include/Hash.h"
#include <type_traits>


size_t flt::CollisionPair::HashFunction::operator()(const CollisionPair& pair) const
{
	static_assert(std::is_same_v<size_t, uint64>, "size_t is not uint64");
	uint64 hash = 0;
	//hash_combine(hash, pair.obj1);
	//hash_combine(hash, pair.obj2);
	return hash;
}
