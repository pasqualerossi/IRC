NAME = ircserv

CC = c++

FLAGS = -g -Werror -Wall -Wextra -std=c++98

SOURCES = ./Logic/commands/channel_commands/*.cpp \
		  ./Logic/commands/server_commands/*.cpp \
		  ./Logic/classes.cpp \
		  ./Logic/main.cpp \
		  ./Logic/utils.cpp

DSYM = ./ircserv.dSYM

$(NAME):
		$(CC) $(FLAGS) $(SOURCES) -o $(NAME)

all: $(NAME)

clean:

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(DSYM)

re: fclean all