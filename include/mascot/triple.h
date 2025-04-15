#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct 
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
} mascot_triple_t;

typedef mascot_triple_t mascot_triple_share_t;

void mascot_triple_init();

mascot_triple_t* generate_mascot_triples(size_t n_triples, uint32_t prime);
mascot_triple_share_t* generate_mascot_triple_shares(size_t n_shares, mascot_triple_t triple, uint32_t prime);