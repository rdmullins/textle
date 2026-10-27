#include "textle.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void expect_score(const char *answer, const char *guess,
                         const char *expected)
{
    char feedback[TEXTLE_FEEDBACK_SIZE];
    textle_score_guess(answer, guess, feedback);
    if (strcmp(feedback, expected) != 0) {
        fprintf(stderr, "%s vs %s: expected [%s], got [%s]\n",
                answer, guess, expected, feedback);
        assert(strcmp(feedback, expected) == 0);
    }
}

static void test_alphabet_tracker(void)
{
    char status[TEXTLE_ALPHABET_SIZE];
    char feedback[TEXTLE_FEEDBACK_SIZE];
    char all_absent[TEXTLE_FEEDBACK_SIZE] = "_ _ _ _ _";

    memset(status, TEXTLE_TRACKER_UNKNOWN, sizeof(status));
    textle_score_guess("GLITZ", "GRIFT", feedback);
    textle_update_alphabet(status, "GRIFT", feedback);

    assert(status['G' - 'A'] == TEXTLE_TRACKER_PRESENT);
    assert(status['I' - 'A'] == TEXTLE_TRACKER_PRESENT);
    assert(status['T' - 'A'] == TEXTLE_TRACKER_PRESENT);
    assert(status['R' - 'A'] == TEXTLE_TRACKER_ABSENT);
    assert(status['F' - 'A'] == TEXTLE_TRACKER_ABSENT);
    assert(status['A' - 'A'] == TEXTLE_TRACKER_UNKNOWN);

    /*
     * A later all-absent result for a repeated letter must not erase
     * an earlier confirmed-present status.
     */
    textle_update_alphabet(status, "TTTTT", all_absent);
    assert(status['T' - 'A'] == TEXTLE_TRACKER_PRESENT);
}

int main(void)
{
    assert(textle_is_five_letters("GRIFT"));
    assert(textle_is_five_letters("grift"));
    assert(!textle_is_five_letters("four"));
    assert(!textle_is_five_letters("sixxxx"));
    assert(!textle_is_five_letters("ab d!"));

    expect_score("GLITZ", "GLITZ", "G L I T Z");
    expect_score("GLITZ", "GRIFT", "G _ I _ T*");
    expect_score("SHEEP", "EERIE", "E* E* _ _ _");
    expect_score("APPLE", "ALLEY", "A L* _ E* _");
    expect_score("LEVEL", "LEEEE", "L E _ E _");

    test_alphabet_tracker();

    puts("All feedback tests passed.");
    return 0;
}
