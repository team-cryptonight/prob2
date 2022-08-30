#include "blockchain_type.h"

#include <algorithm>
#include <iomanip>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha3.h"

// uint160_t
uint160_t::uint160_t(uint32_t n) 
{
    *reinterpret_cast<uint32_t *>(bytes) = n;
}

uint160_t::uint160_t(const std::vector<uint8_t> &vch)
{
    memcpy(bytes, vch.data(), sizeof(bytes));
}

uint160_t uint160_t::operator++(int x)
{
    for (auto &byte: bytes)
    {
        byte++;
        if (byte)
        {
            return *this;
        }
    }

    return *this;
}

bool uint160_t::operator==(const uint160_t &other)
{
    return memcmp(bytes, other.bytes, sizeof(bytes)) == 0;
}

std::ostream &
operator<<(std::ostream &os, const uint160_t &i160)
{
    os << std::setfill('0') 
       << std::setw(8) 
       << std::hex;

    for (int offset = 0; offset < 20; offset += 4)
    {
        os << *reinterpret_cast<const uint32_t *>(i160.bytes + offset);
    }

    return os;
}

// Transaction
Transaction::Transaction(const uint160_t transaction_id, const uint8_t *data) : transaction_id(transaction_id)
{
    for (int i = 0; i < 108; i++)
    {
        this->data[i] = data[i];
    }
}

Transaction::Transaction(const Transaction &other) : Transaction(other.transaction_id, other.data) {}

// Block
Block::Block(uint160_t &hash_value, Transactions_t &transactions) : block_id(next_block_id++), hash_value(hash_value), transactions(transactions)
{
}

uint160_t Block::next_block_id = 0;

uint160_t Block::hash_block()
{
    CryptoPP::SHA3_256 hash;

    hash.Update(static_cast<const CryptoPP::byte *>(block_id.bytes), sizeof(block_id.bytes));
    hash.Update(static_cast<const CryptoPP::byte *>(hash_value.bytes), sizeof(hash_value.bytes));

    for (auto &transaction : transactions)
    {
        hash.Update(static_cast<const CryptoPP::byte *>(transaction.transaction_id.bytes), sizeof(transaction.transaction_id.bytes));
        hash.Update((const CryptoPP::byte *)transaction.data, sizeof(uint8_t) * 108);
    }

    std::vector<uint8_t> vch = std::vector<uint8_t>();
    vch.reserve(TRUNCATE_BYTE_LENGTH);

    hash.TruncatedFinal((CryptoPP::byte *)(vch.data()), TRUNCATE_BYTE_LENGTH);

    return uint160_t(vch);
}