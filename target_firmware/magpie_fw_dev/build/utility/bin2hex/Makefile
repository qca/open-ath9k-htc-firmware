TARGET = ../bin/bin2hex		# for Linux environment

E=echo

all: $(TARGET)

$(TARGET):
	gcc -o $(TARGET) bin2hex.c
	@$(E) "  CC " $@

clean:
	-rm $(TARGET)
