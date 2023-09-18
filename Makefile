NAME		= libsocky.so

CC			= gcc

ARCHIVER	= ar

SRCS		= src/create.c
SRCS		+= src/destroy.c
SRCS		+= src/shutdown.c
SRCS		+= src/listen.c
SRCS		+= src/get_port.c
SRCS 		+= src/get_address_str.c
SRCS 		+= src/get_address.c
SRCS		+= src/accept.c
SRCS		+= src/connect.c
SRCS		+= src/connect_from_string.c
SRCS		+= src/write.c
SRCS		+= src/send.c
SRCS		+= src/read.c
SRCS		+= src/recv.c
SRCS		+= src/set_options.c


TESTS_SRCS	:= $(SRCS)
TESTS_SRCS	+= tests/create_tests.c
TESTS_SRCS	+= tests/destroy_tests.c
TESTS_SRCS	+= tests/listen_tests.c
TESTS_SRCS	+= tests/get_port_tests.c
TESTS_SRCS	+= tests/get_address_str_tests.c
TESTS_SRCS	+= tests/get_address_tests.c
TESTS_SRCS	+= tests/accept_tests.c
TESTS_SRCS	+= tests/connect_tests.c
TESTS_SRCS	+= tests/connect_from_string_tests.c
TESTS_SRCS	+= tests/write_tests.c
TESTS_SRCS	+= tests/send_tests.c
TESTS_SRCS	+= tests/read_tests.c
TESTS_SRCS	+= tests/recv_tests.c

OBJS		= $(SRCS:.c=.o)
TESTS_OBJS	= $(TESTS_SRCS:.c=.o)

RM			= rm -f

CFLAGS		+= -Wall -Wextra -fPIC -pedantic
CPPFLAGS	+= -I ./include
LDFLAGS		= -shared

GREEN=`tput setaf 2`
RED=`tput setaf 1`
YELLOW=`tput setaf 3`
NO_COLOR=`tput sgr0`

%.o : %.c
	@$ $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo "$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ ["$(GREEN)"OK"$(NO_COLOR)"]"
.SUFFIXES: .o .c

all: $(NAME)

$(NAME): $(OBJS)
	@$ $(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	@echo "$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"

tests_run: $(TESTS_OBJS)
	@echo $(TESTS_OBJS)
	@$ $(CC) -lcriterion $(TESTS_OBJS) $(LIBS) -o $@
	@echo "$(CC) -lcriterion $(TESTS_OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"
	./$@
	@pkill nc || true
	@pkill $@ || true
	@$(RM) $@
	@$(RM) $(TESTS_OBJS)

val_run: CFLAGS += -g3
val_run: $(TESTS_OBJS)
	@$ $(CC) -lcriterion $(TESTS_OBJS) $(LIBS) -o $@
	@echo "$(CC) -lcriterion $(TESTS_OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"
	valgrind --trace-children=yes --quiet ./$@
	@pkill nc || true
	@pkill $@ || true
	@$(RM) $@
	@$(RM) $(TESTS_OBJS)

debug: CFLAGS += -g3
debug: re

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(NAME:.so=.a)

re: fclean all

install: re
	@cp $(NAME) /usr/lib/$(NAME) 2> /dev/null || \
	printf "\033[1m\033[31mError : try sudo make install\033[0m\n" && \
	cp include/socky.h /usr/include/ 2> /dev/null && \
	printf "\033[1m\033[32mLibrary $(NAME) successfully installed !\033[0m\n"

static: $(OBJS)
	$(ARCHIVER) rc $(NAME:.so=.a) $(OBJS)

.PHONY: all clean fclean re tests_run val_run debug install static
