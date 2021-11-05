#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "whereami.h"

__attribute__((noinline))
ssize_t whereami(char* path, size_t length)
{
	void *return_addr = __builtin_extract_return_addr(__builtin_return_address(0));

	return whereisthis(return_addr, path, length);
}

ssize_t whereisthis(void* addr, char* path, size_t length)
{
	FILE* maps = fopen("/proc/self/maps", "r");
	if (maps == NULL)
		return -1;

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int retval = -1;

	while ((read = getline(&line, &len, maps)) != -1) {
		//turn first - and ' ' into null char so we have null terminated fields
		char* delim = strchr(line, '-');
		char* field = strchr(line, ' ');
		if (delim == NULL || field == NULL)
			continue;

		*delim = '\0';
		*field = '\0';

		char* start_field = line;
		char* end_field = delim + 1;
		size_t start = strtol(start_field, NULL, 16);
		size_t end = strtol(end_field, NULL, 16);

		if (start == 0 || end == 0)
			continue;

		// continue if return address is outside map range
		if (start > (size_t)addr || end <= (size_t)addr)
			continue;

		//seek to path field - there are 4 space-delimited fields inbetween
		for (int i = 0; i < 4 && field != NULL; i++)
			field = strchr(field+1, ' ');

		if (field == NULL)
			break;

		// maps file is extremely space-padded, this removes the front pad
		size_t padding = strspn(field, " ");
		field += padding;

		//remove newline
		char* newline = strchr(field, '\n');
		if (newline != NULL)
			*newline = '\0';

		size_t field_len;
		if ((field_len = strlen(field)) <= 0)
			break;

		// we shouldn't try to copy if there's not enough room for the string + null terminator
		if (field_len >= length)
			break;

		strncpy(path, field, length);
		retval = field_len;

		break;
	}

	fclose(maps);

	if (line != NULL)
		free(line);

	return retval;
}

__attribute__((noinline))
ssize_t whereami__self(char* path, size_t length)
{
	return whereami(path, length);
}