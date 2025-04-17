#include <stdio.h>
#include <time.h>

#include "mta/agent.h"
#include "mta/cot_machine.h"

#include <secp256k1.h>

uint32_t random32(void) {return rand();}

int main(int argc, char** argv){

    if(argc != 3)
    {
        printf("Please give two share values. Number of values: %d\n", argc);
        return 1;
    }

    srand(time(NULL));

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);

    ecdsa_curve curve = secp256k1;

    init_global_ecc(curve);
    init_global_network(2);

    mta_agent_t* agent0 = create_agent(0, num1);
    mta_agent_t* agent1 = create_agent(1, num2);

    perform_2p_mta(agent0, agent1);

    int expected, actual;

    if(!verify_2p_mta(agent0, agent1, &expected, &actual))
    {
        printf("Verification failed\nExpected: %d, Actual: %d\n", expected, actual);
        return 1;
    }
    else {
        printf("Verification succeeded\n");
    }

    destroy_agent(agent0);
    destroy_agent(agent1);

    destroy_cot_network();

    return 0;
}
