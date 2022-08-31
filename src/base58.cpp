#include "base58.h"

#include <cstring>

const char *pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

// https://github.com/bitcoin/bitcoin/blob/master/src/base58.cpp
std::string EncodeBase58(const uint160_t &input)
{
    // Skip & count leading zeroes.
    unsigned char bytes[20];
    memcpy(bytes, input.bytes, sizeof(bytes));

    int zeroes = 0;
    int length = 0;

    for (int i = 19; i >= 0; i--)
    {
        if (bytes[i] == 0)
            zeroes++;
        else
            break;
    }

    if (zeroes == 20)
    {
        return "1";
    }

    // Allocate enough space in big-endian base58 representation.
    int size = (20 - zeroes) * 138 / 100 + 1; // log(256) / log(58), rounded up.
    unsigned char b58[size] = {
        0,
    };
    // Process the bytes.
    for (int t = 19 - zeroes; t >= 0; t--)
    {
        int carry = bytes[t];
        int j;
        for (j = 0; carry != 0 || j < length; j++)
        {
            carry += 256 * b58[j];
            b58[j] = carry % 58;
            carry /= 58;
        }
        length = j;
    }

    int b58_zeros = 0;
    for (int i = size - 1; i >= 0 && b58[i] == 0; i--)
    {
        b58_zeros++;
    }

    std::string str;
    str.reserve(size - b58_zeros);

    for (int i = size - b58_zeros - 1; i >= 0; i--)
        str += pszBase58[b58[i]];

    return str;
}