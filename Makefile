NAME = ircserv

CFLAGS = -Wall -Werror -Wextra -std=c++98

CC = c++

SOURCES = classes/*.cpp \
		  commands/channel_commands/*.cpp \
		  commands/server_commands/*.cpp \
		  program/*.cpp

all: $(NAME)

clean:

fclean: 
	rm -rf $(NAME)

re: fclean all