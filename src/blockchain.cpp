#include "blockchain.h"

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha3.h"

Block generate_genesis_block(Transactions_t &transactions)
{
    uint160_t u0 = 0;
    return Block(u0, transactions);
}

Block generate_next_block(Block &prev_block, Transactions_t &transactions) {
    uint160_t prev_hash = prev_block.hash_block();
    return Block(prev_hash, transactions);
}

bool verify_blocks(Block target)
{
    return false;
}