#include "mascot/setup.h"

#include <stdlib.h>
#include <time.h>

mascot_setup_t create_mascot_setup(size_t max_agents, uint32_t prime, mascot_crypto_params crypto_params)
{
    srand(time(NULL));

    mascot_setup_t setup = {
        .prime = prime,
        .alpha = rand()%prime,
        .agents = malloc(sizeof(mascot_agent_t) * max_agents),
        .num_agents = 0,
        .max_agents = max_agents,
        .crypto_params = crypto_params,
    };

    return setup;
}

mascot_agent_t* add_mascot_agent(mascot_setup_t* setup, uint32_t val)
{

    if(setup->num_agents >= setup->max_agents)
    {
        return NULL;
    }

    int agent_id = setup->num_agents;

    mascot_agent_t* agent = &setup->agents[agent_id];

    agent->id = agent_id;
    agent->val = val;
    agent->triplet_shares = NULL;
    agent->num_triplet_shares = 0;
    agent->alpha_share = 0;

    setup->crypto_params.keygen(&agent->public_key, &agent->public_key_size, &agent->private_key, &agent->private_key_size);
    
    setup->num_agents++;

    return agent;
}

mascot_setup_error_e init_mascot_protocol(mascot_setup_t* setup, int num_triplets)
{

    setup->alpha = rand() % setup->prime;
    uint32_t* alpha_shares = generate_additive_shares(setup->num_agents, setup->alpha, setup->prime);

    for(int i=0; i<setup->num_agents; i++)
    {
        setup->agents[i].alpha_share = alpha_shares[i];
    }

    mascot_triple_init();

    mascot_triple_t* triples = generate_mascot_triples(num_triplets, setup->prime);

    for(int i=0; i<setup->num_agents; i++)
    {
        setup->agents[i].triplet_shares = malloc(sizeof(mascot_triple_share_t) * num_triplets);
        setup->agents[i].num_triplet_shares = num_triplets;
    }

    for(int i=0; i<num_triplets; i++)
    {
        mascot_triple_share_t* shares = generate_mascot_triple_shares(setup->num_agents, triples[i], setup->prime);

        for(int j=0; j<setup->num_agents; j++)
        {
            setup->agents[j].triplet_shares[i] = shares[j];
        }

        free(shares);
    }

    free(triples);
    free(alpha_shares);

    return MASCOT_SETUP_ERROR_NONE;
}