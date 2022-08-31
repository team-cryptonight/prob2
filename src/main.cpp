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
    uint160_t txid;

    CryptoPP::SHA3_256 hash;

    RandomData random_data;
    for (auto &transaction: transactions)
    {
        for (auto &word: random_data.words)
        {
            word = gen();
        }

        hash.Update(random_data.bytes, 108);
        hash.TruncatedFinal(txid.bytes, TRUNCATE_BYTE_LENGTH);

        transaction = Transaction(txid, random_data.bytes);
    }

    return transactions;
}

int main()
{
    // fixed seed for reproducibility
    std::mt19937 gen(823);

    Transactions_t transactions;

    Prover prover;

    // generate blockchain
    transactions = generate_random_transactions(transactions, gen);
    Block genesis_block = generate_genesis_block(transactions, prover);

    transactions = generate_random_transactions(transactions, gen);
    Block first_block = generate_next_block(genesis_block, transactions, prover);

    transactions = generate_random_transactions(transactions, gen);
    Block second_block = generate_next_block(first_block, transactions, prover);

    transactions = generate_random_transactions(transactions, gen);
    Block third_block = generate_next_block(second_block, transactions, prover);

    // verify transactions
    std::vector<Transaction> transactions_to_verify;

    transactions_to_verify.push_back(first_block.transactions[127]);
    transactions_to_verify.push_back(first_block.transactions[1023]);

    std::cout << verify_transactions(transactions_to_verify, first_block, prover) << std::endl;

    transactions_to_verify.clear();

    second_block.transactions[255].data[63]++;
    transactions_to_verify.push_back(second_block.transactions[255]);
    transactions_to_verify.push_back(second_block.transactions[2047]);

    std::cout << verify_transactions(transactions_to_verify, second_block, prover) << std::endl;

    return 0;
}