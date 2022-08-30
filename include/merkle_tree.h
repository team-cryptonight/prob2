#pragma once

#include <deque>
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
    std::deque<std::pair<MerkleProofNodeType, MerkleProofNodeType> > proof_tree;
    std::deque<uint160_t> skipped_hashes;
    std::vector<size_t> txid_perm;
};

class MerkleTree
{
    std::unordered_map<uint160_t, size_t, TxIdHasher> txid_to_index;
    std::array<uint160_t, (NUM_TX_PER_BLOCK << 1)> tree;

public:
    MerkleTree(const Block &block);

    MerkleProof get_proof(std::vector<uint160_t> &txids);
};

class Prover
{
    std::vector<MerkleTree> trees;

public:
    void add_tree(Block &block);
    MerkleProof get_proof(std::vector<uint160_t> &txids, size_t block_id);
};