/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgorlich <fgorlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:00:00 by fgorlich          #+#    #+#             */
/*   Updated: 2025/05/26 18:25:24 by fgorlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "get_next_line.h"
# include "../mlx/mlx.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_map
{
	int		rows;
	int		cols;
	char	*string[1000];
	char	**screen;
}	t_map;

typedef struct s_q
{
	int		queue_size;
	int		front;
	int		rear;
	int		itemcount;
	t_point	p;
	t_point	*queue;
}	t_q;

typedef struct s_comps
{
	t_point	exit;
	t_point	player;
	t_point	collectible;
	int		exit_reachable;
	int		collectible_reachable;
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
}	t_comps;

typedef struct s_game_ctx
{
	t_comps	*comps;
	t_map	*map_data;
}	t_game_ctx;

char			*get_next_line(int fd);
int				ft_strlen_mod(char *s);
t_q				enqueue(t_point p, t_q q);
t_q				dequeue(t_q q, t_game_ctx *ctx);
void			printscreeniter(char **screen, t_map map);
int				floodfilliterative(t_game_ctx *ctx, t_point player, t_map map);
char			**initialize_map(char	**colsstring, int cols, int rows);
t_point			insert_coordinates(int i, int j);
t_comps			save_map_components(char **testscreen, int cols, int rows);
t_map			gnl_engine(int ac, char *av);
char			**rdy_for_floodfill(char **screen, t_comps map_components);
void			load_assets(t_game_ctx *game);
void			render_game_map(t_comps *game, t_map *map);
int				handle_keypress(int keysym, void *param);
int				cleanup_and_exit(t_game_ctx *ctx);
void			if_del(t_game_ctx *ctx);
void			*if_img(char tile_type, t_comps *game);
t_game_ctx		combine_structs(t_comps *game, t_map *map);
void			update_chars(t_game_ctx *ctx);

#endif