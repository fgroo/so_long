#include "so_long.h"

t_q	enqueue(t_point p, t_q q)
{
	if (q.itemcount < q.queue_size)
	{
		q.queue[q.rear] = p;
		q.rear = (q.rear + 1) % q.queue_size;
		q.itemcount++;
	}
	else
		printf("Warteschlange ist voll!\n");
	return (q);
}

t_q	dequeue(t_q q, t_game_ctx *ctx)
{
	q.p = (t_point){-1, -1};
	if (q.itemcount > 0)
	{
		q.p = q.queue[q.front];
		q.front = (q.front + 1) % q.queue_size;
		q.itemcount--;
		if (ctx->comps->exit.x == q.p.x
			&& ctx->comps->exit.y == q.p.y)
			ctx->comps->exit_reachable = 1;
	}
	else
		printf("Warteschlange ist leer!\n");
	return (q);
}

void	printscreeniter(char **screen, t_map map)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (++i < map.rows)
	{
		while (++j < map.cols)
		{
			printf("%c ", screen[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int	floodfilliterative(t_game_ctx *ctx, t_point player, t_map map)
{
	t_q	q;

	q = (t_q){map.rows * map.cols, 0, 0, 0, (t_point){0, 0}, 0};
	q.queue = malloc(sizeof(t_point) * (size_t)(map.rows * map.cols) + 1);
	if (!q.queue || ctx->comps->error_flag)
		return (free(q.queue), cleanup_and_exit(ctx), -1);
	q = enqueue((t_point){player.x, player.y}, q);
	while ((q.itemcount != 0) && q.queue)
	{
		q = dequeue(q, ctx);
		if (map.screen[q.p.x][q.p.y] != '0')
			continue ;
		if (!q.p.x || q.p.x >= map.rows - 1 || !q.p.y || q.p.y >= map.cols - 1)
			return (free(q.queue), ++ctx->comps->error_flag,
				cleanup_and_exit(ctx), -1);
		map.screen[q.p.x][q.p.y] = '3';
		q = enqueue((t_point){q.p.x + 1, q.p.y}, q);
		q = enqueue((t_point){q.p.x - 1, q.p.y}, q);
		q = enqueue((t_point){q.p.x, q.p.y + 1}, q);
		q = enqueue((t_point){q.p.x, q.p.y - 1}, q);
	}
	free(q.queue);
	if (ctx->comps->exit_reachable)
		return (ctx->map_data->screen = map.screen, 0);
	return (++ctx->comps->error_flag, cleanup_and_exit(ctx), -1);
}

void	update_chars(t_game_ctx *ctx)
{
	ctx->map_data->screen[ctx->comps->player.x][ctx->comps->player.y] = 'P';
	ctx->map_data->screen[ctx->comps->collectible.x]
	[ctx->comps->collectible.y] = 'C';
	ctx->map_data->screen[ctx->comps->exit.x][ctx->comps->exit.y] = 'E';
}
