#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#define TRUNCATE_BYTE_LENGTH 20

struct uint160_t {
    uint8_t bytes[20] = {0, };

    uint160_t() = default;
    uint160_t(uint32_t n);
    uint160_t(const std::vector<uint8_t> &vch);
    uint160_t(const uint160_t &other);
    uint160_t operator++(int x);

    bool operator==(const uint160_t &other);

    friend std::ostream& operator<<(std::ostream &os, const uint160_t &i160);
};

struct Transaction {
    uint160_t transaction_id;
    uint8_t data[108];

    Transaction() = default;
    Transaction(const uint160_t transaction_id, const uint8_t *data);
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
