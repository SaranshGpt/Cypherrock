#include "mta/cot_machine.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#include <ecdsa.h>
#include <bignum.h>
#include <sha3.h>


static ecdsa_curve global_curve;

static mta_cot_network global_network;

void init_global_ecc(ecdsa_curve curve)
{
    global_curve = curve;
}

void init_global_network(uint32_t max_machines)
{
    global_network.machines = malloc(sizeof(mta_cot_machine_t) * max_machines);
    global_network.num_machines = 0;
    global_network.max_machines = max_machines;
}

mta_cot_machine_t* create_cot_machine()
{
    mta_cot_machine_t* machine = malloc(sizeof(mta_cot_machine_t));
    machine->state = MTA_COT_MACHINE_STATE_NONE;
    machine->secret_scalar = 0;
    machine->values[0] = 0;
    machine->values[1] = 0;
    machine->selector = 0;
    machine->recieved = NULL;
    machine->num_recieved = 0;
    machine->to_send = NULL;
    machine->num_to_send = 0;

    global_network.machines[global_network.num_machines++] = *machine;

    return machine;
}

void cot_machine_init_sender(mta_cot_machine_t* machine, uint32_t value, uint32_t correlation)
{
    machine->state = MTA_COT_MACHINE_SENDER_STATE_INIT;
    machine->values[0] = value;
    machine->values[1] = (value + correlation)%PRIME;
}

void cot_machine_init_reciever(mta_cot_machine_t* machine, uint32_t selector)
{
    machine->state = MTA_COT_MACHINE_RECIEVER_STATE_INIT;
    machine->selector = selector;
}

static void add_send_data(mta_cot_machine_t* machine, uint8_t* data, uint32_t size, uint32_t source, uint32_t destination)
{
    machine->to_send = realloc(machine->to_send, sizeof(mta_communication_t) * (machine->num_to_send + 1));
    machine->to_send[machine->num_to_send].data = data;
    machine->to_send[machine->num_to_send].size = size;
    machine->to_send[machine->num_to_send].source = source;
    machine->to_send[machine->num_to_send].destination = destination;
    machine->num_to_send++;
}

mta_cot_machine_state_t cot_machine_step(mta_cot_machine_t* machine, int index)
{
    switch(machine->state)
    {
        case MTA_COT_MACHINE_STATE_NONE:
        {
            return MTA_COT_MACHINE_STATE_NONE;
        }
        case MTA_COT_MACHINE_SENDER_STATE_INIT:
        {

            machine->secret_scalar = rand() % PRIME;

            curve_point point = global_curve.G;

            bignum256 secret_scalar;
            bn_read_uint32(machine->secret_scalar, &secret_scalar);
            scalar_multiply(&global_curve, &secret_scalar, &point);

            uint8_t* send_data = malloc(sizeof(curve_point));
            memcpy(send_data, &point, sizeof(curve_point));
            add_send_data(machine, send_data, sizeof(curve_point), index, (index + 1)%2);

            machine->shared_secret_point = point;

            machine->state = MTA_COT_MACHINE_SENDER_STATE_SELECTOR;
            return MTA_COT_MACHINE_SENDER_STATE_SELECTOR;
        }
        case MTA_COT_MACHINE_SENDER_STATE_SELECTOR:
        {

            curve_point recv_point = *((curve_point*)machine->recieved[0].data);

            free(machine->recieved[0].data);
            free(machine->recieved);

            curve_point point0 = recv_point;
            bignum256 scalar;
            bn_read_uint32(machine->secret_scalar, &scalar);
            scalar_multiply(&global_curve, &scalar, &point0);

            curve_point point1 = recv_point;
            curve_point shared_pt = machine->shared_secret_point;

            bn_cnegate(1, &shared_pt.y, &global_curve.prime);

            point_add(&global_curve, &shared_pt, &point1);

            scalar_multiply(&global_curve, &scalar, &point1);

            uint8_t key0[257];
            uint8_t key1[257];

            compress_coords(&point0, key0);
            compress_coords(&point1, key1);

            uint8_t key0_hash[256];
            uint8_t key1_hash[256];

            sha3_256(key0, 257, key0_hash);
            sha3_256(key1, 257, key1_hash);

            uint32_t key0_val = *((uint32_t*)(key0_hash));
            uint32_t key1_val = *((uint32_t*)(key1_hash));
            
            uint32_t value0 = machine->values[0];
            value0 ^= key0_val;
            uint32_t value1 = machine->values[1];
            value1 ^= key1_val;

            machine->to_send = malloc(sizeof(mta_communication_t) * 2);
            machine->to_send[0].data = malloc(sizeof(uint32_t));
            machine->to_send[0].size = sizeof(uint32_t);
            machine->to_send[0].source = index;
            machine->to_send[0].destination = (index + 1)%2;
            *((uint32_t*)machine->to_send[0].data) = value0;
            machine->to_send[1].data = malloc(sizeof(uint32_t));
            machine->to_send[1].size = sizeof(uint32_t);
            machine->to_send[1].source = index;
            machine->to_send[1].destination = (index + 1)%2;
            *((uint32_t*)machine->to_send[1].data) = value1;

            machine->num_to_send = 2;

            uint8_t* send_data_1 = malloc(sizeof(uint32_t));
            memcpy(send_data_1, &machine->secret_scalar, sizeof(uint32_t));
            add_send_data(machine, send_data_1, sizeof(uint32_t), index, (index + 1)%2);
            
            uint8_t* send_data_2 = malloc(sizeof(uint32_t));
            memcpy(send_data_2, &machine->selector, sizeof(uint32_t));
            add_send_data(machine, send_data_2, sizeof(uint32_t), index, (index + 1)%2);

            machine->state = MTA_COT_MACHINE_SENDER_STATE_DONE;

            return MTA_COT_MACHINE_SENDER_STATE_DONE;
        }
        case MTA_COT_MACHINE_SENDER_STATE_DONE:
        {
            return MTA_COT_MACHINE_SENDER_STATE_DONE;
        }
        case MTA_COT_MACHINE_RECIEVER_STATE_INIT:
        {
            machine->state = MTA_COT_MACHINE_RECIEVER_STATE_SELECTOR;

            curve_point recv_point = *((curve_point*)machine->recieved[0].data);
            free(machine->recieved[0].data);
            free(machine->recieved);

            machine->secret_scalar = rand() % PRIME;

            curve_point point = global_curve.G;

            bignum256 scalar;
            bn_read_uint32(machine->secret_scalar, &scalar);

            scalar_multiply(&global_curve, &scalar, &point);

            if(machine->selector == 1)
            {
                point_add(&global_curve, &recv_point, &point);
            }
            

            uint8_t* send_data = malloc(sizeof(curve_point));
            memcpy(send_data, &machine->selector, sizeof(curve_point));
            add_send_data(machine, send_data, sizeof(curve_point), index, (index + 1)%2);

            scalar_multiply(&global_curve, &scalar, &recv_point);

            machine->shared_secret_point = recv_point;

            return MTA_COT_MACHINE_RECIEVER_STATE_SELECTOR;
        }
        case MTA_COT_MACHINE_RECIEVER_STATE_SELECTOR:
        {

            curve_point shared_pt = machine->shared_secret_point;

            bignum256 scalar;

            uint8_t key[257];
            compress_coords(&shared_pt, key);

            uint8_t key_hash[256];
            sha3_256(key, 257, key_hash);

            uint32_t key_val = *((uint32_t*)(key_hash));

            uint32_t value1 = *((uint32_t*)machine->recieved[machine->selector].data);
            value1 ^= key_val;

            uint32_t value2 = *((uint32_t*)machine->recieved[(machine->selector + 1)%2].data);
            value2 ^= key_val;

            machine->selector = value1%PRIME;

            free(machine->recieved[0].data);
            free(machine->recieved[1].data);
            free(machine->recieved);

            machine->state = MTA_COT_MACHINE_RECIEVER_STATE_DONE;
            return MTA_COT_MACHINE_RECIEVER_STATE_DONE;
        }
        case MTA_COT_MACHINE_RECIEVER_STATE_DONE:
        {
            return MTA_COT_MACHINE_RECIEVER_STATE_DONE;
        }
        default:
        {
            return MTA_COT_MACHINE_STATE_ERROR;
        }
    }
    return MTA_COT_MACHINE_STATE_ERROR;
}

void cot_network_transmit(mta_cot_machine_t* machine1, mta_cot_machine_t* machine2)
{

    machine1->recieved = malloc(sizeof(mta_communication_t) * machine2->num_to_send);
    machine1->num_recieved = machine2->num_to_send;
    memcpy(machine1->recieved, machine2->to_send, sizeof(mta_communication_t) * machine2->num_to_send);
    free(machine2->to_send);
    machine2->to_send = NULL;
    machine2->num_to_send = 0;

    machine2->recieved = malloc(sizeof(mta_communication_t) * machine1->num_to_send);
    machine2->num_recieved = machine1->num_to_send;
    memcpy(machine2->recieved, machine1->to_send, sizeof(mta_communication_t) * machine1->num_to_send);
    free(machine1->to_send);
    machine1->to_send = NULL;
    machine1->num_to_send = 0;
}

void cot_machine_reset(mta_cot_machine_t* machine)
{
    machine->state = MTA_COT_MACHINE_STATE_NONE;
    machine->secret_scalar = 0;
    machine->values[0] = 0;
    machine->values[1] = 0;
    machine->selector = 0;
    machine->recieved = NULL;
    machine->num_recieved = 0;
    machine->to_send = NULL;
    machine->num_to_send = 0;
}

void destroy_cot_network()
{
    free(global_network.machines);
    global_network.machines = NULL;
    global_network.num_machines = 0;
    global_network.max_machines = 0;
}