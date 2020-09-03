CC = gcc

SRC = udp_cl.c
OBJ = $(SRC:.c=.o)
EXEC = $(basename $(SRC))
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJSDIR)$(OBJ) -o $(OBJSDIR)$@
$(OBJ):%.o:%.c
	$(CC) -c $< -o $(OBJSDIR)$@ $(CFLAGS) 
clean:
	rm -rf $(OBJSDIR)$(EXEC) $(OBJSDIR)$(OBJ)