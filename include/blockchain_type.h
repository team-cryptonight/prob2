#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#define UINT160_BYTE_LENGTH 20
#define TRUNCATE_BYTE_LENGTH 20

struct uint160_t {
    uint8_t bytes[UINT160_BYTE_LENGTH] = {0, };

    uint160_t() = default;
    uint160_t(uint32_t n);
    uint160_t(const std::vector<uint8_t> &vch);
    uint160_t(const uint160_t &other);
    uint160_t operator++(int x);

    inline int compare_with(const uint160_t &other) const { return memcmp(bytes, other.bytes, sizeof(bytes)); }

    friend inline bool operator==(const uint160_t &a, const uint160_t &b) { return a.compare_with(b) == 0; }
    friend inline bool operator!=(const uint160_t &a, const uint160_t &b) { return a.compare_with(b) != 0; }
    friend inline bool operator<(const uint160_t &a, const uint160_t &b) { return a.compare_with(b) < 0; }

    friend std::ostream& operator<<(std::ostream &os, const uint160_t &i160);
};

struct Transaction {
    uint160_t id;
    uint8_t data[108];

    Transaction() = default;
    Transaction(const uint160_t id, const uint8_t *data);
    Transaction(const Transaction& other);
};

using Transactions_t = std::array<Transaction, 8192>;

struct Block {
    static uint160_t next_block_id;

    const uint160_t block_id;
    const uint160_t hash_value;
    Transactions_t transactions;

    Block(uint160_t &hash_value, Transactions_t &transactions);

    uint160_t hash_block();
};
