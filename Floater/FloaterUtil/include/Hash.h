#pragma once
#include "FloaterType.h"
#include <intrin.h>
#include <memory>
#pragma intrinsic(_umul128)

namespace flt
{
	struct wyhash
	{
	public:
		struct State
		{
			uint64 initSeed;
			uint64 seed;
			uint64 secret[4];
		};

	public:
		wyhash() : _initSeed(UINT64_MAX), _seed(0), _secret{ 0x2d358dccaa6c78a5ull, 0x8bb84b93962eacc9ull, 0x4b33a62ed433d4a3ull, 0x4d5a2da51de1aa47ull } {}
		wyhash(uint64 seed) : _initSeed(0), _seed(seed), _secret{ 0 }
		{
			make_secret();
		}
		wyhash(State state) :
			_initSeed(state.initSeed), _seed(state.seed), _secret{ state.secret[0], state.secret[1], state.secret[2], state.secret[3] } {}

		inline uint64_t operator()(const void* key, size_t len)
		{
			uint64 seed = _seed;
			const uint8_t* p = (const uint8_t*)key;

			seed ^= _mix(seed ^ _secret[0], _secret[1]);
			uint64_t a, b;
			if (len <= 16)
			{
				if (len >= 4)
				{
					a = (_r4(p) << 32) | _r4(p + ((len >> 3) << 2));
					b = (_r4(p + len - 4) << 32) | _r4(p + len - 4 - ((len >> 3) << 2));
				}
				else if (len > 0)
				{
					a = _r3(p, len); b = 0;
				}
				else
				{
					a = b = 0;
				}
			}
			else
			{
				size_t i = len;
				if (i >= 48)
				{
					uint64_t see1 = seed, see2 = seed;
					do
					{
						seed = _mix(_r8(p) ^ _secret[1], _r8(p + 8) ^ seed);
						see1 = _mix(_r8(p + 16) ^ _secret[2], _r8(p + 24) ^ see1);
						see2 = _mix(_r8(p + 32) ^ _secret[3], _r8(p + 40) ^ see2);
						p += 48; i -= 48;
					} while (i >= 48);
					seed ^= see1 ^ see2;
				}
				while (i > 16)
				{
					seed = _mix(_r8(p) ^ _secret[1], _r8(p + 8) ^ seed);
					i -= 16; p += 16;
				}
				a = _r8(p + i - 16);  b = _r8(p + i - 8);
			}

			a ^= _secret[1];
			b ^= seed;
			_mum(&a, &b);
			return  _mix(a ^ _secret[0] ^ len, b ^ _secret[1]);
		}

		void GetState(State* outState)
		{
			ASSERT(outState, "nullptr");
			outState->seed = _seed;
			outState->secret[0] = _secret[0];
			outState->secret[1] = _secret[1];
			outState->secret[2] = _secret[2];
			outState->secret[3] = _secret[3];
			outState->initSeed = _initSeed;
		}
	private:
		const uint64 _initSeed;
		uint64 _seed;
		uint64 _secret[4];

		inline uint64 _rot(uint64 x)
		{
			return (x >> 32) | (x << 32);
		}
		inline void _mum(uint64* A, uint64* B)
		{
			*A = _umul128(*A, *B, B);
		}
		inline uint64 _mix(uint64 A, uint64 B)
		{
			_mum(&A, &B);
			return A ^ B;
		}
		inline uint64 _r8(const uint8* p)
		{
			uint64 v;
			memcpy(&v, p, 8);
			return v;
		}
		inline uint64 _r4(const uint8* p)
		{
			uint32 v;
			memcpy(&v, p, 4);
			return v;
		}
		inline uint64 _r3(const uint8* p, size_t k)
		{
			return (((uint64)p[0]) << 16) | (((uint64)p[k >> 1]) << 8) | p[k - 1];
		}

		inline uint64_t hash64(uint64_t A, uint64_t B)
		{
			A ^= 0x2d358dccaa6c78a5ull;
			B ^= 0x8bb84b93962eacc9ull;
			_mum(&A, &B);
			return _mix(A ^ 0x2d358dccaa6c78a5ull, B ^ 0x8bb84b93962eacc9ull);
		}
		inline uint64_t wyrand()
		{
			//uint64 seed = _seed;
			_seed += 0x2d358dccaa6c78a5ull;
			return _mix(_seed, _seed ^ 0x8bb84b93962eacc9ull);
		}
		inline void make_secret()
		{
			uint8 c[] = { 15, 23, 27, 29, 30, 39, 43, 45, 46, 51, 53, 54, 57, 58, 60, 71, 75, 77, 78, 83, 85, 86, 89, 90, 92, 99, 101, 102, 105, 106, 108, 113, 114, 116, 120, 135, 139, 141, 142, 147, 149, 150, 153, 154, 156, 163, 165, 166, 169, 170, 172, 177, 178, 180, 184, 195, 197, 198, 201, 202, 204, 209, 210, 212, 216, 225, 226, 228, 232, 240 };
			for (size_t i = 0; i < 4; i++)
			{
				uint8 ok = 0;
				do
				{
					ok = 1;
					_secret[i] = 0;
					for (size_t j = 0; j < 64; j += 8)
					{
						_secret[i] |= ((uint64)c[wyrand() % sizeof(c)]) << j;
					}

					// _secret[i] must odd
					if (_secret[i] % 2 == 0)
					{
						ok = 0;
						continue;
					}

					for (size_t j = 0; j < i; j++)
					{
						// _secret[i] must different 32bit with _secret[j]
						if (_mm_popcnt_u64(_secret[j] ^ _secret[i]) != 32)
						{
							ok = 0;
							break;
						}
					}
					if (!ok)
					{
						continue;
					}

					for (uint64 k = 3; k < 0x1'0000'0000ull; k += 2)
					{
						// _secret[i] must prime
						if ((_secret[i] % k) == 0)
						{
							ok = 0;
							break;
						}
					}
				} while (!ok);
			}
		}
	};

	struct dhb2
	{
		inline uint32_t operator()(const void* key, size_t len)
		{
			const uint8_t* p = (const uint8_t*)key;
			uint32_t hash = 5381;
			for (size_t i = 0; i < len; i++)
			{
				// hash * 33 + p[i]
				hash = ((hash << 5) + hash) + p[i];
			}
			return hash;
		}
	};
}
