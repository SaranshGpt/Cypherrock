#include "mascot/numerics.h"

#include <stdlib.h>
#include <time.h>

void mascot_triple_init()
{
    srand(time(NULL));
}

uint32_t *generate_additive_shares(size_t n_shares, uint32_t val, uint32_t prime)
{
    uint32_t* shares = malloc(sizeof(uint32_t) * n_shares);

    int64_t curr = val;

    for(int i=0; i<n_shares-1; i++)
    {
        shares[i] = rand()%prime;
        curr -= shares[i];
        curr = (curr % prime + prime) % prime;
    }

    shares[n_shares-1] = curr;

    return shares;
}

mascot_triple_t* generate_mascot_triples(size_t n_triplets, uint32_t prime)
{
    mascot_triple_t* triplets = malloc(sizeof(mascot_triple_t) * n_triplets);

    for(int i=0; i<n_triplets; i++)
    {
        triplets[i].a = rand()%prime;
        triplets[i].b = rand()%prime;

        uint64_t large_c = (uint64_t)triplets[i].a * (uint64_t)triplets[i].b;

        triplets[i].c = large_c % prime;
    }

    return triplets;
}

mascot_triple_share_t* generate_mascot_triple_shares(size_t n_shares, mascot_triple_t triple, uint32_t prime)
{
    mascot_triple_share_t* shares = malloc(sizeof(mascot_triple_share_t) * n_shares);

    uint32_t* a_shares = generate_additive_shares(n_shares, triple.a, prime);
    uint32_t* b_shares = generate_additive_shares(n_shares, triple.b, prime);
    uint32_t* c_shares = generate_additive_shares(n_shares, triple.c, prime);

    for(int i=0; i<n_shares; i++)
    {
        shares[i].a = a_shares[i];
        shares[i].b = b_shares[i];
        shares[i].c = c_shares[i];
    }

    free(a_shares);
    free(b_shares);
    free(c_shares);

    return shares;
}