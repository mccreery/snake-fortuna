#ifndef GEOM_H_
#define GEOM_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
} point_t;

typedef struct {
    point_t pos;
    point_t size;
} rectangle_t;

static inline bool point_eq(const point_t a, const point_t b) {
    return a.x == b.x && a.y == b.y;
}

#define POINT_ZERO (point_t){0, 0}
#define RECT_ZERO (rectangle_t){{0, 0}, {0, 0}}

#define NEGATE_POINT(p) (point_t){-p.x, -p.y}

#endif
