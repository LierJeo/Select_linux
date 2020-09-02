CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra -O2

SRC = udp_cl.c
OBJ = $(SRC:.c=.o)
EXEC = $(basename $(SRC))
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJSDIR)$(OBJ) -o $(OBJSDIR)$@
$(OBJ):%.o:%.c
	$(CXX) -c $< -o $(OBJSDIR)$@ $(CXXFLAGS) 
clean:
	rm -rf $(OBJSDIR)$(EXEC) $(OBJSDIR)$(OBJ)