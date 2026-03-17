# injector.h

`injector.h` is an STB-style header-only injector library.

## Usage

Just do `#include "injector.h"` and do `#define _INJECTOR_IMPL` before it.

`injector.h` contains two functions:

```c
// inject <len> bytes of <data> into file <filename>. always assumes that file <filename> does exist.
void inject(char *filename, char *data, int len);
// read the injected data into file <filename>. if anything failed or no data injected, return NULL.
char *get_inject_content(char *filename);
```

It's effects are noted in the comments above.

## Repository hierachy

`README.md`: this file.

`injector.h`: main source code.

`self_injector.c`: sample usage of `injector.h`. This file is an executable that injects some other file into itself. The injected executable will output the injected file if no arguments offered; but it can perform its original function by giving normal arguments and add a `-r` flag in the end.
