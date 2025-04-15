#include "mascot/triple.h"

#include <stdlib.h>
#include <time.h>

void mascot_triple_init()
{
    srand(time(NULL));
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

    int64_t curr_a = triple.a;
    int64_t curr_b = triple.b;
    int64_t curr_c = triple.c;

    for(int i=0; i<n_shares-1; i++)
    {
        shares[i].a = rand()%prime;
        shares[i].b = rand()%prime;
        shares[i].c = rand()%prime;

        curr_a -= shares[i].a;
        curr_b -= shares[i].b;
        curr_c -= shares[i].c;

        curr_a = (curr_a % prime + prime) % prime;
        curr_b = (curr_b % prime + prime) % prime;
        curr_c = (curr_c % prime + prime) % prime;
    }

    shares[n_shares-1].a = curr_a;
    shares[n_shares-1].b = curr_b;
    shares[n_shares-1].c = curr_c;

    return shares;
}