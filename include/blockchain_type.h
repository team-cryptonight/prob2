#pragma once

#include <cstdint>
#include <array>

struct uint160_t {
    uint32_t lower_bytes;
    uint64_t center_bytes;
    uint64_t upper_bytes;

    uint160_t(uint32_t n);
    uint160_t(const uint160_t&) = default;
    uint160_t operator++(int x);
};

struct transaction {
    uint160_t transaction_id;
    uint8_t data[108];
};

using transactions_t = std::array<transaction, 8192>;

struct block {
    static uint160_t next_block_id;

    uint160_t block_id;
    uint160_t hash_value;
    transactions_t transactions;

    block(uint160_t &hash_value, transactions_t &transactions)
    : block_id(next_block_id++), hash_value(hash_value), transactions(transactions){}

    uint160_t hash_block();
};

