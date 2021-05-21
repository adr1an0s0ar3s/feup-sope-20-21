.PHONY: server client

all: client server

client: client/client.c client/common.h
	gcc client/client.c client/common.h -Wall -pthread -o c

server: server/serverMain.c server/serverMain.h server/serverConsumer.c server/serverConsumer.h server/serverProducer.c server/serverProducer.h server/log.c server/log.h server/delay.c server/delay.h server/lib.c server/lib.h server/queue.c server/queue.h 
	gcc server/serverMain.c server/serverConsumer.c server/serverProducer.c server/log.c server/delay.c server/lib.c server/queue.c -Wall -pthread -DDELAY=100 -o s

cpplint:
	cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting,-build/header_guard,-build/include --recursive server

valgrind:
	valgrind $(CMD)

infer:
	infer run -- make

clean:
	rm -f c s