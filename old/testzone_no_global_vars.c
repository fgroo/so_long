#include "../gnlsimple/get_next_line.h"
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
} t_map;

typedef struct s_q
{
	int queue_size;
	t_point *queue;
	t_point p;
	int front;
	int rear;
	int itemCount;
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
} t_comps;

t_q enqueue(t_point p, t_q q)
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

int isQueueEmpty(t_q q)
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
	t_q q;
	t_point startPoint;
	t_q current;
	int x;
	int y;

	q = (t_q){map.rows * map.cols};
	q.queue = malloc(sizeof(t_point) * (map.rows * map.cols) + 1);
	q = enqueue((t_point){player.x, player.y}, q);
	while (!isQueueEmpty(q))
	{
		q = dequeue(q);
		x = q.p.x;
		y = q.p.y;
		if (x < 0 || x >= map.rows || y < 0 || y >= map.cols) // 1. Gültigkeitsprüfungen
			continue; // Nächstes Element aus der Warteschlange
		if (screen[x][y] != '0')
			continue;
		screen[x][y] = '3';	// 2. Pixel füllen
		q = enqueue((t_point){x + 1, y}, q); // Nachbar unten
		q = enqueue((t_point){x - 1, y}, q); // Nachbar oben
		q = enqueue((t_point){x, y + 1}, q); // Nachbar rechts
		q = enqueue((t_point){x, y - 1}, q); // Nachbar links
	}
}

char	**initialize_map(char	**colsstring, int	colslen, int rowslen)
{
	int		i;
	int		j;
	char	**screen;

	i = -1;
	screen = malloc(sizeof(char *) * rowslen + 1);
	while (++i <= rowslen)
	{
		j = -1;
		screen[i] = malloc(colslen + 1);
		screen[i][colslen] = 0;
		while (++j < colslen)
				screen[i][j] = colsstring[i][j];
	}
	return (screen);
}
t_point	insert_coordinates(int i, int j)
{
	t_point	new;

	return (new.x = i, new.y = j, new);
}

t_comps	save_map_components(char	**testscreen, int colslen, int rows)
{
	t_comps	lst;

	lst = (t_comps){0};
	lst.i = -1;
	while (++lst.i <= rows)
	{
		lst.j = -1;
		while (++lst.j < colslen && !lst.error_flag)
		{
			if (testscreen[lst.i][lst.j] == 'C' && !lst.check_c++)
				lst.collectible = insert_coordinates(lst.i, lst.j);
			else if (testscreen[lst.i][lst.j] == 'P' && !lst.check_p++)
				lst.player = insert_coordinates(lst.i, lst.j);
			else if (testscreen[lst.i][lst.j] == 'E' && !lst.check_e++)
				lst.exit = insert_coordinates(lst.i, lst.j);
			else if (testscreen[lst.i][lst.j] == '0'
				|| testscreen[lst.i][lst.j] == '1')
				continue;
			else
				lst.error_flag = 1;
		}
	}
	if (!lst.error_flag && (!lst.check_p || !lst.check_e))
		lst.error_flag = 1; // Setze Fehler, wenn P oder E fehlen
	return (lst);
}

t_map gnl_engine(void)
{
	int		fd;
	t_map gnl;
	fd = open("readfile", O_RDONLY);
	gnl = (t_map){0};

	gnl.colsstring[0] = get_next_line(fd);
	gnl.cols = ft_strlen_mod(gnl.colsstring[gnl.rows]);
	if (gnl.cols == -1 || gnl.cols > 1921)
		return (perror("invalid input"), (t_map){0});
	while (++gnl.rows <= 1000)
	{
		gnl.colsstring[gnl.rows] = get_next_line(fd);
		if (ft_strlen_mod(gnl.colsstring[gnl.rows]) == -1)
			return (perror("invalid input"), (t_map){0});
		if (!gnl.colsstring[gnl.rows]
			|| !gnl.colsstring[gnl.rows][gnl.cols - 1])
			break ;
	}
	return (gnl);
}

char	**rdy_for_floodfill(char **screen, t_comps map_components)
{
	screen[map_components.player.x][map_components.player.y] = '0';
	screen[map_components.exit.x][map_components.exit.y] = '0';
	screen[map_components.collectible.x][map_components.collectible.y] = '0';
	return (screen);
}

int main(void)
{
	t_map	 gnl;
	char	**screen;
	t_comps map_components;

	gnl = gnl_engine();
	printf("Bild vor iterativem Floodfill:\n");
	
	screen = initialize_map(gnl.colsstring, --gnl.cols, gnl.rows);
	map_components = save_map_components(screen, gnl.cols, gnl.rows++);
	printScreenIter(screen, gnl);
    if (map_components.player.x < 0 || map_components.player.x >= gnl.rows
		|| map_components.player.y < 0 || map_components.player.y >= gnl.cols)
		return (printf("Startpunkt außerhalb der Grenzen!\n"), 1);
    screen = rdy_for_floodfill(screen, map_components); // Wichtig: oldColor muss die Farbe des Startpixels sein!
	floodfilliterative(screen, map_components.player, gnl);
	printf("Bild nach iterativem Floodfill:\n");
	printScreenIter(screen, gnl);
	return (0);
}
	// printf("Bild vor iterativem Floodfill:\n");
	// printScreenIter(screen);
	// int screen[ROWS][COLS] = {
	// 	{1, 1, 1, 1, 0},
	// 	{1, 1, 0, 1, 0},
	// 	{1, 0, 0, 0, 1},
	// 	{0, 1, 1, 1, 1},
	// 	{0, 0, 0, 1, 0}
	// };
    // printf("Bild nach iterativem Floodfill:\n");
    // printScreenIter(screen);