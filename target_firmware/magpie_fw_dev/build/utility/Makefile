DIRS = bin2hex

all:
	@for i in $(DIRS) ; do $(MAKE) -C $$i || exit $? ; done

clean :
	@for i in $(DIRS) ; do $(MAKE) -C $$i clean; done
