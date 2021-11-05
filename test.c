#include <stdio.h>
#include <limits.h>
#include "whereami.h"
#include "test.h"

__attribute__((noinline))
void whereami__test()
{
	char path[PATH_MAX];
	ssize_t error;

	if ((error = whereami(path, PATH_MAX)) < 0) {
		printf("[test] error: %ld\n", error);
	} else {
		printf("[test] %s\n", path);
	}
}