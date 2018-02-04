NAME	=	libmy_malloc.so

CC	=	gcc

SRCS	=	malloc.c \
		processing.c \
		show_mem.c

OBJS	=	$(SRCS:.c=.o)

CFLAGS	+=	-fPIC -lpthread -W -Wall -Werror -Wextra

CFLAGS	+=	 -I./ -I./include

LFLAGS	+=	-shared

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)

clean:
		rm -f $(OBJS)

fclean:		clean
		rm -f $(NAME)

re:		fclean all

.PHONY:		all clean fclean re
