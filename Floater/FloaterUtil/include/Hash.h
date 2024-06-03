#pragma once
#include "FloaterType.h"
#include <intrin.h>
#include <memory>
#pragma intrinsic(_umul128)

namespace flt
{
	namespace hash
	{
		struct xxh64 {
			static constexpr uint64 hash(const char* p, uint64 len, uint64 seed)
			{
				return finalize((len >= 32 ? h32bytes(p, len, seed) : seed + PRIME5) + len, p + (len & ~0x1F), len & 0x1F);
			}

		private:
			static constexpr uint64 PRIME1 = 11400714785074694791ULL;
			static constexpr uint64 PRIME2 = 14029467366897019727ULL;
			static constexpr uint64 PRIME3 = 1609587929392839161ULL;
			static constexpr uint64 PRIME4 = 9650029242287828579ULL;
			static constexpr uint64 PRIME5 = 2870177450012600261ULL;

			static constexpr uint64 rotl(uint64 x, int r)
			{
				return ((x << r) | (x >> (64 - r)));
			}
			static constexpr uint64 mix1(const uint64 h, const uint64 prime, int rshift)
			{
				return (h ^ (h >> rshift)) * prime;
			}
			static constexpr uint64 mix2(const uint64 p, const uint64 v = 0)
			{
				return rotl(v + p * PRIME2, 31) * PRIME1;
			}
			static constexpr uint64 mix3(const uint64 h, const uint64 v)
			{
				return (h ^ mix2(v)) * PRIME1 + PRIME4;
			}
#ifdef XXH64_BIG_ENDIAN
			static constexpr uint32 endian32(const char* v)
			{
				return uint32(uint8(v[3])) | (uint32(uint8(v[2])) << 8)
					| (uint32(uint8(v[1])) << 16) | (uint32(uint8(v[0])) << 24);
			}
			static constexpr uint64 endian64(const char* v)
			{
				return uint64(uint8(v[7])) | (uint64(uint8(v[6])) << 8)
					| (uint64(uint8(v[5])) << 16) | (uint64(uint8(v[4])) << 24)
					| (uint64(uint8(v[3])) << 32) | (uint64(uint8(v[2])) << 40)
					| (uint64(uint8(v[1])) << 48) | (uint64(uint8(v[0])) << 56);
			}
#else
			static constexpr uint32 endian32(const char* v)
			{
				return uint32(uint8(v[0])) | (uint32(uint8(v[1])) << 8)
					| (uint32(uint8(v[2])) << 16) | (uint32(uint8(v[3])) << 24);
			}
			static constexpr uint64 endian64(const char* v)
			{
				return uint64(uint8(v[0])) | (uint64(uint8(v[1])) << 8)
					| (uint64(uint8(v[2])) << 16) | (uint64(uint8(v[3])) << 24)
					| (uint64(uint8(v[4])) << 32) | (uint64(uint8(v[5])) << 40)
					| (uint64(uint8(v[6])) << 48) | (uint64(uint8(v[7])) << 56);
			}
#endif
			static constexpr uint64 fetch64(const char* p, const uint64 v = 0)
			{
				return mix2(endian64(p), v);
			}
			static constexpr uint64 fetch32(const char* p)
			{
				return uint64(endian32(p)) * PRIME1;
			}
			static constexpr uint64 fetch8(const char* p)
			{
				return uint8(*p) * PRIME5;
			}
			static constexpr uint64 finalize(const uint64 h, const char* p, uint64 len)
			{
				return (len >= 8) ? (finalize(rotl(h ^ fetch64(p), 27) * PRIME1 + PRIME4, p + 8, len - 8)) :
					((len >= 4) ? (finalize(rotl(h ^ fetch32(p), 23) * PRIME2 + PRIME3, p + 4, len - 4)) :
						((len > 0) ? (finalize(rotl(h ^ fetch8(p), 11) * PRIME1, p + 1, len - 1)) :
							(mix1(mix1(mix1(h, PRIME2, 33), PRIME3, 29), 1, 32))));
			}
			static constexpr uint64 h32bytes(const char* p, uint64 len, const uint64 v1, const uint64 v2, const uint64 v3, const uint64 v4)
			{
				return (len >= 32) ? h32bytes(p + 32, len - 32, fetch64(p, v1), fetch64(p + 8, v2), fetch64(p + 16, v3), fetch64(p + 24, v4)) :
					mix3(mix3(mix3(mix3(rotl(v1, 1) + rotl(v2, 7) + rotl(v3, 12) + rotl(v4, 18), v1), v2), v3), v4);
			}
			static constexpr uint64 h32bytes(const char* p, uint64 len, const uint64 seed)
			{
				return h32bytes(p, len, seed + PRIME1 + PRIME2, seed + PRIME2, seed, seed - PRIME1);
			}
		};


		struct xxh32
		{
			static constexpr uint32 hash(const char* input, uint32 len, uint32 seed)
			{
				return finalize((len >= 16 ? h16bytes(input, len, seed) : seed + PRIME5) + len, (input)+(len & ~0xF), len & 0xF);
			}

		private:
			static constexpr uint32 PRIME1 = 0x9E3779B1U;
			static constexpr uint32 PRIME2 = 0x85EBCA77U;
			static constexpr uint32 PRIME3 = 0xC2B2AE3DU;
			static constexpr uint32 PRIME4 = 0x27D4EB2FU;
			static constexpr uint32 PRIME5 = 0x165667B1U;

			// 32-bit rotate left.
			static constexpr uint32 rotl(uint32 x, int r)
			{
				return ((x << r) | (x >> (32 - r)));
			}
			// Normal stripe processing routine.
			static constexpr uint32 round(uint32 acc, const uint32 input)
			{
				return rotl(acc + (input * PRIME2), 13) * PRIME1;
			}

			static constexpr uint32 avalanche_step(const uint32 h, const int rshift, const uint32 prime)
			{
				return (h ^ (h >> rshift)) * prime;
			}
			// Mixes all bits to finalize the hash.
			static constexpr uint32 avalanche(const uint32 h)
			{
				return avalanche_step(avalanche_step(avalanche_step(h, 15, PRIME2), 13, PRIME3), 16, 1);
			}

#ifdef XXH32_BIG_ENDIAN
			static constexpr uint32 endian32(const char* v)
			{
				return uint32(uint8(v[3])) | (uint32(uint8(v[2])) << 8)
					| (uint32(uint8(v[1])) << 16) | (uint32(uint8(v[0])) << 24);
			}
#else
			static constexpr uint32 endian32(const char* v)
			{
				return uint32(uint8(v[0])) | (uint32(uint8(v[1])) << 8)
					| (uint32(uint8(v[2])) << 16) | (uint32(uint8(v[3])) << 24);
			}
#endif // XXH32_BIG_ENDIAN

			static constexpr uint32 fetch32(const char* p, const uint32 v)
			{
				return round(v, endian32(p));
			}

			// Processes the last 0-15 bytes of p.
			static constexpr uint32 finalize(const uint32 h, const char* p, uint32 len)
			{
				return
					(len >= 4) ? finalize(rotl(h + (endian32(p) * PRIME3), 17) * PRIME4, p + 4, len - 4) :
					(len > 0) ? finalize(rotl(h + (uint8(*p) * PRIME5), 11) * PRIME1, p + 1, len - 1) :
					avalanche(h);
			}

			static constexpr uint32 h16bytes(const char* p, uint32 len, const uint32 v1, const uint32 v2, const uint32 v3, const uint32 v4)
			{
				return
					(len >= 16) ? h16bytes(p + 16, len - 16, fetch32(p, v1), fetch32(p + 4, v2), fetch32(p + 8, v3), fetch32(p + 12, v4)) :
					rotl(v1, 1) + rotl(v2, 7) + rotl(v3, 12) + rotl(v4, 18);
			}
			static constexpr uint32 h16bytes(const char* p, uint32 len, const uint32 seed)
			{
				return h16bytes(p, len, seed + PRIME1 + PRIME2, seed + PRIME2, seed, seed - PRIME1);
			}
		};
	}
}
