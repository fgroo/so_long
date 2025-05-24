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

t_q	dequeue(t_q q)
{
	q.p = (t_point){-1, -1};
	if (q.itemcount > 0)
	{
		q.p = q.queue[q.front];
		q.front = (q.front + 1) % q.queue_size;
		q.itemcount--;
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

void	floodfilliterative(char **screen, t_point player, t_map map)
{
	t_q	q;
	int	x;
	int	y;

	q = (t_q){map.rows * map.cols, 0, 0, 0, (t_point){0, 0}, 0};
	q.queue = malloc(sizeof(t_point) * (size_t)(map.rows * map.cols) + 1);
	q = enqueue((t_point){player.x, player.y}, q);
	while ((q.itemcount != 0) && q.queue)
	{
		q = dequeue(q);
		x = q.p.x;
		y = q.p.y;
		if (screen[x][y] != '0')
			continue ;
		if (!x || x >= map.rows - 1 || !y || y >= map.cols - 1)
			(free(q.queue), q.queue = NULL);
		screen[x][y] = '3';
		q = enqueue((t_point){x + 1, y}, q);
		q = enqueue((t_point){x - 1, y}, q);
		q = enqueue((t_point){x, y + 1}, q);
		q = enqueue((t_point){x, y - 1}, q);
	}
	free(q.queue);
}
