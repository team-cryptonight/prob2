#pragma once

#include "blockchain_type.h"

block generate_genesis_block(transactions_t transactions);

block generate_next_block(block &prev_block, transactions_t transactions);

bool verify_blocks(block target);