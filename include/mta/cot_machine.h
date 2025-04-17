#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <ecdsa.h>

#define PRIME 179

typedef enum
{

    MTA_COT_MACHINE_STATE_NONE = 0,

    MTA_COT_MACHINE_SENDER_STATE_INIT = 1,
    MTA_COT_MACHINE_SENDER_STATE_SELECTOR = 2,
    MTA_COT_MACHINE_SENDER_STATE_DONE = 3,

    MTA_COT_MACHINE_RECIEVER_STATE_INIT = 4,
    MTA_COT_MACHINE_RECIEVER_STATE_SELECTOR = 5,
    MTA_COT_MACHINE_RECIEVER_STATE_DONE = 6,

    MTA_COT_MACHINE_STATE_ERROR = 31,

} mta_cot_machine_state_t;

typedef struct 
{
    uint8_t* data;
    uint32_t size;
    uint32_t source;
    uint32_t destination;
} mta_communication_t;

typedef struct 
{
    mta_cot_machine_state_t state;

    uint32_t secret_scalar;

    uint32_t values[2];

    uint32_t selector;

    curve_point shared_secret_point;

    mta_communication_t* recieved;
    size_t num_recieved;

    mta_communication_t* to_send;
    size_t num_to_send;

} mta_cot_machine_t;

typedef struct
{
    mta_cot_machine_t* machines;
    size_t num_machines;
    size_t max_machines;
} mta_cot_network;

void init_global_ecc(ecdsa_curve curve);

void init_global_network(uint32_t max_machines);

mta_cot_machine_t* create_cot_machine();

void cot_machine_init_sender(mta_cot_machine_t* machine, uint32_t value, uint32_t correlation);
void cot_machine_init_reciever(mta_cot_machine_t* machine, uint32_t selector);

mta_cot_machine_state_t cot_machine_step(mta_cot_machine_t* machine, int index);
void cot_network_transmit();

void cot_machine_reset(mta_cot_machine_t* machine);

void destroy_cot_network();