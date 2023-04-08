/**
 * @file GC9A01.h
 *
 **/

#ifndef GC9A01_H
#define GC9A01_H

#include <cstdint>

/**********************
 * GLOBAL PROTOTYPES
 **********************/


void GC9A01_init();

void GC9A01_fillWithColor(uint8_t r, uint8_t g, uint8_t b);

void GC9A01_write(uint16_t *buffer, uint32_t length);

#endif /* GC9A01_H */

