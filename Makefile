ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := lib/libft_malloc_$(HOSTTYPE).so
LINK := lib/libft_malloc.so
LIB_DIR := lib
BUILD_DIR := build
INC_DIR := include
SRC_DIR := src
RM := rm -rf
CC := cc
CFLAGS := -Wall -Wextra -Werror -nostdlib
SRCS := malloc free realloc show_alloc_mem manage
OBJS := $(SRCS:=.o)


all: $(LINK)

$(NAME): $(addprefix $(BUILD_DIR)/, $(OBJS)) $(LIB_DIR)
	$(CC) -shared -o $@ $(addprefix $(BUILD_DIR)/, $(OBJS))

$(LINK): $(NAME)
	ln -sf $< $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR)

$(BUILD_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir -p $@

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re