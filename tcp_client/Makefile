CC = gcc

SRC = tcp_cl.c
OBJ = $(SRC:.c=.o)
EXEC = tcp_cl
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJSDIR)$(OBJ) -o $(OBJSDIR)$@
$(OBJ):%.o:%.c
	$(CC) -c $< -o $(OBJSDIR)$@ $(CFLAGS) 
clean:
	rm -rf $(OBJSDIR)$(EXEC) $(OBJSDIR)$(OBJ)