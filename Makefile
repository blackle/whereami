project_name := whereami

all : libwhereami.so main

libwhereami.so : whereami.c whereami.h
	gcc -shared -o $@ $< -fPIC -Wall -Werror -Wextra

libwhereamitest.so : test.c test.h whereami.h libwhereami.so
	gcc -shared -o $@ $< -lwhereami -L. -fPIC -Wall -Werror -Wextra

main : main.c test.h libwhereamitest.so whereami.h libwhereami.so
	gcc -o $@ $< -lwhereami -lwhereamitest -L. -Wl,-rpath='$$ORIGIN' -Wall -Werror -Wextra 

.PHONY: clean

clean:
	rm main libwhereamitest.so libwhereami.so