/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_and_cleaning.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgorlich <fgorlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:59:37 by fgorlich          #+#    #+#             */
/*   Updated: 2025/05/26 16:59:38 by fgorlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	if_del(t_game_ctx *ctx)
{
	if (ctx->comps->img_player)
		mlx_destroy_image(ctx->comps->mlx_ptr, ctx->comps->img_player);
	if (ctx->comps->img_wall)
		mlx_destroy_image(ctx->comps->mlx_ptr, ctx->comps->img_wall);
	if (ctx->comps->img_path)
		mlx_destroy_image(ctx->comps->mlx_ptr, ctx->comps->img_path);
	if (ctx->comps->img_collectible)
		mlx_destroy_image(ctx->comps->mlx_ptr, ctx->comps->img_collectible);
	if (ctx->comps->img_exit_open)
		mlx_destroy_image(ctx->comps->mlx_ptr, ctx->comps->img_exit_open);
	if (ctx->comps->img_exit_closed)
		mlx_destroy_image(ctx->comps->mlx_ptr, ctx->comps->img_exit_closed);
	if (ctx->comps->win_ptr)
		mlx_destroy_window(ctx->comps->mlx_ptr, ctx->comps->win_ptr);
	mlx_destroy_display(ctx->comps->mlx_ptr);
	free(ctx->comps->mlx_ptr);
}

int	cleanup_and_exit(t_game_ctx *ctx)
{
	if (!ctx->comps)
	{
		while (ctx->map_data->rows
			&& ctx->map_data->string[--ctx->map_data->rows])
			free(ctx->map_data->string[ctx->map_data->rows]);
		return (0);
	}
	if (!ctx->comps->mlx_ptr && !ctx->comps->error_flag)
		return (1);
	if (ctx->comps->mlx_ptr)
		if_del(ctx);
	if (ctx->map_data->screen)
	{
		while (ctx->map_data->rows
			&& ctx->map_data->screen[--ctx->map_data->rows])
			(free(ctx->map_data->screen[ctx->map_data->rows]),
				free(ctx->map_data->string[ctx->map_data->rows]));
		free(ctx->map_data->screen);
	}
	printf("Game exited.\n");
	exit(0);
}

void	load_assets(t_game_ctx *game)
{
	int	img_w;
	int	img_h;

	update_chars(game);
	game->comps->img_path = mlx_xpm_file_to_image
		(game->comps->mlx_ptr, "./textures/path_b.xpm", &img_w, &img_h);
	game->comps->img_wall = mlx_xpm_file_to_image
		(game->comps->mlx_ptr, "./textures/wall_b.xpm", &img_w, &img_h);
	game->comps->img_player = mlx_xpm_file_to_image
		(game->comps->mlx_ptr, "./textures/player_b.xpm", &img_w, &img_h);
	game->comps->img_collectible = mlx_xpm_file_to_image
		(game->comps->mlx_ptr, "./textures/collectible_b.xpm", &img_w, &img_h);
	game->comps->img_exit_open = mlx_xpm_file_to_image
		(game->comps->mlx_ptr, "./textures/exit_open_b.xpm", &img_w, &img_h);
	game->comps->img_exit_closed = mlx_xpm_file_to_image
		(game->comps->mlx_ptr, "./textures/exit_close_b.xpm", &img_w, &img_h);
	if (!game->comps->img_path || !game->comps->img_wall
		|| !game->comps->img_player || !game->comps->img_collectible
		|| !game->comps->img_exit_open
		|| !game->comps->img_exit_closed)
	{
		fprintf(stderr, "Error\nCould not load all game assets.\n");
		cleanup_and_exit(game);
	}
}

void	*if_img(char tile_type, t_comps *game)
{
	void	*current_img;

	if (tile_type == '1')
		current_img = game->img_wall;
	else if (tile_type == '3')
		current_img = game->img_path;
	else if (tile_type == 'C')
		current_img = game->img_collectible;
	else if (tile_type == 'E')
	{
		if (game->collectibles_found == 1)
			current_img = game->img_exit_open;
		else
			current_img = game->img_exit_closed;
	}
	else
		current_img = NULL;
	return (current_img);
}

t_game_ctx	combine_structs(t_comps *game, t_map *map)
{
	t_game_ctx	ctx;

	ctx.comps = game;
	ctx.map_data = map;
	if (!game || !*map->screen)
		cleanup_and_exit(&ctx);
	return (ctx);
}
