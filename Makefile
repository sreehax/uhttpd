CFLAGS=-Wall -lssl -lcrypto -lisc -Llib -Iinclude
all:
	cd src && gcc $(CFLAGS) -c *.c
	cd src/lib && gcc $(CFLAGS) -c *.c
	cd src/lib && ar rcs libuhttpd.a *.o && cd .. # build static library
	cd src && gcc $(CFLAGS) -o ../uhttpd *.o lib/*.a
clean:
	find src -name '*.o' -o -name '*.a' | xargs rm
	rm uhttpd
