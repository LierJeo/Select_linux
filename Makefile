CC = gCC

OBJ = $(SRC:.c=.o)
EXEC = MAIN

export OBJSDIR = ${shell pwd}/bin/
export CFLAGS = -Wall -Werror -Wextra -O2 -DDEBUG

$(EXEC):$(OBJSDIR)
	$(MAKE) -C server
	$(MAKE) -C tcp_client
	$(MAKE) -C udp_client

$(OBJSDIR):
	mkdir -p $(dir $@)
clean:
	rm -rf $(OBJSDIR)