NAME = ircserv

CC = c++

FLAGS = -Wall -Wextra -Werror

SOURCES = ./classes/*.cpp \
		  ./commands/channel_commands/*.cpp \
		  ./commands/server_commands/*.cpp \
		  ./program/*.cpp 

$(NAME):
		$(CC) $(FLAGS) $(SOURCES) -Iheaders -o $(NAME)

all: $(NAME)

clean:

fclean: clean
	@rm -rf $(NAME)

re: fclean all