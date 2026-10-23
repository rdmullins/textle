#include "textle.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void expect_score(const char *answer, const char *guess,
                         const char *expected)
{
    char feedback[TEXTLE_WORD_LENGTH * 2 + 1];

    textle_score_guess(answer, guess, feedback);
    if (strcmp(feedback, expected) != 0) {
        fprintf(stderr, "%s vs %s: expected [%s], got [%s]\n",
                answer, guess, expected, feedback);
        assert(strcmp(feedback, expected) == 0);
    }
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
    expect_score("APPLE", "ALLEY", "A L* _ _ E");
    expect_score("LEVEL", "LEEEE", "L E E* E* _");

    puts("All feedback tests passed.");
    return 0;
}
