.PHONY: all clean fclean

NAME	= so_long
SRC_DIR = src/
INC_DIR = inc/
MLX_DIR = mlx
OBJ_DIR = obj/
LIBFT_DIR = inc/libft/
LIBFT = libft.a

CCFLAGS += -Wall
CCFLAGS += -Wextra
CCFLAGS += -Werror
CCFLAGS += -Wpedantic
CCFLAGS += -Wconversion
CCFLAGS += -O2
CCFLAGS += -Wno-newline-eof

CPPFLAGS += -Imlx
CPPFLAGS += -I$(INC_DIR)
CPPFLAGS += -I$(MLX_DIR)
CPPFLAGS += -I$(LIBFT_DIR)

LDLIBS += -lmlx
LDLIBS += -lX11
LDLIBS += -lXext
LDLIBS += -lft

LDFLAGS	+= -L/usr/local/lib/
LDFLAGS	+= -L/usr/lib/
LDFLAGS += -L$(MLX_DIR)
LDFLAGS += -L$(LIBFT_DIR)


vpath %.c $(SRC_DIR)
SRC += map_initialization.c
SRC += map_validation.c
SRC += main.c
SRC += loading_and_cleaning.c

OBJ		:= $(SRC:.c=.o)
OBJ		:= $(addprefix $(OBJ_DIR), $(OBJ))

all: $(NAME)

$(LIBFT_DIR)$(LIBFT):
	make -C $(LIBFT_DIR)
$(NAME): $(OBJ) $(LIBFT_DIR)$(LIBFT)
	cc $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@
$(OBJ_DIR)%.o: %.c | $(OBJ_DIR)
	cc $(CCFLAGS) $(CPPFLAGS) -c $< -o $@
$(OBJ_DIR):
	mkdir -vp $@
clean:
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean
	rm -f ./textures/*.Identifier
	rm -f ./src/*.Identifier
	rm -f ./*.Identifier
	rm -f ./inc/libft/*.Identifier
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean
re: clean all

