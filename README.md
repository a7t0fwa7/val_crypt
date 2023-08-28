# valcrypt [![](https://img.shields.io/badge/version-1.0.0-green.svg)]()

r-value encryption, compile-time and based on intrins. single-header and easy-to-use call

## Hide your important data
```cpp
sockaddr_in serverAddress;
serverAddress.sin_port = htons(valcrypt(12345u));
```

## Simple example
```cpp
int main()
{
    std::uint32_t value = valcrypt(61294918);

    std::cout << "Crypted value result is: " << value << std::endl;
}
```
And as expected the output is:
`Crypted value result is: 61294918`

## Features

- Ability to use SSE intrins.
- Ability to use AVX intrins.
- Compile-time value encryption.
- Doesn't leave any imports in the executable.

## API

- `valcrypt(rvalue)      -> decrypt already crypted value`
- `valcrypt_get(rvalue)  -> crypt class getter, contains a few methods`

## Configuration

| `#define`                                 | EFFECT                                                                                  |
| ----------------------------------------- | --------------------------------------------------------------------------------------- |
| `VALCRYPT_USE_ONLY_AVX_INTRINS`           | forcing to use only AVX intrins in crypting operations                                  |
| `VALCRYPT_NO_FORCEINLINE`                 | disables force inlining                                                                 |

At the compilation-time `xor` operations are performed on `value`, during run-time the same `xor` operations are performed on `value` again, but based on intrins (SSE/AVX). 
This repository is designed to make it a little more difficult for a reverse engineer to analyze your program, as well as to protect important r-value int constants.

## IDA Pro Pseudocode Output (code from example)
```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  unsigned int v3; // ebx
  int i; // eax
  __int64 v5; // rdx
  unsigned int v6; // ecx
  __m128i v7; // xmm2
  __m128i v8; // xmm0
  int v9; // ecx
  __m128i v10; // xmm1
  __m128i v11; // xmm2
  __int64 v12; // rax
  __int64 v13; // rax

  v3 = 29495662;
  for ( i = 1; i <= 7; ++i )
  {
    v5 = (unsigned int)_mm_cvtsi128_si32(_mm_shuffle_epi32(_mm_cvtsi32_si128(-1619594369 * i), 0));
    v6 = _mm_cvtsi128_si32(_mm_shuffle_epi32(_mm_cvtsi32_si128(952968912 * i), 0));
    if ( v6 != (_DWORD)v5 )
    {
      v7 = _mm_cvtsi32_si128(v6);
      v8 = _mm_cvtsi32_si128(v5);
      v6 = _mm_cvtsi128_si32(_mm_or_si128(_mm_and_si128(_mm_andnot_si128(v8, (__m128i)xmmword_1400032D0), v7), _mm_andnot_si128(v7, v8)));
    }
    if ( v6 == 952968912 )
    {
      v9 = 952968912;
    }
    else
    {
      v10 = _mm_cvtsi32_si128(v6);
      v11 = _mm_cvtsi32_si128(0x38CD26D0u);
      v9 = _mm_cvtsi128_si32(
             _mm_or_si128(
               _mm_and_si128(_mm_andnot_si128(v10, (__m128i)xmmword_1400032D0), v11),
               _mm_andnot_si128(v11, v10)));
    }
    v3 ^= v9;
  }
  v12 = sub_140001150(std::cout, v5, 952968912i64);
  v13 = std::ostream::operator<<(v12, v3);
  std::ostream::operator<<(v13, sub_140001300);
  return 0;
}
```

## Thanks again)) to:
invers1on :heart:, metafaze :heart:
