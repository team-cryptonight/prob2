#include <cstdint>
#include <random>
#include <iostream>

#include "cryptopp/sha3.h"

#include "blockchain.h"
#include "blockchain_type.h"
#include "base58.h"
#include "merkle_tree.h"

union RandomData
{
    uint8_t bytes[108];
    uint32_t words[27];
};

Transactions_t& generate_random_transactions(Transactions_t &transactions, std::mt19937 &gen)
{
    std::vector<uint8_t> vch;
    vch.reserve(20);

    CryptoPP::SHA3_256 hash;

    RandomData random_data;
    for (auto &transaction: transactions)
    {
        for (auto &word: random_data.words)
        {
            word = gen();
        }

        hash.Update(random_data.bytes, 108);
        hash.TruncatedFinal(vch.data(), TRUNCATE_BYTE_LENGTH);

        uint160_t txid(vch);
        transaction = Transaction(txid, random_data.bytes);
    }

    return transactions;
}

int main()
{
    std::mt19937 gen(823);

    RandomData random_data;

    Transactions_t transactions;

    Prover prover;

    transactions = generate_random_transactions(transactions, gen);
    Block genesis_block = generate_genesis_block(transactions, prover);

    transactions = generate_random_transactions(transactions, gen);
    Block first_block = generate_next_block(genesis_block, transactions, prover);

    std::vector<Transaction> transactions_to_verify;

    std::cout << first_block.hash_value << std::endl;
    std::cout << first_block.merkle_root << std::endl;

    transactions_to_verify.push_back(first_block.transactions[3]);
    transactions_to_verify.push_back(first_block.transactions[127]);

    std::cout << verify_blocks(transactions_to_verify, first_block, prover) << std::endl;

    return 0;
}