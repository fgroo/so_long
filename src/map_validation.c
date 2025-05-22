#include "../inc/so_long.h"

t_q	enqueue(t_point p, t_q q)
{
	if (q.itemCount < q.queue_size)
	{
		q.queue[q.rear] = p;
		q.rear = (q.rear + 1) % q.queue_size;
		q.itemCount++;
	}
	else
		printf("Warteschlange ist voll!\n");
	return (q);
}

t_q	dequeue(t_q q)
{
	q.p = (t_point){-1, -1}; // Ungültiger Punkt als Fehlerindikator
	if (q.itemCount > 0)
	{
		q.p = q.queue[q.front];
		q.front = (q.front + 1) % q.queue_size;
		q.itemCount--;
	}
	else
		printf("Warteschlange ist leer!\n");
	return (q);
}

int	isQueueEmpty(t_q q)
{
	return (q.itemCount == 0);
}

// Funktion zum Ausgeben des Arrays (zur Visualisierung)
void	printScreenIter(char **screen, t_map map)
{
	for (int i = 0; i < map.rows; i++) {
		for (int j = 0; j < map.cols; j++) {
			printf("%c ", screen[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// Iterative Floodfill-Funktion
void	floodfilliterative(char **screen, t_point player, t_map map)
{
	t_q	q;
	int	x;
	int	y;

	q = (t_q){map.rows * map.cols, 0, 0, 0, (t_point){0, 0}, 0};
	q.queue = malloc(sizeof(t_point) * (size_t)(map.rows * map.cols) + 1);
	q = enqueue((t_point){player.x, player.y}, q);
	while (!isQueueEmpty(q))
	{
		q = dequeue(q);
		x = q.p.x;
		y = q.p.y;
		if (x < 0 || x >= map.rows || y < 0 || y >= map.cols) // 1. Gültigkeitsprüfungen
			continue; // Nächstes Element aus der Warteschlange
		if (screen[x][y] != '0')
			continue ;
		screen[x][y] = '3'; // 2. Pixel füllen
		q = enqueue((t_point){x + 1, y}, q); // Nachbar unten
		q = enqueue((t_point){x - 1, y}, q); // Nachbar oben
		q = enqueue((t_point){x, y + 1}, q); // Nachbar rechts
		q = enqueue((t_point){x, y - 1}, q); // Nachbar links
	}
	free(q.queue);
}
