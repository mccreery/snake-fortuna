#include <avr/eeprom.h>
#include <string.h>

#include "scoreio.h"
#include "parity.h"

__attribute__((section (".noinit")))
score_t hiscores[10];
__attribute__((section (".noinit")))
score_t score;

// Reserve EEPROM memory for scores
static score_t eep_hiscores[10] EEMEM;
static uint8_t eep_parity EEMEM;

static bool are_scores_valid(void) {
    for(uint8_t i = 0; i < 10; i++) {
        for(uint8_t j = 0; j < 3; j++) {
            char c = hiscores[i].name[j];
            if(c && (c < 'A' || c > 'Z')) return false;
        }
        for(uint8_t j = 0; j < 4; j++) {
            if(hiscores[i].score[j] > 9) return false;
        }
    }
    return true;
}

bool load_scores(void) {
    eeprom_read_block(hiscores, eep_hiscores, sizeof(hiscores));
    bool loaded = eeprom_read_byte(&eep_parity) ==
            parity(hiscores, sizeof(hiscores)) &&
            are_scores_valid();

    if(!loaded) {
        memset(hiscores, 0, sizeof(hiscores));
        update_scores();
    }
    return loaded;
}

void update_scores(void) {
    eeprom_update_block(hiscores, eep_hiscores, sizeof(hiscores));
    eeprom_update_byte(&eep_parity, parity(hiscores, sizeof(hiscores)));
}
