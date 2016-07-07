## CREPE : grep like search tool

This project is inspired by [tkengo/highway](https://github.com/tkengo/highway).
`crepe` have much respect for `highway`.

`crepe` is still heavily under development.

## Features

- Recursive exploration.
- Skip binary files.
- ... (under development...)

## Install

```
git clone https://github.com/agatan/crepe
cd crepe
make
```

## Usage

```
crepe <PATTERN> <SEARCH_PATH>
```

If a directory name is given as `SEARCH_PATH`, `crepe` recursively explores the directory entries and tests the pattern for each entry file.
Currently, `crepe` can only handle exact matches.

`crepe` depends on [Boost C++ Libraries](http://www.boost.org/), which is licensed by [The Boost License](http://www.boost.org/users/license.html).
