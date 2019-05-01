#ifndef BLOB_H_
#define BLOB_H_

#include <avr/pgmspace.h>
#include <stdint.h>

// Object file symbol names
#define BLOB_SIZE_ADDR(x) x ## _size_addr
#define BLOB(x) x
#define BLOB_END(x) x ## _end

#define BLOB_SIZE(x) (*BLOB_SIZE_ADDR(x))

#define USEBLOB(x) extern const uint8_t BLOB(x)[] PROGMEM; \
    extern const uint8_t BLOB_END(x)[] PROGMEM; \
    extern const uint16_t BLOB_SIZE_ADDR(x)[]

#endif
