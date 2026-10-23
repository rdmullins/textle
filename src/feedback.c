#include "textle.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

bool textle_is_five_letters(const char *word)
{
    size_t i;
    if (word == NULL || strlen(word) != TEXTLE_WORD_LENGTH) return false;
    for (i = 0; i < TEXTLE_WORD_LENGTH; ++i) {
        if (!isalpha((unsigned char)word[i])) return false;
    }
    return true;
}

void textle_score_guess(const char answer[TEXTLE_WORD_LENGTH + 1],
                        const char guess[TEXTLE_WORD_LENGTH + 1],
                        char feedback[TEXTLE_WORD_LENGTH * 2 + 1])
{
    bool used[TEXTLE_WORD_LENGTH] = { false };
    bool exact[TEXTLE_WORD_LENGTH] = { false };
    size_t i, out = 0;

    for (i = 0; i < TEXTLE_WORD_LENGTH; ++i) {
        if (answer[i] == guess[i]) {
            used[i] = true;
            exact[i] = true;
        }
    }

    for (i = 0; i < TEXTLE_WORD_LENGTH; ++i) {
        size_t j;
        bool misplaced = false;
        if (i != 0) feedback[out++] = ' ';
        if (exact[i]) {
            feedback[out++] = guess[i];
            continue;
        }
        for (j = 0; j < TEXTLE_WORD_LENGTH; ++j) {
            if (!used[j] && answer[j] == guess[i]) {
                used[j] = true;
                misplaced = true;
                break;
            }
        }
        if (misplaced) {
            feedback[out++] = guess[i];
            feedback[out++] = '*';
        } else {
            feedback[out++] = '_';
        }
    }
    feedback[out] = '\0';
}
