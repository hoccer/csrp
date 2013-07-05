
SSL_CFLAGS=$(shell pkg-config --cflags openssl)
SSL_LDFLAGS=$(shell pkg-config --libs openssl)

CFLAGS+=-Wall -I. -D_XOPEN_SOURCE=700 -g

CFLAGS+=${SSL_CFLAGS}
LDFLAGS+=${SSL_LDFLAGS}

TESTS=test_srp

UTILS=srp-generator srp-server srp-client

all: srp.o ${UTILS}
.PHONY: all

test: ${TESTS}
	./test_srp
.PHONY: test

clean:
	rm -f ${TESTS} ${UTILS} *.o
.PHONY: clean

test_srp: srp.o test_srp.o
srp-client: srp.o srp-utils.o srp-client.o
srp-server: srp.o srp-utils.o srp-server.o
srp-generator: srp.o srp-utils.o srp-generator.o
