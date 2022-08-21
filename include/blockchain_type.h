#pragma once

#include <cstdint>
#include <array>

#define TRUNCATE_BIT_LENGTH 160

struct uint160_t {
    uint32_t lower_bytes;
    uint64_t center_bytes;
    uint64_t upper_bytes;

    uint160_t() = default;
    uint160_t(uint32_t n);
    uint160_t(uint32_t lower_bytes, uint64_t center_bytes, uint64_t upper_bytes);
    uint160_t(const uint160_t&) = default;
    uint160_t operator++(int x);
};

struct Transaction {
    uint160_t transaction_id;
    uint8_t data[108];

    Transaction(const uint160_t transaction_id, const uint8_t *data);
    Transaction(const Transaction& other);
};

using Transactions_t = std::array<Transaction, 8192>;

struct Block {
    static uint160_t next_block_id;

    uint160_t block_id;
    uint160_t hash_value;
    Transactions_t transactions;

    Block(uint160_t &hash_value, Transactions_t &transactions);

    uint160_t hash_block();
};
