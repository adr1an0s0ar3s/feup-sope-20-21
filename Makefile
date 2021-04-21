all: client server

client: src/client.h src/message.h src/thread.h src/thread.c
	gcc src/client.c src/thread.c -Wall -pthread -o test/client

server: src/server.o src/lib.o src/delay.h src/delay.c
	gcc src/server.o src/lib.o src/delay.c -Wall -pthread -DDELAY=0 -o test/server

cpplint:
	cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting,-build/header_guard,-build/include --recursive src

valgrind:
	valgrind -q --gen-suppressions=yes $(CMD)

infer:
	infer run -- make

clean:
	rm -f test/client test/server