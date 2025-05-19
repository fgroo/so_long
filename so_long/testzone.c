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

void enqueue(Point p) {
    if (itemCount < QUEUE_SIZE) {
        queue[rear] = p;
        rear = (rear + 1) % QUEUE_SIZE;
        itemCount++;
    } else {
        printf("Warteschlange ist voll!\n");
    }
}

Point dequeue() {
    Point p = {-1, -1}; // Ungültiger Punkt als Fehlerindikator
    if (itemCount > 0) {
        p = queue[front];
        front = (front + 1) % QUEUE_SIZE;
        itemCount--;
    } else {
        printf("Warteschlange ist leer!\n");
    }
    return p;
}

int isQueueEmpty() {
    return itemCount == 0;
}

// Funktion zum Ausgeben des Arrays (zur Visualisierung)
void printScreenIter(int screen[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", screen[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Iterative Floodfill-Funktion
void floodFillIterative(int screen[ROWS][COLS], int startX, int startY, int oldColor, int newColor)
{
	if (oldColor == newColor) // Wenn alte und neue Farbe gleich sind, gibt es nichts zu tun
		return;
	
	Point startPoint = {startX, startY};

	enqueue(startPoint);
	while (!isQueueEmpty())
	{
		Point current = dequeue();

		int x = current.x;
		int y = current.y;

		if (x < 0 || x >= ROWS || y < 0 || y >= COLS) // 1. Gültigkeitsprüfungen
			continue; // Nächstes Element aus der Warteschlange
		if (screen[x][y] != oldColor)
			continue;
		screen[x][y] = newColor;	// 2. Pixel füllen
		Point next;	// 3. Nachbarn zur Warteschlange hinzufügen (4-Wege)
		next.x = x + 1; next.y = y; enqueue(next); // Unten
		next.x = x - 1; next.y = y; enqueue(next); // Oben
		next.x = x; next.y = y + 1; enqueue(next); // Rechts
		next.x = x; next.y = y - 1; enqueue(next); // Links
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

components	save_map_components(char	**testscreen, int colslen, int rowslen)
{
	components	lst;

	lst = (components){0};
	lst.i = -1;
	while (++lst.i <= rowslen)
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

int main(void)
{
	int screen[ROWS][COLS] = {
		{1, 1, 1, 1, 0},
		{1, 1, 0, 1, 0},
		{1, 0, 0, 0, 1},
		{0, 1, 1, 1, 1},
		{0, 0, 0, 1, 0}
	};
	int	fd = open("readfile", O_RDONLY);
	char	*colsstring[1000];
	int		colslen = 0;
	int		rowslen = 0;
	components map_components;

	colsstring[0] = get_next_line(fd);
	colslen = ft_strlen_mod(colsstring[rowslen]);
	if (colslen == -1 || colslen > 1921)
		return (perror("invalid input"), 1);
	while (++rowslen <= 1000)
	{
		colsstring[rowslen] = get_next_line(fd);
		if (ft_strlen_mod(colsstring[rowslen]) == -1)
			return (perror("invalid input"), 1);
		if (!colsstring[rowslen] || !colsstring[rowslen][colslen - 1])
			break ;
	}
	char	**testscreen;
	testscreen = initialize_map(colsstring, --colslen, rowslen);
	map_components = save_map_components(testscreen, colslen, rowslen);


    int startX_iter = 3;
    int startY_iter = 2;
    // Wichtig: oldColor muss die Farbe des Startpixels sein!
    if (startX_iter < 0 || startX_iter >= ROWS || startY_iter < 0 || startY_iter >= COLS) {
        printf("Startpunkt außerhalb der Grenzen!\n");
        return 1;
    }
    int oldColor_iter = screen[startX_iter][startY_iter];
    int newColor_iter = 3;

    printf("Bild vor iterativem Floodfill:\n");
    printScreenIter(screen);

    // Sicherstellen, dass der Startpunkt die zu füllende Farbe hat
    if (screen[startX_iter][startY_iter] == oldColor_iter) {
         floodFillIterative(screen, startX_iter, startY_iter, oldColor_iter, newColor_iter);
    } else {
        printf("Startpixel hat nicht die erwartete alte Farbe.\n");
    }

    printf("Bild nach iterativem Floodfill:\n");
    printScreenIter(screen);
    return (0);
}
  
