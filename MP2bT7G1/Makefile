.PHONY: server client

all: client server

client: client/client.c client/common.h
	gcc client/client.c client/common.h -Wall -pthread -o c

server: server/*.c server/*.h
	gcc server/*.c -Wall -pthread -DDELAY=100 -o s

cpplint:
	cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting,-build/header_guard,-build/include --recursive server

valgrind:
	valgrind $(CMD)

infer:
	infer run -- make

clean:
	rm -f c s