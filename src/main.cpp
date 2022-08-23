#include <cstdint>
#include <random>
#include <iostream>

#include "blockchain.h"
#include "blockchain_type.h"

union RandomData
{
    uint8_t bytes[108];
    uint32_t words[27];
};

RandomData& generate_random_data(RandomData &random_data, std::mt19937 &gen)
{
    for (int i = 0; i < 27; i++)
    {
        random_data.words[i] = gen();
    }

    return random_data;
}

int main()
{
    std::mt19937 gen(823);

    uint32_t cnt = 0;
    RandomData random_data;

    random_data = generate_random_data(random_data, gen);

    Transactions_t transactions;
    for (auto &transaction: transactions)
    {
        random_data = generate_random_data(random_data, gen);
        transaction = Transaction(cnt++, random_data.bytes);
    }

    Block genesis_block = generate_genesis_block(transactions);

    std::cout << std::hex 
        << genesis_block.hash_value.upper_bytes 
        << genesis_block.hash_value.center_bytes 
        << genesis_block.hash_value.lower_bytes 
        << std::endl;

    return 0;
}