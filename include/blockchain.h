#pragma once

#include "blockchain_type.h"

Block generate_genesis_block(Transactions_t transactions);

Block generate_next_block(Block &prev_block, Transactions_t transactions);

bool verify_blocks(Block target);