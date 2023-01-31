NAME = ircserv

CC = c++

FLAGS = -g -Werror -Wall -Wextra -std=c++98

SOURCES = ./logic/classes/*.cpp \
		  ./logic/commands/channel_commands/*.cpp \
		  ./logic/commands/server_commands/*.cpp \
		  ./logic/main.cpp \
		  ./logic/utils.cpp

$(NAME):
		$(CC) $(FLAGS) $(SOURCES) -o $(NAME)

all: $(NAME)

clean:

fclean: clean
	@rm -rf $(NAME)

re: fclean all
