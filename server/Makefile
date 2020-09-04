CC = gcc -g

SRC = server.c ../log/log.c
OBJ = $(SRC:.c=.o)
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJ)
	cd $(OBJSDIR) && $(CC) $(CFLAGS) $(notdir $(OBJ)) -o $(OBJSDIR)$@
$(OBJ):%.o:%.c
	$(CC) -c $< -o $(OBJSDIR)$(notdir $@) $(CFLAGS) 
clean:
	rm -rf $(OBJSDIR)$(OBJ) $(OBJSDIR)$(EXEC)
