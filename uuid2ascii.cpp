//
// Created by rj on 2/17/20.
//


#include <cstdint>
#include <cstdio>

struct UUID {
    uint32_t
            timeLow;
    uint16_t
            timeMid,
            timeHigh,
            clock;
    uint8_t
            node[6];
};

//-----------------------------------------------------------------------------
// static char *uuid2ascii(uint8_t *uuid)
//  convert UUID to ascii hex form
//
// Parm
//  uuid - 16-byte uuid
//
// Returns
//  pointer to statically allocated character string with hex form of uuid
//
// Notes
// - Is not reentrant, uses a single static buffer
//

/*
static char *uuid2ascii(struct UUID *uuid) {

    sprintf(uuidStr,"%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x",
            uuid->timeLow,uuid->timeMid,uuid->timeHigh,uuid->clock,
            uuid->node[0],uuid->node[1],uuid->node[2],uuid->node[3],
            uuid->node[4],uuid->node[5]);
    return uuidStr;
}
 */
