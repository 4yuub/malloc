ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := lib/libft_malloc_$(HOSTTYPE).so
LINK := lib/libft_malloc.so
BUILD_DIR := build 
SRC_DIR := src
RM := rm -rf
CC := cc
CFLAGS := -Wall -Wextra -Werror
SRCS := 
OBJS := $(SRCS:=.o)


all: $(LINK)

$(NAME): $(OBJS)
	@mkdir -p $(shell dirname $@)
	touch $@

$(LINK): $(NAME)
	@mkdir -p $(shell dirname $@)
	ln -sf $< $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME) $(LINK)

re: fclean all