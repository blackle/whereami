#include <stdio.h>
#include <limits.h>
#include "whereami.h"
#include "test.h"

int main()
{
	char path[PATH_MAX];
	ssize_t error;
	if ((error = whereami(path, PATH_MAX)) < 0) {
		printf("[main] error: %ld\n", error);
	} else {
		printf("[main] %s\n", path);
	}

	whereami__test();

	if ((error = whereami__self(path, PATH_MAX)) < 0) {
		printf("[whereami] error: %ld\n", error);
	} else {
		printf("[wheremai] %s\n", path);
	}

	static int myvar = 0;
	if ((error = whereisthis(&myvar, path, PATH_MAX)) < 0) {
		printf("[myvar] %ld\n", error);
	} else {
		printf("[myvar] %s\n", path); // the path to the executable library that contains myvar
	}
	return 0;
}
