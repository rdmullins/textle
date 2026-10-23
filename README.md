# Textle

A little five-letter word game for the terminal.

Textle is a small, dependency-free C word game. Guess the secret five-letter word in six tries.

## How to play

- Enter any five-letter guess. It does not need to be a dictionary word.
- A letter in the correct position is shown in place.
- A letter followed by `*` is in the answer but in a different position.
- An underscore means no unused copy of that letter remains in the answer.
- You have six guesses. Enter `Q` to quit a round.

The scoring algorithm handles repeated letters by matching exact positions first, then marking misplaced letters from any remaining copies in the answer.

## Build and run

Requirements: a C17-compatible compiler and Make.

```sh
make
./textle
```

Run the scoring tests:

```sh
make test
```

The game loads `data/words.txt` relative to the current working directory. You can supply an alternate list as the first argument:

```sh
./textle /path/to/words.txt
```

## Project status

Early development. Textle is designed for ordinary Linux terminals and will also be tested with serial-terminal setups such as the Brother PowerNote.

## License

MIT. See [LICENSE](LICENSE).
