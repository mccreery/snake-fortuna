#include "blob.h"
#include "grid.h"
#include "levels.h"

#define LEVEL_BYTES (32*32 / 8)
#define OBSTACLE_COLOR (0x7475)

USEBLOB(levels_1);

void load_level(uint8_t level) {
    clear_grid();
    if(level == 0) return;

    const uint8_t * level_data = BLOB(levels_1) + LEVEL_BYTES * (level - 1);

    for(uint8_t y = 0; y < 32; y++) {
        for(uint8_t x = 0; x < 32; x += 8) {
            uint8_t b = pgm_read_byte(level_data++);

            for(uint8_t i = 0; i < 8; i++) {
                write_cell((point_t){x + i, y}, b >> (7 - i) & 1 ? OBSTACLE_COLOR : 0);
            }
        }
    }
}
