CXX = gCC
CXXFLAGS = -Wall -Werror -Wextra -O2

OBJ = $(SRC:.c=.o)
EXEC = MAIN

export OBJSDIR = ${shell pwd}/bin/

$(EXEC):$(OBJSDIR)
	$(MAKE) -C server
	$(MAKE) -C tcp_client
$(OBJSDIR):
	mkdir -p $(dir $@)
clean:
	rm -rf $(OBJSDIR)