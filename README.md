# Blockchain implementation
 A simplified blockchain implementation using C++ that simulates Proof-of-Work mining, Merkle tree construction and transaction validation.

## Overview
This program depicts a functional blockchain implementation:
-	Generates users  
-	Creates transactions between randomly picked users
-	Mines blocks using PoW consensus 
-	Builds Merkle trees for transaction verification
This implementation uses our previously made LIDL hash, which was enhanced with AI (_Sonnet 4.5_ model) to increase the resistance of collisions, for all cryptographic operations such as users hash, transactions IDs, Merke roots and block hashes, which start with zeros (the number of ‘0’ is based on the user chosen difficulty).

## Features
-	Parallel Proof-of-Work Mining: Multi-threaded mining using OpenMP 
-	Merkle tree implementation _([implemented using this](https://codetrips.com/2020/08/02/modern-c-implementing-a-merkle-tree/))_
-	Transaction Validation: Balance checking and transaction ID verification with UTXO model
-	Custom Hash Function: LIDL hash enhanced for collision resistance
-   Multiple threads compete to mine each block

## Running
### 1.	Clone repository 
`git clone https://github.com/augustetum/blockchain-2.git`
### 2.	Open cloned repository 
`cd blockchain-2`
### 3.	Build everything
`make run`
### 4. Build only main program
`make blockchain`
### 5. Build user generator
`make usergen`
### 6. Build transaction generator
`make txgen`
### 7. To clean *.o *.exe files run
`make clean`
### 8. Build and run everything (user generation, transaction generation AND blockchain mining)
`make all`
> These Makefile commands work on Windows, MacOS, Linux

## Principle of working 
-	Generating users _(username, public key, balance)_
-	Generating transactions _(sender key, receiver key, amount sent)_
-	Creating blocks with transaction
-	Creation of Merkle root
-	Mining
-	Output to the terminal, transactions.txt and users_updated.txt

## Example of the program
### First block
````
- Block Hash: 00037ce0935d0260fbba6aa7c04fac494faca9b8a0fdd846191264c78dbc13160
- Previous Hash: 0000000000000000000000000000000000000000000000000000000000000000
- Merkle Root: 6204af027912d2bf984be9b04add70d0a22b98f2645411f1797c40df06664765
- Timestamp: 1762367523
- Difficulty: 3
- Nonce: 4946
- Transactions: 100
````

### Last block
````
- Block Hash: 0004ac5fbdb0ea38f8b373a682dd25965ef858b4b89d8a62cf9a3239450f8b5c
- Previous Hash: 000ebe6af32ae36df2b1640defe70f5e647927d1951196d3b739f3dcb5994c37
- Merkle Root: 7684e6bdb2daaa11db603c4814f58df6518d45a5e4ec1f64724c75ac02b83c8f
- Timestamp: 1762367524
- Difficulty: 3
- Nonce: 1208
- Transactions: 100
````

### Randomly chosen block 98
````
- Selected 100 transactions
- Remaining in pool: 200
- Mining block...
- Block mined! Nonce: 9523 (after 9524 attempts)
- Block hash: 0009b5b9bfac8baeed8f57ecd889fb4a5f53d091bd50d019ea9f103eb9a58475
- Merkle root: b8c68241d998506cdf4865d0a9d54ad2f7b1fa81517d9ff91a7ae7261fb37c27
- Nonce found: 9523
- Mining time: 35ms
````

### After the completion
````
- Total blocks mined: 100
- Total time: 1 seconds
- Transactions remaining in pool: 0
< blockchain saved to blockchain.txt, updated user balances saved to users_updated.txt
````

## Parallel mining implementation with OpenMP

Multiple threads simultaneously attempt to mine the same block with different transaction sets, and the first thread to find a valid nonce wins. The implementation of OpenMP was written with the help of AI _(Sonnet 4.5 model)_

Each thread gets its own candidate block with:
- Same previous hash (maintaining chain)
- Same difficulty target

### Mining Process:
- Five different blocks (for five threads) are generated
- Threads attempt to mine these blocks 
- Each thread checks if another thread won _(stopFlag)_ while mining
- Each thread calculates hash with current nonce
- Validation happens by checking leading zeros 
- If valid → return true
- Block (which was mined first) is added to a blockchain
- Other blocks are terminated


## Difficulty impact analysis
| Difficulty | Avg Attempts | Avg Time | Probability |
|-----------|--------------|----------|-------------|
| 1         | ~16          | ~2ms     | 1/16        |
| 2         | ~256         | ~8ms     | 1/256       |
| 3         | ~4096        | ~35ms    | 1/4096      |
| 4         | ~65536       | ~500ms   | 1/65536     |
| 5         | ~1048576     | ~8s      | 1/1048576   |

## UTXO model
A UTXO model was also constructed for this program. It was done while (almost) entirely relying on in-terminal Claude AI and Sonnet 4.5. User generation, transaction generation and mining validation were all adapted to work with the UTXO model. Because of the messy code structure AI provides, the code is stored in a separate branch called `utxo-implementation`. The make commands all run the same way.

### Workflow
1. Load genesis UTXOs (1000 users, each with 1 UTXO)
2. Load transactions from file (reconstruct with UTXO inputs/outputs)
3. Mining loop:
- Validate transactions against current UTXO set
- Select up to 100 valid transactions
- All 5 threads mine the same block with different nonces
- First thread to find valid hash wins
- Apply winning block: remove input UTXOs, add output UTXOs
- Repeat until no valid transactions remain
4. Save final UTXO state to file

### Examples
1. UTXO list after user generation
````
TransactionID                                          OutputIndex Amount OwnerPublicKey                                                   Spent
0000000000000000000000000000000000000000000000000000000000000000 0 823155 9a213280fd0a88c83e9e144a74907eabcc14f1d41edf5422ebf7286ad944d41c false
0000000000000000000000000000000000000000000000000000000000000000 1 534236 0945ea17e24b9f068c5f60989d8c1aaeca457e53e8167851f140b88eefd88282 false
0000000000000000000000000000000000000000000000000000000000000000 2 218335 aececb8c1c211454a587c0102148f52da999a611dcddf7217a8d5b336d978e3f false
0000000000000000000000000000000000000000000000000000000000000000 3 861307 0d00da43ff4ff51be513055143949bf402cef79fd9eaf0c8eb0f8540589309f3 false
0000000000000000000000000000000000000000000000000000000000000000 4 533906 47a9fd49dfae0d0d64db2748b6f11871e31337a6d19de2435c395957279f690e false
0000000000000000000000000000000000000000000000000000000000000000 5 212077 d03d3b06c02ac393636167dfd4588dc1c8e139551df543c6635ceff7899b515e false
0000000000000000000000000000000000000000000000000000000000000000 6 696015 442b4c705f210ce5a67f9a1fac853ab2b9bafec1a41397112e5d12a200173545 false
0000000000000000000000000000000000000000000000000000000000000000 7 495084 9f1d0aaccba0f4f2c3d5841e0b0682c7ccb94829a8cbf9c2187e6f6d100b2822 false
0000000000000000000000000000000000000000000000000000000000000000 8 140619 357b09bc89ace290c253b67de7d15de98e8515f3702a2271f3773e0b73995578 false
0000000000000000000000000000000000000000000000000000000000000000 9 806635 a7a0b0026573a489a1d97dc183d798aeb2208c426f58629c262fdab2c4139b6e false
````
2. Transaction generation
````
TX: ee6316dfd9855cc680abd3a728dab6fd372e63a68ef25069d302e6692113fa02
  From: 80996aa75362a4bd236b96060d85df9629aab2aa14fb43e9ee730fdd6c91cc7d
  To: 932e418d6611065bc7778d5ea55d09365afe3b897ffac8903db3503f2fd0d4f5
  Inputs (1):
    - UTXO 56fa0586b3cc5d56e3f11032aedec5c1dfbe3f9ff9ceff070d040a7740184fe2:0 : 6576
  Total input: 6576
  Outputs (2):
    - To 932e418d6611065b... : 986
    - To 80996aa75362a4bd... : 5590 (change)
  Total output: 6576
  Transaction amount: 986

TX: 67b8048537f0533888bc689454f5a9c734bcd0fb362e3ff8d8e5e1c917df3016
  From: 6d110f328be9bc2a6f390d1ca97cffbd1241d32e555524d77d1e138e8fa63ff3
  To: d8d7e37a14f91c6f9b3fd56e6d40754af5b9bdb7ec1e451e78b4885f2ec326e7
  Inputs (1):
    - UTXO 15e8d581e4f74ff41b094344a4349f9227469bb9831988cd01fe76af2b9e36b1:1 : 18639
  Total input: 18639
  Outputs (2):
    - To d8d7e37a14f91c6f... : 6150
    - To 6d110f328be9bc2a... : 12489 (change)
  Total output: 18639
  Transaction amount: 6150

TX: 2041a0f54109811042980ff845451774e43544b8996ca3d2c2040c736508cf19
  From: 83c5d941799657f993c34cd7a94fcc7ffd5555adf7129c9f7ce4c95ba68e75a7
  To: e496eadca6746d817e3c7864a5313aa0ba29228f32bbe3422d76ad546180f0b4
  Inputs (1):
    - UTXO 0cf0ac61fca1f761c86e3bf2fdf0adfaeba606bb2180aa6adcda86a7ae29adf0:0 : 28329
  Total input: 28329
  Outputs (2):
    - To e496eadca6746d81... : 14164
    - To 83c5d941799657f9... : 14165 (change)
  Total output: 28329
  Transaction amount: 14164
````
3. Block mining
````
--- Block 54 ---
Transaction pool size: 3174
Selected 100 valid transactions
Remaining in pool: 3074
Failed to mine block with 1000 attempts per thread. Retrying with 2000 attempts per thread...
Failed to mine block with 2000 attempts per thread. Retrying with 3000 attempts per thread...
Failed to mine block with 3000 attempts per thread. Retrying with 4000 attempts per thread...
Failed to mine block with 4000 attempts per thread. Retrying with 5000 attempts per thread...
Block mined by Thread 4
Block hash: 000689186a7f8331bcd31b123bc10ab6f86ca4e394d84cd6214049d27ba357f8
Merkle root: a4ec4da3f2289760c407c5ac3d310c2e43fcb853c925f992f2754dc1d6d08e81
Nonce found: 4295
Winner's attempts: 4296
Total attempts (all 5 threads): 21444
Mining time: 7ms

--- Block 55 ---
Transaction pool size: 3074
Selected 100 valid transactions
Remaining in pool: 2974
Failed to mine block with 1000 attempts per thread. Retrying with 2000 attempts per thread...
Failed to mine block with 2000 attempts per thread. Retrying with 3000 attempts per thread...
Block mined by Thread 1
Block hash: 000bb16aa4cdae36a6de46ac2d6b9e40b1583bbc3644700f9c66110ab870e306
Merkle root: 59b186bde544b57a076062ac20372545724d5ae7f911d0e3f71fcb05a2402263
Nonce found: 2436
Winner's attempts: 2437
Total attempts (all 5 threads): 12174
Mining time: 4ms

--- Block 56 ---
Transaction pool size: 2974
Selected 100 valid transactions
Remaining in pool: 2874
Failed to mine block with 1000 attempts per thread. Retrying with 2000 attempts per thread...
Failed to mine block with 2000 attempts per thread. Retrying with 3000 attempts per thread...
Failed to mine block with 3000 attempts per thread. Retrying with 4000 attempts per thread...
Failed to mine block with 4000 attempts per thread. Retrying with 5000 attempts per thread...
Failed to mine block with 5000 attempts per thread. Retrying with 6000 attempts per thread...
Block mined by Thread 4
Block hash: 00018d2c81f18a5d888768196d951c7e5c893687fea2ee796aa36244e1661fc8
Merkle root: 74479bc29b9f80504760e0f2ca2986973a367166b2315e2d63f96ceb5d9cdb6c
Nonce found: 5944
Winner's attempts: 5945
Total attempts (all 5 threads): 29695
Mining time: 11ms

--- Block 57 ---
Transaction pool size: 2874
Selected 100 valid transactions
Remaining in pool: 2774
Failed to mine block with 1000 attempts per thread. Retrying with 2000 attempts per thread...
Failed to mine block with 2000 attempts per thread. Retrying with 3000 attempts per thread...
Failed to mine block with 3000 attempts per thread. Retrying with 4000 attempts per thread...
Failed to mine block with 4000 attempts per thread. Retrying with 5000 attempts per thread...
Failed to mine block with 5000 attempts per thread. Retrying with 6000 attempts per thread...
Failed to mine block with 6000 attempts per thread. Retrying with 7000 attempts per thread...
Failed to mine block with 7000 attempts per thread. Retrying with 8000 attempts per thread...
Failed to mine block with 8000 attempts per thread. Retrying with 9000 attempts per thread...
Failed to mine block with 9000 attempts per thread. Retrying with 10000 attempts per thread...
Block mined by Thread 5
Block hash: 00085242d01fcdb7ce348e22fa8807f6466034c1a15261c05618a4430d4425e7
Merkle root: 025d609e0ee2b9c8b6fb66a4ab71ef1ec8b958022e1fef64ec932c331521fc9d
Nonce found: 9215
Winner's attempts: 9216
Total attempts (all 5 threads): 46052
Mining time: 16ms
````
