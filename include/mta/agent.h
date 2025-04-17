#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "cot_machine.h"

typedef struct
{

    uint32_t id;

    uint32_t val;

    uint32_t output;

    uint32_t val_p1;
    uint32_t val_p2;

    mta_cot_machine_t* machine;

} mta_agent_t;

mta_agent_t* create_agent(uint32_t id, uint32_t val);

void perform_2p_mta(mta_agent_t* agent1, mta_agent_t* agent2);

bool verify_2p_mta(mta_agent_t* agent1, mta_agent_t* agent2, int* expected, int* actual);

void destroy_agent(mta_agent_t* agent);