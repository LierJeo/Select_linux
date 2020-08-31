CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra -pedantic -g 

SRC = server.c
OBJ = $(SRC:.c=.o)
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJSDIR)$(OBJ) -o $(OBJSDIR)$@
$(OBJ):%.o:%.c
	$(CXX) -c $< -o $(OBJSDIR)$@ $(CXXFLAGS) 
clean:
	rm -rf $(OBJSDIR)$(OBJ) $(OBJSDIR)$(EXEC)