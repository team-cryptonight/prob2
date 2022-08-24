#include "blockchain_type.h"

#include <algorithm>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha3.h"

// uint160_t
uint160_t::uint160_t(uint32_t n): lower_bytes(n), center_bytes(0), upper_bytes(0)
{
}

uint160_t::uint160_t(uint32_t lower_bytes, uint64_t center_bytes, uint64_t upper_bytes)
    : lower_bytes(lower_bytes), center_bytes(center_bytes), upper_bytes(upper_bytes) {}

uint160_t uint160_t::operator++(int x)
{
    lower_bytes++;
    if (lower_bytes) {
        return *this;
    }

    center_bytes++;
    if (center_bytes) {
        return *this;
    }

    upper_bytes++;

    return *this;
}

std::ostream& operator<<(std::ostream &os, const uint160_t &i160)
{
    os  << i160.upper_bytes
        << i160.center_bytes
        << i160.lower_bytes;

    return os;
}


// Transaction
Transaction::Transaction(const uint160_t transaction_id, const uint8_t *data): transaction_id(transaction_id)
{
    for (int i = 0; i < 108; i++)
    {
        this->data[i] = data[i];
    }
}

Transaction::Transaction(const Transaction &other): Transaction(other.transaction_id, other.data) {}


// Block
Block::Block(uint160_t &hash_value, Transactions_t &transactions): block_id(next_block_id++), hash_value(hash_value), transactions(transactions)
{
}

uint160_t Block::next_block_id = 0;

uint160_t Block::hash_block() {
    CryptoPP::SHA3_256 hash;
    
    hash.Update((const CryptoPP::byte*)(&block_id.lower_bytes), sizeof(uint32_t));
    hash.Update((const CryptoPP::byte*)(&block_id.center_bytes), sizeof(uint64_t));
    hash.Update((const CryptoPP::byte*)(&block_id.upper_bytes), sizeof(uint64_t));

    hash.Update((const CryptoPP::byte*)(&hash_value.lower_bytes), sizeof(uint32_t));
    hash.Update((const CryptoPP::byte*)(&hash_value.center_bytes), sizeof(uint64_t));
    hash.Update((const CryptoPP::byte*)(&hash_value.upper_bytes), sizeof(uint64_t));

    for (auto &transaction: transactions) {
        hash.Update((const CryptoPP::byte*)(&transaction.transaction_id.lower_bytes), sizeof(uint32_t));
        hash.Update((const CryptoPP::byte*)(&transaction.transaction_id.center_bytes), sizeof(uint64_t));
        hash.Update((const CryptoPP::byte*)(&transaction.transaction_id.upper_bytes), sizeof(uint64_t));
        hash.Update((const CryptoPP::byte*)transaction.data, sizeof(uint8_t) * 108);
    }

    CryptoPP::byte digest[TRUNCATE_BYTE_LENGTH];
    hash.TruncatedFinal((CryptoPP::byte*)&digest[0], TRUNCATE_BYTE_LENGTH);

    return uint160_t(*reinterpret_cast<uint32_t*>(digest), *reinterpret_cast<uint64_t*>(digest + 4), *reinterpret_cast<uint64_t*>(digest + 12));
}