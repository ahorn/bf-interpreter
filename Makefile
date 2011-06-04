CC = gcc
CFLAGS = -Wall

CFILES = bf.c vm.c
TARG = bf

all: $(TARG)

$(TARG): $(CFILES)
	$(CC) $(CFLAGS) -o $(TARG) $(CFILES) 

install: $(TARG) 
	install -m 755 $(TARG) /usr/local/bin
	gzip < $(TARG).1 > /usr/local/man/man1/$(TARG).1.gz

uninstall:
	rm -rf /usr/local/bin/$(TARG)

CLEANFILES+=$(TARG)
clean:
	rm -f *.o $(CLEANFILES)

.PHONY: all
.PHONY: clean
