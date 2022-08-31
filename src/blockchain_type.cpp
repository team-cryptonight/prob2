#include "blockchain_type.h"

#include <algorithm>
#include <iomanip>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha3.h"

#include "base58.h"

// uint160_t
uint160_t::uint160_t(uint32_t n)
{
    *reinterpret_cast<uint32_t *>(bytes) = n;
}

uint160_t::uint160_t(const std::vector<uint8_t> &vch)
{
    memcpy(bytes, vch.data(), sizeof(bytes));
}

uint160_t::uint160_t(const uint160_t &other)
{
    memcpy(bytes, other.bytes, sizeof(bytes));
}

uint160_t uint160_t::operator++(int x)
{
    for (auto &byte : bytes)
    {
        byte++;
        if (byte)
        {
            return *this;
        }
    }

    return *this;
}

std::ostream &operator<<(std::ostream &os, const uint160_t &i160)
{
    os << EncodeBase58(i160);

    return os;
}

// Transaction
Transaction::Transaction(const uint160_t id, const uint8_t *data) : id(id)
{
    memcpy(this->data, data, TX_DATA_BYTE_LENGTH);
}

Transaction::Transaction(const Transaction &other) : Transaction(other.id, other.data) {}

// Block
Block::Block(uint160_t &hash_value, uint160_t &merkle_root, Transactions_t &transactions)
    : block_id(next_block_id), hash_value(hash_value), merkle_root(merkle_root), transactions(transactions)
{
    next_block_id++;
}

uint160_t Block::next_block_id = 0;

uint160_t Block::hash_block()
{
    CryptoPP::SHA3_256 hash;

    hash.Update(static_cast<const CryptoPP::byte *>(block_id.bytes), sizeof(block_id.bytes));
    hash.Update(static_cast<const CryptoPP::byte *>(hash_value.bytes), sizeof(hash_value.bytes));

    for (auto &transaction : transactions)
    {
        hash.Update(static_cast<const CryptoPP::byte *>(transaction.id.bytes), sizeof(transaction.id.bytes));
        hash.Update(static_cast<const CryptoPP::byte *>(transaction.data), sizeof(transaction.data));
    }

    std::vector<uint8_t> vch = std::vector<uint8_t>();
    vch.reserve(TRUNCATE_BYTE_LENGTH);

    hash.TruncatedFinal((CryptoPP::byte *)(vch.data()), TRUNCATE_BYTE_LENGTH);

    return uint160_t(vch);
}