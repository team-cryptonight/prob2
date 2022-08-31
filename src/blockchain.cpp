#include "blockchain.h"

#include <vector>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha3.h"

Block generate_genesis_block(Transactions_t &transactions, Prover &prover)
{
    uint160_t u0 = 0;
    uint160_t merkle_root = prover.add_tree(transactions);
    return Block(u0, merkle_root, transactions);
}

Block generate_next_block(Block &prev_block, Transactions_t &transactions, Prover &prover)
{
    uint160_t prev_hash = prev_block.hash_block();
    uint160_t merkle_root = prover.add_tree(transactions);
    return Block(prev_hash, merkle_root, transactions);
}

uint160_t descend(std::vector<Transaction> &transactions, MerkleProof &proof, CryptoPP::SHA3_256 &hash)
{
    uint160_t merkle_root;
    auto proof_node = proof.proof_tree.front();
    proof.proof_tree.pop();

    uint160_t left_hash;
    uint160_t right_hash;

    switch(proof_node.first)
    {
    case MerkleProofNodeType::descend:
        left_hash = descend(transactions, proof, hash);
        break;
    case MerkleProofNodeType::skip:
        left_hash = proof.skipped_hashes.front();
        proof.skipped_hashes.pop();
        break;
    case MerkleProofNodeType::verify:
    {
        auto &transaction = transactions[proof.txid_perm.front()];
        proof.txid_perm.pop();

        hash.Update(static_cast<const CryptoPP::byte *>(transaction.id.bytes), UINT160_BYTE_LENGTH);
        hash.Update(transaction.data, sizeof(transaction.data));
        hash.TruncatedFinal(left_hash.bytes, TRUNCATE_BYTE_LENGTH);

        break;
    }
    default:
        break;
    }

    switch(proof_node.second)
    {
    case MerkleProofNodeType::descend:
        right_hash = descend(transactions, proof, hash);
        break;
    case MerkleProofNodeType::skip:
        right_hash = proof.skipped_hashes.front();
        proof.skipped_hashes.pop();
        break;
    case MerkleProofNodeType::verify:
    {
        auto &transaction = transactions[proof.txid_perm.front()];
        proof.txid_perm.pop();

        hash.Update(static_cast<const CryptoPP::byte *>(transaction.id.bytes), UINT160_BYTE_LENGTH);
        hash.Update(transaction.data, sizeof(transaction.data));
        hash.TruncatedFinal(right_hash.bytes, TRUNCATE_BYTE_LENGTH);

        break;
    }
    default:
        break;
    }

    hash.Update(left_hash.bytes, UINT160_BYTE_LENGTH);
    hash.Update(right_hash.bytes, UINT160_BYTE_LENGTH);
    hash.TruncatedFinal(merkle_root.bytes, TRUNCATE_BYTE_LENGTH);

    return merkle_root;
}

bool verify_transactions(std::vector<Transaction> &transactions, const Block &block, Prover &prover)
{
    std::vector<uint160_t> txids;
    for (auto &transaction : transactions)
    {
        txids.push_back(transaction.id);
    }

    MerkleProof proof = prover.get_proof(txids, *reinterpret_cast<const size_t *>(block.block_id.bytes));

    CryptoPP::SHA3_256 hash;

    uint160_t merkle_root = descend(transactions, proof, hash);

    return merkle_root == block.merkle_root;
}