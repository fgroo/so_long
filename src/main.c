/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgorlich <fgorlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:59:43 by fgorlich          #+#    #+#             */
/*   Updated: 2025/05/26 17:05:05 by fgorlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	render_game_map(t_comps *game, t_map *map)
{
	t_point	p;
	void	*current_img;
	char	tile_type;

	p = (t_point){0, 0};
	mlx_clear_window(game->mlx_ptr, game->win_ptr);
	while (p.y < map->rows)
	{
		p.x = -1;
		while (++p.x < map->cols)
		{
			tile_type = map->screen[p.y][p.x];
			current_img = if_img(tile_type, game);
			if (game->img_path)
				mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
					game->img_path, p.x * 64, p.y * 64);
			if (current_img != NULL && current_img != game->img_path)
				mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
					current_img, p.x * 64, p.y * 64);
		}
		p.y++;
	}
	if (game->img_player)
		mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
			game->img_player, game->player.y * 64, game->player.x * 64);
}

int	rec_mov(char target_tile, t_game_ctx *ctx, t_point *p)
{
	target_tile = ctx->map_data->screen[p->x][p->y];
	if (target_tile == '1')
		return (1);
	ctx->comps->player.x = p->x;
	ctx->comps->player.y = p->y;
	ctx->comps->player_moves++;
	printf("Moves: %d\n", ctx->comps->player_moves);
	if (target_tile == 'C')
	{
		ctx->map_data->screen[p->x][p->y] = '3';
		printf("Collected: %d\n", ++ctx->comps->collectibles_found);
	}
	else if (target_tile == 'E')
	{
		if (ctx->comps->collectibles_found == 1)
		{
			printf("Congratulations! You reached the exit in %d moves!\n",
				ctx->comps->player_moves);
			cleanup_and_exit(ctx);
		}
		else
			printf("Exit is closed! Collect item first (%d).\n",
				ctx->comps->collectibles_found);
	}
	return (0);
}

int	handle_keypress(int keysym, void *param)
{
	t_game_ctx	*ctx;
	t_point		p;

	ctx = (t_game_ctx *)param;
	p.x = ctx->comps->player.x;
	p.y = ctx->comps->player.y;
	if (keysym == XK_Escape)
		cleanup_and_exit(ctx);
	if (keysym == XK_w || keysym == XK_Up)
		p.x--;
	else if (keysym == XK_a || keysym == XK_Left)
		p.y--;
	else if (keysym == XK_s || keysym == XK_Down)
		p.x++;
	else if (keysym == XK_d || keysym == XK_Right)
		p.y++;
	else
		return (0);
	if (p.x < 0 || p.x >= ctx->map_data->rows
		|| p.y < 0 || p.y >= ctx->map_data->cols)
		return (0);
	if (rec_mov((char){0}, ctx, &p) == 1)
		return (0);
	return (render_game_map(ctx->comps, ctx->map_data), 0);
}

int	main(int ac, char **av)
{
	t_map		gnl;
	t_comps		map_components;
	t_game_ctx	game_ctx;

	gnl = gnl_engine(ac, av[1]);
	gnl.screen = initialize_map(gnl.string, gnl.cols, gnl.rows);
	map_components = save_map_components(gnl.screen, gnl.cols, gnl.rows);
	gnl.screen = rdy_for_floodfill(gnl.screen, map_components);
	game_ctx = combine_structs(&map_components, &gnl);
	if (floodfilliterative(&game_ctx, map_components.player, gnl) == -1)
		return (perror("Error\nout of bounce"), 1);
	game_ctx.comps->mlx_ptr = mlx_init();
	if (!game_ctx.comps->mlx_ptr)
		return (fprintf(stderr, "Error\nmlx_init() failed.\n"), 1);
	load_assets(&game_ctx);
	game_ctx.comps->win_ptr = mlx_new_window
		(game_ctx.comps->mlx_ptr, gnl.cols * 64, gnl.rows * 64, "so_long");
	if (!game_ctx.comps->win_ptr)
		return (fprintf(stderr, "Error\nmlx_new_window() failed.\n")
			, cleanup_and_exit(&game_ctx), 1);
	mlx_key_hook(game_ctx.comps->win_ptr, handle_keypress, &game_ctx);
	mlx_hook(game_ctx.comps->win_ptr, 17, 0, cleanup_and_exit, &game_ctx);
	render_game_map(&map_components, &gnl);
	mlx_loop(game_ctx.comps->mlx_ptr);
	return (cleanup_and_exit(&game_ctx), 0);
}
