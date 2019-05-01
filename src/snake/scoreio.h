#ifndef SCORE_IO_H_
#define SCORE_IO_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    char name[3];
    // Big endian; least significant digit last (read order)
    uint8_t score[4];
} score_t;

extern score_t hiscores[10];
extern score_t score;

/**
 * @return true if the scores could be loaded successfully.
 */
bool load_scores(void);
void update_scores(void);

static inline void clear_score(score_t * const score) {
    for(uint8_t i = 0; i < 3; i++) {
        score->name[i] = 'A';
    }
    for(uint8_t i = 0; i < 4; i++) {
        score->score[i] = 0;
    }
}

/**
 * @return > 0 if a > b
 * @return 0 if a == b
 * @return < 0 if a < b
 */
static inline int8_t compare_score(const score_t a, const score_t b) {
    int8_t cmp = 0;

    for(uint8_t i = 0; i < 4 && !cmp; i++) {
        cmp = a.score[i] - b.score[i];
    }
    return cmp;
}

/**
 * Increments the score.
 * @return The index of the most significant digit which changed.
 */
static inline int8_t increment_score(score_t * const score) {
    bool carry;
    int8_t i = 3;

    do {
        carry = ++score->score[i] > 9;

        if(carry) {
            score->score[i] = 0;
        }
    } while(carry && --i >= 0);

    return i;
}

/**
 * Fills str with the score. Requires at least a char[5].
 */
static inline void get_score_text(const score_t score, char * const str) {
    for(uint8_t i = 0; i < 4; i++) {
        str[i] = '0' + score.score[i];
    }
    str[4] = '\0';
}

#endif
