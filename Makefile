NAME = ircserv

CFLAGS = -Wall -Werror -Wextra -std=c++98

CC = c++

SOURCES = classes/*.cpp \
		  channel_commands/*.cpp \
		  server_commands/*.cpp \
		  main.cpp \
		  utils.cpp

all: $(NAME)

clean:

fclean: 
	rm -rf $(NAME)

re: fclean all