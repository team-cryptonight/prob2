#include "merkle_tree.h"

#include <stack>
#include <unordered_map>
#include <vector>

#include "cryptopp/sha3.h"

// MerkleTree
MerkleTree::MerkleTree(const Block &block)
{
    CryptoPP::SHA3_256 hash;

    std::vector<uint8_t> vch;
    vch.reserve(TRUNCATE_BYTE_LENGTH);

    size_t index = NUM_TX_PER_BLOCK;

    for (auto &transaction : block.transactions)
    {
        hash.Update(transaction.id.bytes, sizeof(transaction.id.bytes));
        hash.Update(transaction.data, sizeof(transaction.data));
        hash.TruncatedFinal(vch.data(), TRUNCATE_BYTE_LENGTH);

        txid_to_index.emplace(transaction.id, index);
        tree[index++] = uint160_t(vch);
    }

    for (index = NUM_TX_PER_BLOCK - 1; index > 0; index--)
    {
        hash.Update(tree[index << 1].bytes, UINT160_BYTE_LENGTH);
        hash.Update(tree[(index << 1) + 1].bytes, UINT160_BYTE_LENGTH);
        hash.TruncatedFinal(vch.data(), TRUNCATE_BYTE_LENGTH);

        tree[index] = uint160_t(vch);
    }
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
        size_t index = txid_to_index.at(txid);
        index_to_txid_order.emplace(index, tx_cnt);
        nodes[index] = MerkleProofNodeType::verify;

        index >>= 1;

        while (index > 0 && nodes[index] != MerkleProofNodeType::descend)
        {
            nodes[index] = MerkleProofNodeType::descend;
            index >>= 1;
        }

        tx_cnt++;
    }

    std::stack<size_t> preorder_stack;
    preorder_stack.push(1);

    while (!preorder_stack.empty())
    {
        size_t index = preorder_stack.top();
        preorder_stack.pop();

        switch (nodes[index])
        {
        case MerkleProofNodeType::descend:
            proof.proof_tree.emplace_back(nodes[index << 1], nodes[(index << 1) + 1]);
            preorder_stack.push((index << 1) + 1);
            preorder_stack.push(index << 1);
            break;
        case MerkleProofNodeType::skip:
            proof.skipped_hashes.push_back(tree[index]);
            break;
        case MerkleProofNodeType::verify:
            proof.txid_perm.push_back(index_to_txid_order.at(index));
            break;
        default:
            break;
        }
    }

    return proof;
}

// Prover
void Prover::add_tree(Block &block)
{
    trees.push_back(MerkleTree(block));
}

MerkleProof Prover::get_proof(std::vector<uint160_t> &txids, size_t block_id)
{
    return trees[block_id].get_proof(txids);
}