#ifndef SO_LONG_H
# define SO_LONG_H

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
} t_comps;

t_q enqueue(t_point p, t_q q);
t_q	dequeue(t_q q);
int isQueueEmpty(t_q q);
void	printScreenIter(char **screen, t_map map);
void	floodFillIterative(char **screen, t_point player, t_map map);
char	**initialize_map(char	**colsstring, int	cols, int rows);
t_point	insert_coordinates(int i, int j);
t_comps	save_map_components(char	**testscreen, int cols, int rows);
t_map gnl_engine(void);
char	**rdy_for_floodfill(char **screen, t_comps map_components);

#endif