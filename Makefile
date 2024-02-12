NAME = ircserv

SRC = main.cpp

FLAGS = -Wall -Wextra -Werror -std=c++98

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(SRC)
	@c++ $(FLAGS) -o $(NAME) $(SRC)

clean:
	@rm -f $(NAME)

fclean: clean
	@rm -f $(NAME) client

re: fclean all

client: client.cpp
	@c++ $(FLAGS) -o client client.cpp

debug: FLAGS += -g
debug: re