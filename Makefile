OBJS = main.o mine_sweeper.o

main: $(OBJS)
	gcc $(OBJS) -o main $(LDFLAGS)

LIBS :=  
LIBDIRS := 
LDFLAGS := $(LIBDIRS) $(LIBS) $(LDFLAGS)
CFLAGS :=

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

.PHONY: all test clean
all: main test
test:
	./main
clean:
	rm -f main $(OBJS)
