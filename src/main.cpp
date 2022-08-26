#include <cstdint>
#include <random>
#include <iostream>

#include "blockchain.h"
#include "blockchain_type.h"
#include "base58.h"

union RandomData
{
    uint8_t bytes[108];
    uint32_t words[27];
};

Transactions_t& generate_random_transactions(Transactions_t &transactions, std::mt19937 &gen)
{
    uint32_t cnt = 0;

    RandomData random_data;
    for (auto &transaction: transactions)
    {
        for (auto &word: random_data.words)
        {
            word = gen();
        }
        transaction = Transaction(cnt++, random_data.bytes);
    }

    return transactions;
}

int main()
{
    std::mt19937 gen(823);

    uint32_t cnt = 0;
    RandomData random_data;

    Transactions_t transactions;

    transactions = generate_random_transactions(transactions, gen);
    Block genesis_block = generate_genesis_block(transactions);

    transactions = generate_random_transactions(transactions, gen);
    Block first_block = generate_next_block(genesis_block, transactions);

    transactions = generate_random_transactions(transactions, gen);
    Block second_block = generate_next_block(first_block, transactions);

    transactions = generate_random_transactions(transactions, gen);
    Block third_block = generate_next_block(second_block, transactions);

    std::cout << second_block.hash_value << std::endl;
    std::cout << third_block.hash_value << std::endl;

    std::cout << verify_blocks(second_block, third_block) << std::endl;
    
    second_block.transactions[128].data[64]++;

    std::cout << verify_blocks(second_block, third_block) << std::endl;

    uint x;
    std::cin >> x;

    uint160_t c(x);

    std::cout << EncodeBase58(c) << std::endl;  
    std::cout << EncodeBase58(second_block.hash_value) << std::endl;  



    return 0;
}