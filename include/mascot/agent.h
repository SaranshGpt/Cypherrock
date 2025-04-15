#pragma once

#include <stdint.h>

#include "triple.h"

typedef struct 
{
    uint32_t val;

    uint8_t* key;
    int key_size;

    mascot_triple_share_t* triplet_shares;
    size_t num_triplet_shares;

    

} mascot_agent_t;