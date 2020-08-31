CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra -O2

SRC = tcp_cl.c
OBJ = $(SRC:.c=.o)
EXEC = tcp_cl
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJSDIR)$(OBJ) -o $(OBJSDIR)$@
$(OBJ):%.o:%.c
	$(CXX) -c $< -o $(OBJSDIR)$@ $(CXXFLAGS) 
clean:
	rm -rf $(OBJSDIR)$(EXEC) $(OBJSDIR)$(OBJ)