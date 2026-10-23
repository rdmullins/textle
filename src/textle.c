#include "textle.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WORD_LINE_SIZE 256
#define INITIAL_WORD_CAPACITY 128

static void uppercase_word(char word[TEXTLE_WORD_LENGTH + 1])
{
    size_t i;

    for (i = 0; i < TEXTLE_WORD_LENGTH; ++i) {
        word[i] = (char)toupper((unsigned char)word[i]);
    }
    word[TEXTLE_WORD_LENGTH] = '\0';
}

static char *trim_line(char *line)
{
    size_t len = strlen(line);
    char *start = line;

    while (len > 0 && isspace((unsigned char)line[len - 1])) {
        line[--len] = '\0';
    }
    while (*start != '\0' && isspace((unsigned char)*start)) {
        ++start;
    }
    return start;
}

static int load_words(const char *path, char ***words_out, size_t *count_out)
{
    FILE *file = fopen(path, "r");
    char line[WORD_LINE_SIZE];
    char **words;
    size_t count = 0;
    size_t capacity = INITIAL_WORD_CAPACITY;

    if (file == NULL) {
        fprintf(stderr, "Textle: cannot open word list '%s': %s\n",
                path, strerror(errno));
        return -1;
    }

    words = malloc(capacity * sizeof(*words));
    if (words == NULL) {
        fprintf(stderr, "Textle: out of memory loading word list.\n");
        fclose(file);
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *word = trim_line(line);
        char **larger;
        char *copy;

        if (!textle_is_five_letters(word)) {
            continue;
        }

        uppercase_word(word);

        if (count == capacity) {
            capacity *= 2;
            larger = realloc(words, capacity * sizeof(*words));
            if (larger == NULL) {
                fprintf(stderr, "Textle: out of memory growing word list.\n");
                while (count > 0) free(words[--count]);
                free(words);
                fclose(file);
                return -1;
            }
            words = larger;
        }

        copy = malloc(TEXTLE_WORD_LENGTH + 1);
        if (copy == NULL) {
            fprintf(stderr, "Textle: out of memory loading word list.\n");
            while (count > 0) free(words[--count]);
            free(words);
            fclose(file);
            return -1;
        }

        memcpy(copy, word, TEXTLE_WORD_LENGTH + 1);
        words[count++] = copy;
    }

    if (ferror(file)) {
        fprintf(stderr, "Textle: error reading word list '%s'.\n", path);
        while (count > 0) free(words[--count]);
        free(words);
        fclose(file);
        return -1;
    }

    fclose(file);

    if (count == 0) {
        fprintf(stderr, "Textle: word list '%s' contains no five-letter words.\n",
                path);
        free(words);
        return -1;
    }

    *words_out = words;
    *count_out = count;
    return 0;
}

static void free_words(char **words, size_t count)
{
    size_t i;

    for (i = 0; i < count; ++i) free(words[i]);
    free(words);
}

static void print_instructions(void)
{
    puts("Welcome to Textle!");
    puts("You have six tries to guess a five-letter word.");
    puts("A letter in the correct place stays in place.");
    puts("A letter followed by * is in the word, but elsewhere.");
    puts("An underscore means no matching letter remains.");
    puts("Enter any five letters; guesses need not be dictionary words.");
    puts("Enter Q to quit the current game.");
    putchar('\n');
}

static int play_round(const char *answer)
{
    char input[WORD_LINE_SIZE];
    int attempt;

    for (attempt = 1; attempt <= TEXTLE_MAX_GUESSES; ++attempt) {
        char guess[TEXTLE_WORD_LENGTH + 1];
        char feedback[TEXTLE_WORD_LENGTH * 2 + 1];
        char *word;
        size_t i;

        printf("Guess %d of %d: ", attempt, TEXTLE_MAX_GUESSES);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            putchar('\n');
            return 1;
        }

        word = trim_line(input);

        if (strcmp(word, "Q") == 0 || strcmp(word, "q") == 0) {
            printf("The word was %s. Thanks for playing!\n", answer);
            return 1;
        }

        if (!textle_is_five_letters(word)) {
            puts("Please enter exactly five letters.");
            --attempt;
            continue;
        }

        for (i = 0; i < TEXTLE_WORD_LENGTH; ++i) {
            guess[i] = (char)toupper((unsigned char)word[i]);
        }
        guess[TEXTLE_WORD_LENGTH] = '\0';

        textle_score_guess(answer, guess, feedback);
        puts(feedback);

        if (strcmp(guess, answer) == 0) {
            puts("Correct!");
            return 0;
        }

        putchar('\n');
    }

    printf("Out of tries! The word was %s.\n", answer);
    return 0;
}

int main(int argc, char **argv)
{
    const char *word_path = argc > 1 ? argv[1] : "data/words.txt";
    char **words = NULL;
    size_t word_count = 0;
    int exit_status = EXIT_SUCCESS;
    char replay[32];

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [word-list-path]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (load_words(word_path, &words, &word_count) != 0) {
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL));
    print_instructions();

    do {
        size_t index = (size_t)rand() % word_count;
        int result;

        result = play_round(words[index]);
        if (result != 0) {
            break;
        }

        printf("\nPlay again? (Y/N): ");
        fflush(stdout);
        if (fgets(replay, sizeof(replay), stdin) == NULL) {
            break;
        }
        {
            char *answer = trim_line(replay);
            if (answer[0] != 'Y' && answer[0] != 'y') {
                break;
            }
        }
        putchar('\n');
    } while (1);

    free_words(words, word_count);
    return exit_status;
}
