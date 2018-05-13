# logfind
Grep like program.

Usage:
`logfind [-o] word [... word_N]`

Find files which contain all provided words. 
Provide `-o` argument to enable OR search mode: find files which contain at least one word.

Create `.logfind` file to set filter rules. 
Example:
```
*.c
*.txt
Makefile
```
