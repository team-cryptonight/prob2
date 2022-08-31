#pragma once

#include <vector>

#include "blockchain_type.h"
#include "merkle_tree.h"

Block generate_genesis_block(Transactions_t &transactions, Prover &prover);

Block generate_next_block(Block &prev_block, Transactions_t &transactions, Prover &prover);

bool verify_transactions(std::vector<Transaction> &transactions, const Block &block, Prover &prover);