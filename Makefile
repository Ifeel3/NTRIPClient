NAME	= NTRIPClient
SRC		= $(wildcard src/*.cpp)
OBJ		= $(SRC:%.cpp=%.o)
CC		= clang++
#CFLAGS	= -Wall -Werror -Wextra

.PHONY: all re clean fclean

%.o: %.cpp
	$(CC) $(CFLAGS) -c $? -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all