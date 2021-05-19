.PHONY: server client client-prof

all: client-prof server

client: client/client.h client/message.h client/thread.h client/thread.c client/log.h client/log.c
	gcc client/client.c client/thread.c client/log.c -Wall -pthread -o test/c

server: server/serverMain.c server/serverMain.h server/serverConsumer.c server/serverConsumer.h server/serverProducer.c server/serverProducer.h server/log.c server/log.h server/delay.c server/delay.h server/lib.c server/lib.h server/queue.c server/queue.h 
	gcc server/serverMain.c server/serverConsumer.c server/serverProducer.c server/log.c server/delay.c server/lib.c server/queue.c -Wall -pthread -DDELAY=100 -o test/s

client-prof: client-prof/client.c client-prof/common.h
	gcc client-prof/client.c -Wall -pthread -o test/c

cpplint:
	cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting,-build/header_guard,-build/include --recursive client

valgrind:
	valgrind $(CMD)

infer:
	infer run -- make

clean:
	rm -f test/c test/s