#include "merkle_tree.h"

#include <stack>
#include <unordered_map>
#include <vector>

#include "cryptopp/sha3.h"

// MerkleTree
MerkleTree::MerkleTree(const Transactions_t &transactions)
{
    CryptoPP::SHA3_256 hash;

    uint160_t hash_value;

    size_t index = NUM_TX_PER_BLOCK;

    for (auto &transaction : transactions)
    {
        hash.Update(transaction.id.bytes, sizeof(transaction.id.bytes));
        hash.Update(transaction.data, sizeof(transaction.data));
        hash_helper(hash_value, hash);

        txid_to_index.emplace(transaction.id, index);
        tree[index++] = hash_value;
    }

    for (index = NUM_TX_PER_BLOCK - 1; index > 0; index--)
    {
        hash.Update(tree[index << 1].bytes, UINT160_BYTE_LENGTH);
        hash.Update(tree[(index << 1) + 1].bytes, UINT160_BYTE_LENGTH);
        hash_helper(hash_value, hash);

        tree[index] = hash_value;
    }
}

uint160_t MerkleTree::get_root()
{
    return tree[1];
}

MerkleProof MerkleTree::get_proof(std::vector<uint160_t> &txids)
{
    MerkleProof proof;

    MerkleProofNodeType nodes[(NUM_TX_PER_BLOCK << 1)];
    std::fill_n(nodes, (NUM_TX_PER_BLOCK << 1), MerkleProofNodeType::skip);

    std::unordered_map<size_t, size_t> index_to_txid_order;

    size_t tx_cnt = 0;

    for (auto &txid : txids)
    {
        auto it = txid_to_index.find(txid);

        // txid not found; wrong block
        if (it == txid_to_index.end()) {
            // return empty proof
            uint160_t u0;

            proof.proof_tree.emplace(MerkleProofNodeType::skip, MerkleProofNodeType::skip);
            proof.skipped_hashes.push(u0);
            proof.skipped_hashes.push(u0);

            return proof;
        }

        size_t index = it->second;
        index_to_txid_order.emplace(index, tx_cnt);
        nodes[index] = MerkleProofNodeType::verify;

        index >>= 1;

        // mark intermediate nodes in path as descend
        while (index > 0 && nodes[index] != MerkleProofNodeType::descend)
        {
            nodes[index] = MerkleProofNodeType::descend;
            index >>= 1;
        }

        tx_cnt++;
    }

    std::stack<size_t> preorder_stack;
    preorder_stack.push(1);

    // preorder traversal to construct proof tree
    while (!preorder_stack.empty())
    {
        size_t index = preorder_stack.top();
        preorder_stack.pop();

        switch (nodes[index])
        {
        case MerkleProofNodeType::descend:
            proof.proof_tree.emplace(nodes[index << 1], nodes[(index << 1) + 1]);
            preorder_stack.push((index << 1) + 1);
            preorder_stack.push(index << 1);
            break;
        case MerkleProofNodeType::skip:
            proof.skipped_hashes.push(tree[index]);
            break;
        case MerkleProofNodeType::verify:
            proof.txid_perm.push(index_to_txid_order.at(index));
            break;
        default:
            break;
        }
    }

    return proof;
}

// Prover
uint160_t Prover::add_tree(const Transactions_t &transactions)
{
    trees.emplace_back(MerkleTree(transactions));
    return trees.back().get_root();
}

MerkleProof Prover::get_proof(std::vector<uint160_t> &txids, size_t block_id)
{
    return trees[block_id].get_proof(txids);
}