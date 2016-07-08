## CREPE : grep like search tool

This project is inspired by [tkengo/highway](https://github.com/tkengo/highway).
`crepe` have much respect for `highway`.

`crepe` is still heavily under development.  
`crepe` does not support Windows system...

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
Usage: crepe [OPTIONS] pattern [PATH]
    -h, --help:            show this message
    -v, --version:         print crepe version
    --no-line-number:      disable line number print
    --no-group:            disable group output
    --no-color:            disable colorful output
```

If a directory name is given as `PATH`, `crepe` recursively explores the directory entries and tests the pattern for each entry file.  
If no `PATH` provided, `crepe` reads and searches from stdin.  

Currently, `crepe` can only handle exact matches.

`crepe` depends on [Boost C++ Libraries](http://www.boost.org/), which is licensed by [The Boost License](http://www.boost.org/users/license.html).
