# Textle

A little five-letter word game for the terminal.

Textle is a small, dependency-free C word game inspired by the pleasure of old-school text interfaces. Guess the secret five-letter word in six tries.

## How to play

- Enter any five-letter guess. Guesses do not have to be dictionary words.
- A letter shown in its position is an exact match.
- A letter followed by `*` is in the answer, but in a different position.
- An underscore means the letter is not available at that position or has already been accounted for elsewhere in the guess.
- You have six guesses to find the answer.

The feedback algorithm handles repeated letters by marking exact matches first and then matching remaining letters one at a time.

## Build and run

Requirements: a C17-compatible compiler and Make.

```sh
make
./textle
```

Run the feedback tests with:

```sh
make test
```

The game looks for its bundled word list at `data/words.txt` relative to the current working directory. Run it from the project directory, or pass a word-list path as the first argument:

```sh
./textle /path/to/words.txt
```

## Project status

Early development. The initial version prioritizes a portable terminal interface and straightforward game logic. It is designed to work in ordinary Linux terminals and will be tested with serial-terminal setups such as the Brother PowerNote.

## License

Textle is released under the MIT License. See [LICENSE](LICENSE).
