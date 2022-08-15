#include "blockchain.h"

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha3.h"

block generate_genesis_block(transactions_t transactions)
{
    uint160_t u0 = 0;
    return block(u0, transactions);
}

block generate_next_block(block &prev_block, transactions_t transactions) {
    CryptoPP::SHA3_256 hash;
    hash.Update((const CryptoPP::byte*)transactions, )
}