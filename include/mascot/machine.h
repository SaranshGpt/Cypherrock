#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "agent.h"

typedef enum {
    MASCOT_MACHINE_ERROR_NONE = 0,
    MASCOT_MACHINE_ERROR_INVALID_PRIME = 1,
    MASCOT_MACHINE_ERROR_INVALID_N_AGENTS = 2,
    MASCOT_MACHINE_ERROR_INVALID_AGENT = 3,
    MASCOT_MACHINE_ERROR_INVALID_TRIPLE = 4,
    MASCOT_MACHINE_ERROR_INVALID_TRIPLE_SHARE = 5,
    MASCOT_MACHINE_ERROR_INVALID_TRIPLE_SHARES = 6,
} mascot_machine_error_e;

typedef void (*mascot_crypto_encrypt_function_t)(uint8_t* buffer, int buffer_size, uint8_t* key, int key_size);
typedef void (*mascot_crypto_decrypt_function_t)(uint8_t* buffer, int buffer_size, uint8_t* key, int key_size);
typedef void (*mascot_crypto_hash_function_t)(uint8_t* buffer, int buffer_size, uint8_t* hash_buffer, int hash_size);
typedef uint8_t* (*mascot_crypto_keygen_function_t)(int* key_size);

typedef struct 
{
    mascot_crypto_encrypt_function_t encrypt;
    mascot_crypto_decrypt_function_t decrypt;
    mascot_crypto_hash_function_t hash;
    mascot_crypto_keygen_function_t keygen;
} mascot_crypto_params;

typedef struct 
{
    uint32_t prime;

    mascot_agent_t* agents;
    size_t num_agents;
    size_t max_agents;

    mascot_crypto_params crypto_params;

} mascot_machine_t;

mascot_machine_t create_mascot_machine(
    size_t max_agents, 
    uint32_t prime,
    mascot_crypto_params crypto_params
);

mascot_machine_error_e mascot_machine_init_agents(mascot_machine_t* machine, int num_triplets);
