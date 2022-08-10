# prob2

1. 블록체인 모델 구현
```c
typedef struct {
  uint8_t internal[20];
} uint160_t;

struct transaction {
  uint160_t transaction_id;
  uint8_t data[108];
};

struct block {
  uint160_t block_id;
  uint160_t hash_value;
  struct transaction transactions[8192];
};
```
- `struct block generate_genesis_block(struct transaction transactions[]);`
- `struct block generate_next_block(struct block prev_block, struct transaction transactions[]);;`
- `bool verify_blocks(struct block[]);`

## Test code
- `test_generate_genesis_block()`
- `test_generate_next_block()`
- `test_verify_blocks()`

## SHA3-256에 관한 Inspection
- Fast exit (in verification)
