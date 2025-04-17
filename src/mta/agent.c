#include "mta/agent.h"

mta_agent_t* create_agent(uint32_t id, uint32_t val)
{
    mta_agent_t* agent = malloc(sizeof(mta_agent_t));
    agent->id = id;
    agent->val = val;
    agent->output = 0;
    agent->val_p1 = 0;
    agent->val_p2 = 0;
    agent->machine = create_cot_machine();
    return agent;
}

void perform_2p_mta(mta_agent_t* agent1, mta_agent_t* agent2)
{
    mta_cot_machine_t* machine1 = agent1->machine;
    mta_cot_machine_t* machine2 = agent2->machine;

    int64_t val_11 = rand() % PRIME;
    int64_t val_12 = (agent1->val - val_11 + PRIME) % PRIME;

    int64_t val_21 = rand() % PRIME;
    int64_t val_22 = (agent2->val - val_21 + PRIME) % PRIME;


    int64_t accumulator1 = 0;
    int64_t rand1 = rand()%PRIME;

    for(int i=0;i<32;i++)
    {
        uint32_t selector = (val_11 >> i)%2;

        cot_machine_init_sender(machine2, rand1, val_22);
        cot_machine_init_reciever(machine1, selector);

        cot_machine_step(machine2, 1);
        cot_network_transmit(machine1, machine2); 
        cot_machine_step(machine1, 0);
        cot_network_transmit(machine1, machine2);

        cot_machine_step(machine2, 1);
        cot_network_transmit(machine1, machine2);
        cot_machine_step(machine1, 0);
        cot_network_transmit(machine1, machine2);

        int64_t value = (1 << i) * ((int64_t)machine1->selector);
        value %= PRIME;

        accumulator1 += value;
        accumulator1 %= PRIME;
    }

    accumulator1 -= ((((int64_t)1 << 33) - 1) * rand1) % PRIME;
    accumulator1 %= PRIME;

    agent1->output = (accumulator1 + val_11 * val_21)%PRIME;

    int64_t accumulator2 = 0;
    int64_t rand2 = rand()%PRIME;

    for(int i=0;i<32;i++)
    {
        uint32_t selector = (val_21 >> i)%2;

        cot_machine_init_sender(machine1, rand2, val_12);
        cot_machine_init_reciever(machine2, selector);

        cot_machine_step(machine1, 0);
        cot_network_transmit(machine1, machine2);
        cot_machine_step(machine2, 1);
        cot_network_transmit(machine1, machine2);

        cot_machine_step(machine1, 0);
        cot_network_transmit(machine1, machine2);
        cot_machine_step(machine2, 1);
        cot_network_transmit(machine1, machine2);
        
        int64_t value = (1 << i) * ((int64_t)machine2->selector);
        value %= PRIME;

        accumulator2 += value;
        accumulator2 %= PRIME;
    }

    accumulator2 -= ((((int64_t)1 << 33) - 1) * rand2) % PRIME;
    accumulator2 %= PRIME;

    agent2->output = (accumulator2 + val_21 * val_22)%PRIME;

}

bool verify_2p_mta(mta_agent_t* agent1, mta_agent_t* agent2, int* expected, int* actual)
{
    int val1 = agent1->output;
    int val2 = agent2->output;

    *expected = agent1->val + agent2->val;
    *actual = (val1 + val2) % PRIME;

    return (*expected == *actual);
}

void destroy_agent(mta_agent_t* agent)
{

    mta_cot_machine_t* machine = agent->machine;

    for(int i=0;i<machine->num_recieved;i++)
    {
        free(machine->recieved[i].data);
    }

    free(machine->recieved);

    for(int i=0;i<machine->num_to_send;i++)
    {
        free(machine->to_send[i].data);
    }

    free(machine->to_send);

    free(machine);

    free(agent);
}