all: athfw2lnx

ifeq ($(ATH_FW),)
$(error "ERROR: usage: make ATH_FW=fw.c")
endif

athfw2lnx: athfw2lnx.c $(ATH_FW)
	gcc -o $@ athfw2lnx.c $(ATH_FW) -DHOSTNAME=\"$(shell hostname)\"  -DCOMMIT_HASH=\"$(shell git rev-parse --short HEAD)\"

clean:
	rm -f athfw2lnx
