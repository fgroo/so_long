#ifndef SO_LONG_H
# define SO_LONG_H

#include "./libft/get_next_line.h"
#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h> // Für malloc und free, falls eine dynamische Warteschlange benötigt wird
#include <fcntl.h>

// Struktur für einen Punkt (Pixelkoordinaten)
typedef struct s_point
{
	int x;
	int y;
} t_point;

typedef struct s_map
{
	int		rows;
	int		cols;
	char	*colsstring[1000];
	char	**screen;
} t_map;

typedef struct s_q
{
	int queue_size;
	int front;
	int rear;
	int itemCount;
	t_point p;
	t_point *queue;
} t_q ;

typedef struct s_comps
{
	t_point	exit;
	t_point	player;
	t_point	collectible;
	int		error_flag;
	int		check_e;
	int		check_p;
	int		check_c;
	int		i;
	int		j;
	int		collectibles_found;
	int		player_moves;
	void	*img_wall;
	void	*img_path;
	void	*img_player;
	void	*img_collectible;
	void	*img_exit_open;
	void	*img_exit_closed;
	void	*mlx_ptr;
	void	*win_ptr;
} t_comps;

typedef struct s_game_context
{
	t_comps	*comps;
	t_map	*map_data;
	// You could also include other global game resources here if needed
}	t_game_context;

char	*get_next_line(int fd);
int		ft_strlen_mod(char *s);
t_q enqueue(t_point p, t_q q);
t_q	dequeue(t_q q);

void	printScreenIter(char **screen, t_map map);
void	floodfilliterative(char **screen, t_point player, t_map map);
char	**initialize_map(char	**colsstring, int	cols, int rows);
t_point	insert_coordinates(int i, int j);
t_comps	save_map_components(char	**testscreen, int cols, int rows);
t_map	gnl_engine(void);
char	**rdy_for_floodfill(char **screen, t_comps map_components);

void	load_assets(t_comps *game);
void	parse_map(t_comps *game, char **map_data); // Um Startpos, Collectibles etc. zu finden
int		render_game_map(t_comps *game, t_map *map);
int 	handle_keypress(int keysym, void *param);
int		cleanup_and_exit(t_comps *game);
t_map	gnl_engine(void);

#endif