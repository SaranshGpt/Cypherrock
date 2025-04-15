#pragma once

#include <stdint.h>

#include "numerics.h"

typedef struct 
{
    uint32_t id;
    uint32_t val;

    uint8_t* public_key;
    int public_key_size;

    uint8_t* private_key;
    int private_key_size;

    uint32_t alpha_share;

    mascot_triple_share_t* triplet_shares;
    size_t num_triplet_shares;

} mascot_agent_t;