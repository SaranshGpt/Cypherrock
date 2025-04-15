#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "agent.h"

typedef enum {
    MASCOT_SETUP_ERROR_NONE = 0,
    MASCOT_SETUP_ERROR_INVALID_PRIME = 1,
    MASCOT_SETUP_ERROR_INVALID_N_AGENTS = 2,
    MASCOT_SETUP_ERROR_INVALID_AGENT = 3,
    MASCOT_SETUP_ERROR_INVALID_TRIPLE = 4,
    MASCOT_SETUP_ERROR_INVALID_TRIPLE_SHARE = 5,
    MASCOT_SETUP_ERROR_INVALID_TRIPLE_SHARES = 6,
} mascot_setup_error_e;

typedef void (*mascot_crypto_encrypt_function_t)(uint8_t* buffer, int buffer_size, uint8_t* key, int key_size);
typedef void (*mascot_crypto_decrypt_function_t)(uint8_t* buffer, int buffer_size, uint8_t* key, int key_size);
typedef void (*mascot_crypto_hash_function_t)(uint8_t* buffer, int buffer_size, uint8_t* hash_buffer, int hash_size);
typedef void (*mascot_crypto_keygen_function_t)(uint8_t **public_key, int* public_key_size, uint8_t **private_key, int* private_key_size);

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

    uint32_t alpha;

    mascot_agent_t* agents;
    size_t num_agents;
    size_t max_agents;

    mascot_crypto_params crypto_params;

} mascot_setup_t;

mascot_setup_t create_mascot_setup(size_t max_agents, uint32_t prime, mascot_crypto_params crypto_params);

mascot_agent_t* add_mascot_agent(mascot_setup_t* setup, uint32_t val);

mascot_setup_error_e init_mascot_protocol(mascot_setup_t* setup, int num_triplets);

