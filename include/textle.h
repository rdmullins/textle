#ifndef TEXTLE_H
#define TEXTLE_H

#include <stdbool.h>

#define TEXTLE_WORD_LENGTH 5
#define TEXTLE_MAX_GUESSES 6

/*
 * Score a guess against an answer.
 *
 * The result contains one character per position:
 *   matching letter: exact-position match
 *   letter followed by '*': present elsewhere in the answer
 *   '_': no unmatched copy of that letter remains
 *
 * The output buffer must hold at least TEXTLE_WORD_LENGTH * 2 + 1 bytes.
 * Both inputs must be five uppercase ASCII letters.
 */
void textle_score_guess(const char answer[TEXTLE_WORD_LENGTH + 1],
                        const char guess[TEXTLE_WORD_LENGTH + 1],
                        char feedback[TEXTLE_WORD_LENGTH * 2 + 1]);

bool textle_is_five_letters(const char *word);

#endif
