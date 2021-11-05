//CC0 1.0 Universal https://creativecommons.org/publicdomain/zero/1.0/
//To the extent possible under law, Blackle Mori has waived all copyright and related or neighboring rights to this work.

#pragma once

__attribute__((noinline))
ssize_t whereami(char* path, size_t length);

ssize_t whereisthis(void* addr, char* path, size_t length);

__attribute__((noinline))
ssize_t whereami__self();
