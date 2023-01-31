NAME = ircserv

CC = c++

FLAGS = -g -Werror -Wall -Wextra -std=c++98

SOURCES = ./Logic/classes/*.cpp \
		  ./Logic/commands/channel_commands/*.cpp \
		  ./Logic/commands/server_commands/*.cpp \
		  ./Logic/main.cpp \
		  ./Logic/utils.cpp

$(NAME):
		$(CC) $(FLAGS) $(SOURCES) -o $(NAME)

all: $(NAME)

clean:

fclean: clean
	@rm -rf $(NAME)

re: fclean all