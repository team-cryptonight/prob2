#pragma once

#include <queue>
#include <unordered_map>
#include <vector>

#include "blockchain_type.h"

class TxIdHasher
{
public:
    size_t operator() (const uint160_t &u160) const
    {
        return *reinterpret_cast<const size_t *>(u160.bytes);
    }
};

enum class MerkleProofNodeType
{
    descend,
    skip,
    verify
};

struct MerkleProof
{
    std::queue<std::pair<MerkleProofNodeType, MerkleProofNodeType> > proof_tree;
    std::queue<uint160_t> skipped_hashes;
    std::queue<size_t> txid_perm;
};

class MerkleTree
{
    std::unordered_map<uint160_t, size_t, TxIdHasher> txid_to_index;
    std::array<uint160_t, (NUM_TX_PER_BLOCK << 1)> tree;

public:
    MerkleTree(const Transactions_t &transactions);

    uint160_t get_root();
    MerkleProof get_proof(std::vector<uint160_t> &txids);
};

class Prover
{
    std::vector<MerkleTree> trees;

public:
    uint160_t add_tree(const Transactions_t &transactions);
    MerkleProof get_proof(std::vector<uint160_t> &txids, size_t block_id);
};