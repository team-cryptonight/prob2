#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#include "cryptopp/sha3.h"

#define UINT160_BYTE_LENGTH 20
#define TRUNCATE_BYTE_LENGTH 20
#define NUM_TX_PER_BLOCK 8192
#define TX_DATA_BYTE_LENGTH 108

struct uint160_t {
    uint8_t bytes[UINT160_BYTE_LENGTH] = {0, };  // bytes[0] is the least significant byte

    uint160_t() = default;
    uint160_t(const uint32_t n);
    uint160_t(const uint160_t &other);
    uint160_t operator++(int x);

    // https://github.com/bitcoin/bitcoin/blob/master/src/uint256.h 
    inline int compare_with(const uint160_t &other) const { return memcmp(bytes, other.bytes, sizeof(bytes)); }

    friend inline bool operator==(const uint160_t &a, const uint160_t &b) { return a.compare_with(b) == 0; }
    friend inline bool operator!=(const uint160_t &a, const uint160_t &b) { return a.compare_with(b) != 0; }
    friend inline bool operator<(const uint160_t &a, const uint160_t &b) { return a.compare_with(b) < 0; }

    friend std::ostream& operator<<(std::ostream &os, const uint160_t &u160);
};

struct Transaction {
    uint160_t id;
    uint8_t data[TX_DATA_BYTE_LENGTH];

    Transaction() = default;
    Transaction(const uint160_t id, const uint8_t *data);
    Transaction(const Transaction& other);
};

using Transactions_t = std::array<Transaction, NUM_TX_PER_BLOCK>;

struct Block {
    static uint160_t next_block_id;

    const uint160_t block_id;
    const uint160_t hash_value;
    const uint160_t merkle_root;
    Transactions_t transactions;

    Block(uint160_t &hash_value, uint160_t &merkle_root, Transactions_t &transactions);

    uint160_t hash_block();
};

void hash_helper(uint160_t &u160, CryptoPP::SHA3_256 &hash);
