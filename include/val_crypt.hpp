// [FAQ] here: https://github.com/annihilatorq/val_crypt

// Creator Discord - @ntraiseharderror, Telegram - https://t.me/ntraiseharderror, Github - https://github.com/annihilatorq.

/*
* At the compilation-time xor operations are performed on value,
* during run-time the same xor operations are performed on value
* again, but based on intrins (SSE/AVX).
* This repository is designed to make it a little more difficult
* for a reverse engineer to analyze your program, as well as to
* protect important int constants.
*/

#ifndef _VALUE_CRYPT_
#define _VALUE_CRYPT_

#define VALCRYPT_CRYPT_SEED (0x9F76F37F)

#include <cstdint>
#include <immintrin.h>

#ifndef VALCRYPT_NO_FORCEINLINE
#if defined(_MSC_VER)
#define VALCRYPT_FORCEINLINE __forceinline
#else
#define VALCRYPT_FORCEINLINE inline
#endif
#else
#define VALCRYPT_FORCEINLINE inline
#endif

#define valcrypt_get(rvalue) [&]() { std::integral_constant<decltype(rvalue), rvalue> temp; \
                                    val_crypt::detail::const_encryptor<decltype(rvalue), temp.value> instance; return instance; }()

#define valcrypt(rvalue) [&]() { std::integral_constant<decltype(rvalue), rvalue> temp;\
                                    val_crypt::detail::const_encryptor<decltype(rvalue), temp.value> instance; return instance.crypt_get(); }()

namespace val_crypt {
    namespace discrete_math
    {
		constexpr std::int32_t _xor_(
            std::int32_t p, std::int32_t q) noexcept(true) 
        {
			return (~p & q) | (p & ~q);
		}

		constexpr std::int32_t _and_(
            std::int32_t p, std::int32_t q) noexcept(true) 
        {
			return p & q;
		}

		namespace intrin
		{
            VALCRYPT_FORCEINLINE std::int32_t _xor_(
                std::int32_t p, std::int32_t q) noexcept(true) 
            {
                if (p == q) return p;

#ifndef VALCRYPT_USE_ONLY_AVX_INTRINS
				return _mm_cvtsi128_si32(_mm_or_si128(_mm_andnot_si128(_mm_cvtsi32_si128(p), _mm_cvtsi32_si128(q)),
					_mm_and_si128(_mm_cvtsi32_si128(p), _mm_andnot_si128(_mm_cvtsi32_si128(q), _mm_set1_epi32(-1)))));
#else

                return _mm_cvtsi128_si32(_mm256_castsi256_si128(_mm256_or_si256(_mm256_andnot_si256(_mm256_set1_epi32(p),
                    _mm256_set1_epi32(q)), _mm256_andnot_si256(_mm256_set1_epi32(q), _mm256_set1_epi32(p)))));
#endif
			}

            VALCRYPT_FORCEINLINE std::int32_t _mul_(
                std::int32_t p, std::int32_t q) noexcept(true) 
            {
                if (p == 0 || q == 0) return 0;

#ifndef VALCRYPT_USE_ONLY_AVX_INTRINS
                return _mm_cvtsi128_si32(_mm_mullo_epi32(_mm_set1_epi32(p), _mm_set1_epi32(q)));
#else
                return _mm_cvtsi128_si32(_mm256_castsi256_si128(_mm256_mullo_epi32(
                    _mm256_set1_epi32(p), _mm256_set1_epi32(q))));
#endif
            }
		};
    }

    namespace detail 
    {
        template <class _Type, _Type value>
        class const_encryptor 
        {
        public:
            const_encryptor() 
            { 
                decrypt();
            }

            constexpr VALCRYPT_FORCEINLINE _Type encrypt()
            {
                _Type val = value;

                for (auto i = 1; i <= CRYPTO_LOOP; ++i)
                {
                    val ^= (::val_crypt::discrete_math::_xor_(CRYPTO_SEED, ::val_crypt::discrete_math::_xor_(
                        i * CRYPTO_SEED, i * VALCRYPT_CRYPT_SEED)));
                }

                return val;
            }

            VALCRYPT_FORCEINLINE const _Type get() const noexcept(true)
            {
                return value;
            }

            VALCRYPT_FORCEINLINE _Type crypt_get() noexcept(true)
            {
                return crypted_value;
            }

        private:
            VALCRYPT_FORCEINLINE _Type decrypt() noexcept(true)
            {
                crypted_value = encrypt();

                for (auto i = 1; i <= CRYPTO_LOOP; ++i) 
                {
                    crypted_value ^= (::val_crypt::discrete_math::intrin::_xor_(CRYPTO_SEED, ::val_crypt::discrete_math::intrin::_xor_(
                        ::val_crypt::discrete_math::intrin::_mul_(i, CRYPTO_SEED), ::val_crypt::discrete_math::intrin::_mul_(i, VALCRYPT_CRYPT_SEED))));
                }

                return crypted_value;
            }

        private:
            _Type crypted_value = 0;

            enum : std::int32_t
            {
                CRYPTO_SEED = 952968912,
                CRYPTO_LOOP = 7
            };

            static_assert(CRYPTO_LOOP == 7, "Very not recommended to change, unless you know what you're doing.");
        };
    }
}

#endif
