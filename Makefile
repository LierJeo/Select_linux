CC = gcc

OBJ = $(SRC:.c=.o)
EXEC = server udp_client tcp_client

export OBJSDIR = ${shell pwd}/bin/
export CFLAGS = -Wall -Werror -Wextra -O2 -DDEBUG -I../common -I../log

$(EXEC):$(OBJSDIR)
	$(MAKE) -C server
	$(MAKE) -C tcp_client
	$(MAKE) -C udp_client
$(OBJSDIR):
	mkdir -p $(dir $@)
clean:
	rm -rf $(OBJSDIR)