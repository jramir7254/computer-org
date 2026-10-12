# Project 1: Tokenizer

# Makefile symbols explained

Grouped by where you'll see them in your Makefile.

## Structure symbols

| Symbol | Meaning                                               | Example from your file         |
| ------ | ----------------------------------------------------- | ------------------------------ |
| `#`    | Comment (ignored by make)                             | `# Variables`                  |
| `=`    | Assign a variable                                     | `CC = gcc`                     |
| `:`    | Separates target from prerequisites                   | `all: $(ALL_TARGETS)`          |
| Tab    | Starts a recipe line (must be a real tab)             | the indented `$(CC) ...` lines |
| `\|`   | Everything after it is an **order-only** prerequisite | `... \| $(BIN_DIR)`            |

On `|`: the prerequisite must exist before the rule runs, but a newer timestamp on it never triggers a rebuild. That suits directories, whose timestamps change whenever a file is added.

## Variable symbols

| Symbol    | Meaning                                                                   |
| --------- | ------------------------------------------------------------------------- |
| `$(NAME)` | Substitute the value of variable `NAME`. `$(CC)` becomes `gcc`.           |
| `$$`      | A literal `$` (not in your file, but needed if a shell command needs one) |

Parentheses are needed for multi-letter names. `$(CC)` is correct, and `$CC` would be read as `$C` followed by a literal `C`.

Other assignment operators (not in your file, but common):

| Operator | Meaning                                                     |
| -------- | ----------------------------------------------------------- |
| `:=`     | Evaluate once, immediately                                  |
| `?=`     | Set only if not already set (lets `make CC=clang` override) |
| `+=`     | Append to the existing value                                |

## Pattern symbols

**`%`** is a wildcard that matches any non-empty text, called the _stem_. Whatever it matches on the left side is reused on the right:

```make
$(BUILD_DIR)/app_%.o: $(APP_DIR)/%.c
```

To build `build/app_wordle.o`, make matches `%` = `wordle`, so the prerequisite is `apps/wordle.c`.

Don't confuse it with `*`, which is a shell-style glob used inside `$(wildcard ...)`:

```make
$(wildcard src/*.c)    # * expands to real files on disk: src/list.c src/utils.c
```

The difference is that `*` matches files that exist right now, while `%` matches target names inside a rule.

## Automatic variables

These are filled in by make for each rule that runs:

| Symbol | Meaning                                                          | In `bin/wordle: build/app_wordle.o build/list.o | bin` |
| ------ | ---------------------------------------------------------------- | ----------------------------------------------- | ---- |
| `$@`   | The target                                                       | `bin/wordle`                                    |
| `$<`   | The first prerequisite                                           | `build/app_wordle.o`                            |
| `$^`   | All prerequisites (duplicates removed, order-only ones excluded) | `build/app_wordle.o build/list.o`               |

That's why compile rules use `$<` (just the one `.c` file) and link rules use `$^` (all the `.o` files).

## Functions

Functions use the form `$(name arguments)`:

| Function                   | What it does                                                |
| -------------------------- | ----------------------------------------------------------- |
| `$(wildcard pattern)`      | Expands to the list of files matching the pattern           |
| `$(patsubst from,to,list)` | Rewrites every item in `list` that matches `from` into `to` |

Example:

```make
$(patsubst src/%.c, build/%.o, src/list.c src/utils.c)
# -> build/list.o build/utils.o
```

A shorthand form of the same thing is `$(SRCS:src/%.c=build/%.o)`.

## Compiler flags (in the recipes, not make syntax)

| Flag            | Meaning                                  |
| --------------- | ---------------------------------------- |
| `-Wall -Wextra` | Turn on extra warnings                   |
| `-O2`           | Optimization level 2                     |
| `-Iinclude`     | Add `include/` to the header search path |
| `-c`            | Compile only, produce a `.o`, don't link |
| `-o file`       | Name of the output file                  |

So `$(CC) $(CFLAGS) -c $< -o $@` means "compile this one source file into this one object file," and `$(CC) $(CFLAGS) $^ -o $@` means "link all these objects into this executable."

## Symbols from my suggested fixes

| Symbol                        | Meaning                                                                                                      |
| ----------------------------- | ------------------------------------------------------------------------------------------------------------ |
| `@` at start of a recipe line | Don't echo the command before running it (`@echo hi`)                                                        |
| `-` at start of a line        | Ignore errors from that line. Before `include`, it means don't fail if the file is missing: `-include foo.d` |
| `.PHONY:`                     | Declares targets that are actions, not files                                                                 |
| `.SECONDARY:`                 | Tells make not to auto-delete intermediate files                                                             |
| `-MMD -MP`                    | Compiler flags that generate `.d` header-dependency files                                                    |
| `\` at end of line            | Continues the line onto the next one                                                                         |

## Putting it together

Take this line:

```make
$(BIN_DIR)/%: $(BUILD_DIR)/app_%.o $(UTILS_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
```

Read it as:

1. `$(BIN_DIR)/%` is the target pattern: any file in `bin/`, with its name captured as the stem.
2. `:` separates it from what it needs.
3. `$(BUILD_DIR)/app_%.o` is the matching object file, built from the same stem.
4. `$(UTILS_OBJS)` is every shared object file.
5. `| $(BIN_DIR)` means `bin/` must exist first, but it doesn't force a relink.
6. The tab-indented recipe links all the prerequisites (`$^`) into the target (`$@`).

If any of these still feels fuzzy, tell me which one and I'll walk through a concrete example with `make -n` output so you can see exactly what each symbol expands to.
