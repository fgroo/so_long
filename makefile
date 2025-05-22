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

CPPFLAGS += -Imlx
CPPFLAGS += -I$(MLX_DIR)

LDLIBS += -lmlx
LDLIBS += -lX11
LDLIBS += -lXext

LDFLAGS	+= -L/usr/local/lib/
LDFLAGS	+= -L/usr/lib/
LDFLAGS += -L$(MLX_DIR)
LDFLAGS += -L$(LIBFT_DIR)


vpath %.c $(SRC_DIR)
SRC += map_initialization.c
SRC += map_validation.c
SRC += game_logic.c

OBJ		:= $(SRC:.c=.o)
OBJ		:= $(addprefix $(OBJ_DIR), $(OBJ))

vpath $(INC_DIR)
INC += so_long.h
INC += $(LIBFT_DIR)$(LIBFT)



all: $(NAME)

$(LIBFT_DIR)$(LIBFT):
	make -C $(LIBFT_DIR)
$(NAME): $(OBJ) $(LIBFT_DIR)$(LIBFT)
	cc -g $(OBJ) $(LDFLAGS) $(LDLIBS) $(INC_DIR)$(INC) -o $@
$(OBJ_DIR)%.o: %.c | $(OBJ_DIR)
	cc -g $(CCFLAGS) $(CPPFLAGS) -c $< -o $@
$(OBJ_DIR):
	mkdir -vp $@
clean:
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)
	rm $(LIBFT_DIR)$(LIBFT)
	rm $(LIBFT_DIR)*.o
	rm -f ./textures/*.Identifier
fclean: clean
	rm -f $(NAME)
re: clean all

