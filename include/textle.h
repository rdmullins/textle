#ifndef TEXTLE_H
#define TEXTLE_H

#include <stdbool.h>

#define TEXTLE_WORD_LENGTH 5
#define TEXTLE_MAX_GUESSES 6
#define TEXTLE_FEEDBACK_SIZE (TEXTLE_WORD_LENGTH * 3)
#define TEXTLE_ALPHABET_SIZE 26

#define TEXTLE_TRACKER_UNKNOWN ' '
#define TEXTLE_TRACKER_PRESENT '*'
#define TEXTLE_TRACKER_ABSENT 'X'

bool textle_is_five_letters(const char *word);

void textle_score_guess(const char answer[TEXTLE_WORD_LENGTH + 1],
                        const char guess[TEXTLE_WORD_LENGTH + 1],
                        char feedback[TEXTLE_FEEDBACK_SIZE]);

void textle_update_alphabet(
    char status[TEXTLE_ALPHABET_SIZE],
    const char guess[TEXTLE_WORD_LENGTH + 1],
    const char feedback[TEXTLE_FEEDBACK_SIZE]);

#endif
