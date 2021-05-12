.PHONY: server

all: client server

client: client/client.h client/message.h client/thread.h client/thread.c client/log.h client/log.c
	gcc client/client.c client/thread.c client/log.c -Wall -pthread -o test/c

server: server/serverMain.c server/serverConsumer.c server/serverProducer.c server/delay.c server/lib.c
	gcc server/serverMain.c server/serverConsumer.c server/serverProducer.c server/delay.c server/lib.c -Wall -pthread -DDELAY=100 -o test/s

cpplint:
	cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting,-build/header_guard,-build/include --recursive client

valgrind:
	valgrind -q --gen-suppressions=yes $(CMD)

infer:
	infer run -- make

clean:
	rm -f test/c test/s