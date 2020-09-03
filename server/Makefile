CC = gcc

SRC = server.c 
OBJ = $(SRC:.c=.o)
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJSDIR)$(OBJ) -o $(OBJSDIR)$@
$(OBJ):%.o:%.c
	$(CC) -c $< -o $(OBJSDIR)$@ $(CFLAGS) 
clean:
	rm -rf $(OBJSDIR)$(OBJ) $(OBJSDIR)$(EXEC)