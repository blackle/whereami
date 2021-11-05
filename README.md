## whereami

A linux library to get the file path of the currently running shared library. Emulates use of Win32 GetModuleHandleEx/GetModuleFilename.

### usage

Dynamically link `libwhereami.so` or statically compile `whereami.c` into your program. The `whereami` function will give the file path of the code that called it:

```c
void my_function() {
	char path[PATH_MAX];
	ssize_t error;
	if ((error = whereami(path, PATH_MAX)) < 0) {
		printf("error: %ld\n", error);
	} else {
		printf("%s\n", path); // the path to the executable library that contains the line where whereami was called
	}
}
```

Also supplied is a function `whereisthis` that can be used to query the location of a function or static variable:

```c
void my_function() {
	static int myvar = 0;

	char path[PATH_MAX];
	ssize_t error;

	if ((error = whereisthis(&my_function, path, PATH_MAX)) < 0) {
		printf("error: %ld\n", error);
	} else {
		printf("my_function is in %s\n", path); // the path to the executable library that contains my_function
	}

	if ((error = whereisthis(&myvar, path, PATH_MAX)) < 0) {
		printf("error: %ld\n", error);
	} else {
		printf("myvar is in %s\n", path); // the path to the executable library that contains myvar
	}
}
```

The function `whereami__self` gives the file path for the whereami library itself. Only really useful for testing.

### how it works

On linux, the files associated with mapped addresses can be read from the file `/proc/self/maps`. This is a kind of virtual file that the kernel automatically generates depending on the process accessing it. It has this format:

```
$ cat /proc/self/maps
5596711e9000-5596711eb000 r--p 00000000 fd:00 19136663                   /usr/bin/cat
5596711eb000-5596711f0000 r-xp 00002000 fd:00 19136663                   /usr/bin/cat
5596711f0000-5596711f3000 r--p 00007000 fd:00 19136663                   /usr/bin/cat
5596711f3000-5596711f4000 r--p 00009000 fd:00 19136663                   /usr/bin/cat
5596711f4000-5596711f5000 rw-p 0000a000 fd:00 19136663                   /usr/bin/cat
5596728c0000-5596728e1000 rw-p 00000000 00:00 0                          [heap]
7f635ba63000-7f635ba85000 rw-p 00000000 00:00 0 
7f635ba85000-7f635bff5000 r--p 00000000 fd:00 19137364                   /usr/lib/locale/locale-archive
7f635bff5000-7f635c01a000 r--p 00000000 fd:00 19138679                   /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f635c01a000-7f635c192000 r-xp 00025000 fd:00 19138679                   /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f635c192000-7f635c1dc000 r--p 0019d000 fd:00 19138679                   /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f635c1dc000-7f635c1dd000 ---p 001e7000 fd:00 19138679                   /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f635c1dd000-7f635c1e0000 r--p 001e7000 fd:00 19138679                   /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f635c1e0000-7f635c1e3000 rw-p 001ea000 fd:00 19138679                   /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f635c1e3000-7f635c1e9000 rw-p 00000000 00:00 0 
7f635c202000-7f635c203000 r--p 00000000 fd:00 19138675                   /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f635c203000-7f635c226000 r-xp 00001000 fd:00 19138675                   /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f635c226000-7f635c22e000 r--p 00024000 fd:00 19138675                   /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f635c22f000-7f635c230000 r--p 0002c000 fd:00 19138675                   /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f635c230000-7f635c231000 rw-p 0002d000 fd:00 19138675                   /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f635c231000-7f635c232000 rw-p 00000000 00:00 0 
7ffea868f000-7ffea86b0000 rw-p 00000000 00:00 0                          [stack]
7ffea87c4000-7ffea87c8000 r--p 00000000 00:00 0                          [vvar]
7ffea87c8000-7ffea87ca000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 --xp 00000000 00:00 0                  [vsyscall]
```

The function `whereisthis` simply opens this file and parses it, searching for the range containing the passed address. The function `whereami` uses `__builtin_return_address` to get the address of the function caller, and passes that address to `whereisthis`.

## license

![CC0 license badge](https://licensebuttons.net/p/zero/1.0/88x31.png)

CC0 1.0 Universal https://creativecommons.org/publicdomain/zero/1.0/
To the extent possible under law, Blackle Mori has waived all copyright and related or neighboring rights to this work.
