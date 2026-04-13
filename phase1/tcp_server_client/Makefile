NAME_SERVER = server
NAME_CLIENT = client

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRV_SRC = server.c
CLI_SRC = client.c

SRV_OBJ = $(SRV_SRC:.c=.o)
CLI_OBJ = $(CLI_SRC:.c=.o)

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER): $(SRV_OBJ)
	$(CC) $(CFLAGS) $(SRV_OBJ) -o $(NAME_SERVER)

$(NAME_CLIENT): $(CLI_OBJ)
	$(CC) $(CFLAGS) $(CLI_OBJ) -o $(NAME_CLIENT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(SRV_OBJ) $(CLI_OBJ)

fclean: clean
	$(RM) $(NAME_SERVER) $(NAME_CLIENT)

re: fclean all

.PHONY: all clean fclean re