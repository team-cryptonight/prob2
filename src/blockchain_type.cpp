#include "blockchain_type.h"

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha3.h"

uint160_t::uint160_t(uint32_t n)
{
    lower_bytes = n;
}

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


uint160_t block::next_block_id = 0;

uint160_t block::hash_block() {
    CryptoPP::SHA3_256 hash;
    
    hash.Update((const CryptoPP::byte*)block_id.lower_bytes, sizeof(uint32_t));
    hash.Update((const CryptoPP::byte*)block_id.center_bytes, sizeof(uint64_t));
    hash.Update((const CryptoPP::byte*)block_id.upper_bytes, sizeof(uint64_t));

    hash.Update((const CryptoPP::byte*)hash_value.lower_bytes, sizeof(uint32_t));
    hash.Update((const CryptoPP::byte*)hash_value.center_bytes, sizeof(uint64_t));
    hash.Update((const CryptoPP::byte*)hash_value.upper_bytes, sizeof(uint64_t));

    for (auto &transaction: transactions) {
        hash.Update((const CryptoPP::byte*)transaction.transaction_id.lower_bytes, sizeof(uint32_t));
        hash.Update((const CryptoPP::byte*)transaction.transaction_id.center_bytes, sizeof(uint64_t));
        hash.Update((const CryptoPP::byte*)transaction.transaction_id.upper_bytes, sizeof(uint64_t));
        hash.Update((const CryptoPP::byte*)transaction.data, sizeof(uint8_t)*108);
    }

    std::string digest;
    digest.resize(160);
    hash.TruncatedFinal((CryptoPP::byte*)&digest[0], digest.size());
    
}