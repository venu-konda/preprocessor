# preprocessor

A small C preprocessor written in C. Given a `.c` file, it performs the three
classic preprocessing steps and writes the result to a `.i` file:

1. **Comment removal**: strips `/* ... */` and `// ...` comments.
2. **File inclusion**: expands `#include <file>` and `#include "file"`.
3. **Macro replacement**: substitutes object-like `#define NAME VALUE` macros.

It is an educational implementation of the preprocessing stage of a C
compiler, built from scratch with the standard library only. It is not a
drop-in replacement for `cpp` or `gcc -E`; see
[Behaviour and limitations](#behaviour-and-limitations) for what it does and
does not handle.

## Repository layout

Everything lives in the `PREPROCESSOR/` directory.

| File | Purpose |
|---|---|
| `preprocessor.c` | `main()`: argument handling and the stage pipeline |
| `readFromFile.c` | Reads a whole file into a heap buffer |
| `removeComments.c` | Stage 1: comment removal |
| `fileInclusion.c` | Stage 2: `#include` expansion |
| `macroReplacement.c` | Stage 3: `#define` substitution |
| `writeToFile.c` | Writes the buffer to the output file |
| `myheader.h` | Shared prototypes and standard includes |
| `makefile` | Builds the `a.out` binary from the files above |
| `1.c`, `4.c` | Sample inputs exercising macro replacement |
| `5.c`, `11.h` | Sample input exercising file inclusion (local and system header) |
| `a.c` | Plain sample C program |
| `preprocessor1.c` | Earlier single-file version of the whole tool (not built by the makefile; see below) |
| `*.o`, `a.out` | Build artifacts that were committed; safe to delete and regenerate |
| `ee_manase` | Unrelated text file (song lyrics), not part of the project |

## Building

Requirements: a C compiler available as `cc` (GCC or Clang) and `make`.

```sh
cd PREPROCESSOR
make -B
```

`make -B` forces a full rebuild. This matters on a fresh clone: a plain `make`
reports `'a.out' is up to date` and leaves the committed binary untouched,
and the committed `.o` files are a mix of 32-bit and 64-bit objects, so a
partial rebuild can fail at link time. The result is `a.out` in the same
directory.

There is no `clean` target. To clean up by hand:

```sh
rm -f *.o a.out *.i
```

## Usage

```sh
./a.out -E <input.c>
```

The only supported flag is `-E` (mirroring `gcc -E`). The output file name is
the input name with its **last character replaced by `i`**, so `4.c` produces
`4.i` and `prog.c` produces `prog.i`. There is no check on the input name, so
an input that already ends in `i` (for example a `.i` file from an earlier
run) is **overwritten in place**, and a name with no extension such as `prog`
is written to `proi`. Inputs whose name does not end in `i` are never
modified.

The name of every file pulled in by `#include` is printed to standard output
as it is processed.

Example, using the bundled sample:

```sh
$ ./a.out -E 4.c
$ cat 4.i
#define max 10
int main()
{
	int i=10;
	printf("QWDKJBJKWQ");
}
```

Error handling is minimal. A wrong argument count prints `invalid input` and
any flag other than `-E` prints `incorrect command`, both with exit status
`0`. A missing or unreadable input file, or a missing `#include` file,
**crashes the program with a segmentation fault** (exit status 139) and no
`.i` file is written. The `error` message the code tries to print in that case
is usually lost, because the crash happens before standard output is flushed.

## How it works

`main()` runs the stages in a fixed order, each taking the text buffer from the
previous one:

```
readFromFile -> removeComments -> fileInclusion -> macroReplacement -> writeToFile
```

The whole source file is held in one `malloc`/`realloc` buffer. Each stage
edits that buffer in place with `strstr`, `memmove` and `realloc`.

**Comment removal** scans for `/*` and deletes from there up to the first `*`
or the first character followed by `/` (see the limitations: this is not the
same as finding the `*/` pair). It scans for `//` and deletes through and
including the end of the line, so the next line is joined onto it. An
unterminated block comment prints `error` and leaves the rest of the file as
is.

**File inclusion** finds each `#include` directive, removes it, and reads the
named file. `<name>` is resolved under `/usr/include/`; `"name"` is resolved
relative to the current working directory. The file's contents are placed at
the **start of the buffer**, not at the position of the directive, and the
scan then continues from the inserted text.

**Macro replacement** finds each `#define NAME VALUE`, where `NAME` and
`VALUE` are separated by one or more space characters and `VALUE` runs to the
first space or newline, and replaces every later occurrence of `NAME` in the
buffer with `VALUE`. The `#define` line itself is left in the output.

## Behaviour and limitations

These are observed behaviours of the current code, each reproduced with the
built binary, listed so the output is not a surprise.

Things that crash the tool (segmentation fault, no `.i` file written):

- A missing or unreadable input file, or an `#include` whose file cannot be
  opened. Running `5.c` from outside `PREPROCESSOR/` is one way to hit this,
  since `"11.h"` is looked up in the current directory.
- More than one `#include <...>` in a file. The `/usr/include/` path buffer is
  reused without being reset, so the second system header is looked up as
  `/usr/include/stdio.hstdlib.h` and fails. Local `"..."` includes are not
  affected.
- A file whose last line has no trailing newline and ends in a `//` comment
  or a `#define`. The scanners run past the end of the buffer. An unterminated
  `#include` on the last line does not crash but appends stray text to the
  output. Make sure inputs and included files end with a newline.

Things that silently produce wrong output:

- Block comments containing a `/` or a `*` before the closing `*/` (URLs,
  paths, arithmetic, decorated banners) are only partly removed. The rest of
  the comment, including the `*/`, is emitted as source. `/* a/b */ int x;`
  becomes `b */ int x;`.
- String and character literals are not understood by any stage. `//` or
  `/*` inside a string is treated as a comment, so `"http://x"` is truncated,
  and macro names inside strings are replaced.
- Macro matching is plain substring matching. A macro named `max` will also
  rewrite the `max` inside `maximum`. Only occurrences after the `#define`
  are replaced.
- A macro value stops at the first space: `#define SUM 1 + 2` substitutes
  `1`. Only the space character is a separator, so a tab between name and
  value, or a `#define FLAG` with no value, is silently not expanded.
- Included files are prepended to the buffer, so with several `#include`
  lines the last one ends up first. Because scanning resumes from the inserted
  text, nested `#include` lines in a small included file are expanded too, but
  after a large header the scan pointer goes stale and the header's own
  includes are usually left in place. Do not rely on either outcome.
- Comments are removed before inclusion, so comments inside included headers
  remain in the output.
- Including `<stdio.h>` runs to completion, but the macro stage then rewrites
  the header's own `#define` lines with the one-token rule, so the output is a
  mangled copy of the header, not something a compiler will accept.

Not supported at all:

- Function-like macros (`#define SQ(x) ((x)*(x))`), `#undef`, `#ifdef` /
  `#if` / `#else` / `#endif`, `#pragma`, `#error`, and line continuations with
  `\`.
- The `#define` directive is not removed from the output.
- Fixed-size buffers: file names and include paths up to 99 characters, macro
  names up to 99 characters, macro values up to 499 characters.

## Sample inputs

| Input | What it demonstrates | Output file |
|---|---|---|
| `4.c` | One macro used once | `4.i` |
| `1.c` | One macro (`max`) used as the initializer of five variables; all five are replaced | `1.i` |
| `5.c` | A local include (`"11.h"`) and a system include (`<stdio.h>`) | `5.i` |
| `a.c` | Plain program with no directives beyond `<stdio.h>` | `a.i` |

Run them from inside `PREPROCESSOR/` so that `"11.h"` resolves; from any
other directory `5.c` crashes (see above).

## About `preprocessor1.c`

This is the original all-in-one version, containing its own `main()` plus the
read, comment-removal, inclusion and write functions in one file, with macro
replacement left as a stub. It is kept for reference and is **not** part of
the makefile build. Note that this version writes its output back over the
input file, unlike the modular version which writes a separate `.i` file.

To build it on its own:

```sh
cc preprocessor1.c -o preprocessor1
```

## Repository housekeeping

A `.gitignore` is included so that `*.o`, `a.out` and `*.i` are not committed
in future. The artifacts already in the repository have been left in place;
they can be removed with `git rm` at any time and are regenerated by `make`.
The `ee_manase` file is unrelated to the project and can be removed as well.

## License

MIT. See [LICENSE](LICENSE).
