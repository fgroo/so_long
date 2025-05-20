// #include <stdlib.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <fcntl.h>


// int	main(void)
// {
// 	int	fd, i = 0, j, o = 0, linelen = 0, size = 0;
// 	char buf;

// 	fd = open("readfile", O_RDONLY);
// 	while (read(fd, &buf, 1) > 0)
// 	{
// 		if (buf == '\n' && linelen && i != linelen + 1)
// 			return (perror("wrong format"), 0);
// 		if (i == 0 && buf != '1' 
// 			|| i == linelen && buf != '1')
// 			return (printf("Zz%czZ", buf), perror("missing Wall"), 0);
// 		if (buf != '0' && buf != '1' && buf != 'C' &&
// 			buf != 'E' && buf != 'P' && buf != 'X' && buf != '\n')
// 			return (perror("invalid input in mapfile"), 0);
// 		if (buf == '\n' && !linelen)
// 			linelen = i - 1;
// 		if (buf == '\n' && linelen)
// 		{
// 			size++;
// 			i = -1;
// 		}
// 		i++;
// 		printf("%c", buf);
// 	}
// }


#include "./gnlsimple/get_next_line.h"
#include <stdio.h>
#include <stdlib.h> // Für malloc und free, falls eine dynamische Warteschlange benötigt wird
#include <fcntl.h>

#define ROWS 5
#define COLS 5
#define QUEUE_SIZE (ROWS * COLS) // Maximale Größe der Warteschlange





// Struktur für einen Punkt (Pixelkoordinaten)
typedef struct
{
	int x;
	int y;
} Point;

typedef struct
{
	int		rows;
	int		cols;
	char	*colsstring[1000];
} gnl_map;


typedef struct 
{
	Point	exit;
	Point	player;
	Point	collectible;
	int		error_flag;
	int		check_e;
	int		check_p;
	int		check_c;
	int		i;
	int		j;
} components;

// Einfache Warteschlangenimplementierung
Point queue[QUEUE_SIZE];
int front = 0;
int rear = 0;
int itemCount = 0;

void enqueue(Point p)
{
	if (itemCount < QUEUE_SIZE)
	{
		queue[rear] = p;
		rear = (rear + 1) % QUEUE_SIZE;
		itemCount++;
	}
	else
		printf("Warteschlange ist voll!\n");
}

Point	dequeue(void)
{
	Point	p;

	p = (Point){-1, -1}; // Ungültiger Punkt als Fehlerindikator
	if (itemCount > 0)
	{
		p = queue[front];
		front = (front + 1) % QUEUE_SIZE;
		itemCount--;
	}
	else
		printf("Warteschlange ist leer!\n");
	return (p);
}

int isQueueEmpty()
{
	return (itemCount == 0);
}

// Funktion zum Ausgeben des Arrays (zur Visualisierung)
void	printScreenIter(char **screen, gnl_map map)
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
void	floodFillIterative(char **screen, Point player, gnl_map map)
{
	Point startPoint;
	Point current;
	int x;
	int y;

	startPoint = (Point){player.x, player.y};
	enqueue(startPoint);
	while (!isQueueEmpty())
	{
		current = dequeue();
		x = current.x;
		y = current.y;
		if (x < 0 || x >= map.rows || y < 0 || y >= map.cols) // 1. Gültigkeitsprüfungen
			continue; // Nächstes Element aus der Warteschlange
		if (screen[x][y] != '0')
			continue;
		screen[x][y] = '3';	// 2. Pixel füllen
		enqueue((Point){x + 1, y}); // Nachbar unten
		enqueue((Point){x - 1, y}); // Nachbar oben
		enqueue((Point){x, y + 1}); // Nachbar rechts
		enqueue((Point){x, y - 1}); // Nachbar links
	}
}

char	**initialize_map(char	**colsstring, int	colslen, int rowslen)
{
	int		i;
	int		j;
	char	**testscreen;

	i = -1;
	testscreen = malloc(sizeof(char *) * rowslen + 1);
	while (++i <= rowslen)
	{
		j = -1;
		testscreen[i] = malloc(colslen + 1);
		testscreen[i][colslen] = 0;
		while (++j < colslen)
				testscreen[i][j] = colsstring[i][j];
	}
	return (testscreen);
}
Point	insert_coordinates(int i, int j)
{
	Point	new;

	return (new.x = i, new.y = j, new);
}

components	save_map_components(char	**testscreen, int colslen, int rows)
{
	components	lst;

	lst = (components){0};
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

gnl_map gnl_engine(void)
{
	int		fd;
	gnl_map gnl;
	fd = open("readfile", O_RDONLY);
	gnl = (gnl_map){0};

	gnl.colsstring[0] = get_next_line(fd);
	gnl.cols = ft_strlen_mod(gnl.colsstring[gnl.rows]);
	if (gnl.cols == -1 || gnl.cols > 1921)
		return (perror("invalid input"), (gnl_map){0});
	while (++gnl.rows <= 1000)
	{
		gnl.colsstring[gnl.rows] = get_next_line(fd);
		if (ft_strlen_mod(gnl.colsstring[gnl.rows]) == -1)
			return (perror("invalid input"), (gnl_map){0});
		if (!gnl.colsstring[gnl.rows]
			|| !gnl.colsstring[gnl.rows][gnl.cols - 1])
			break ;
	}
	return (gnl);
}

int main(void)
{
	gnl_map gnl;
	char	**screen;
	components map_components;

	gnl = gnl_engine();
	printf("Bild vor iterativem Floodfill:\n");
	
	screen = initialize_map(gnl.colsstring, --gnl.cols, gnl.rows);
	map_components = save_map_components(screen, gnl.cols, gnl.rows);
	printScreenIter(screen, gnl);
    if (map_components.player.x < 0 || map_components.player.x >= gnl.rows
		|| map_components.player.y < 0 || map_components.player.y >= gnl.cols)
		return (printf("Startpunkt außerhalb der Grenzen!\n"), 1);
    screen[map_components.player.x][map_components.player.y] = '0'; // Wichtig: oldColor muss die Farbe des Startpixels sein!
	floodFillIterative(screen, map_components.player, gnl);
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