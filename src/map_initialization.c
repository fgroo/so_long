#include "so_long.h"

char	**initialize_map(char **colsstring, int cols, int rows)
{
	int		i;
	int		j;
	char	**screen;

	i = -1;
	screen = (char **)malloc(sizeof(char *) * (((size_t)rows) + 1));
	screen[rows] = NULL;
	if (!screen)
		return (perror("malloc fail"), NULL);
	while (++i < rows)
	{
		j = -1;
		screen[i] = malloc((size_t)cols + 1);
		if (!screen[i])
		{
			while (screen[--i])
				free(screen[i]);
			return (free(screen), NULL);
		}
		screen[i][cols] = 0;
		while (++j < cols)
			screen[i][j] = colsstring[i][j];
	}
	return (screen);
}

t_point	insert_coordinates(int i, int j)
{
	t_point	new;

	return (new.x = i, new.y = j, new);
}

t_comps	save_map_components(char **testscreen, int cols, int rows)
{
	t_comps	lst;

	lst = (t_comps){0};
	lst.i = -1;
	while (++lst.i < rows)
	{
		lst.j = -1;
		while (++lst.j < cols && !lst.error_flag)
		{
			if (testscreen[lst.i][lst.j] == 'C' && !lst.check_c++)
				lst.collectible = insert_coordinates(lst.i, lst.j);
			else if (testscreen[lst.i][lst.j] == 'P' && !lst.check_p++)
				lst.player = insert_coordinates(lst.i, lst.j);
			else if (testscreen[lst.i][lst.j] == 'E' && !lst.check_e++)
				lst.exit = insert_coordinates(lst.i, lst.j);
			else if (testscreen[lst.i][lst.j] == '0'
				|| testscreen[lst.i][lst.j] == '1')
				continue ;
			else
				lst.error_flag = 1;
		}
	}
	if (!lst.error_flag && (!lst.check_p || !lst.check_e))
		lst.error_flag = 1;
	return (lst);
}

t_map	gnl_engine(void)
{
	int		fd;
	t_map	gnl;

	fd = open("readfile", O_RDONLY);
	gnl = (t_map){0};
	gnl.colsstring[0] = get_next_line(fd);
	gnl.cols = ft_strlen_mod(gnl.colsstring[gnl.rows]);
	if (gnl.cols == -1 || gnl.cols > 1921)
		return (perror("invalid input"), (t_map){0});
	while (++gnl.rows <= 1000)
	{
		gnl.colsstring[gnl.rows] = get_next_line(fd);
		if (!ft_strlen_mod(gnl.colsstring[gnl.rows])
			|| ft_strlen_mod(gnl.colsstring[gnl.rows]) > gnl.cols)
			return (perror("invalid input"), (t_map){0});
		if (!gnl.colsstring[gnl.rows]
			|| !gnl.colsstring[gnl.rows][gnl.cols - 1])
			break ;
	}
	return (++gnl.rows, --gnl.cols, gnl);
}

char	**rdy_for_floodfill(char **screen, t_comps map_components)
{
	screen[map_components.player.x][map_components.player.y] = '0';
	screen[map_components.exit.x][map_components.exit.y] = '0';
	screen[map_components.collectible.x][map_components.collectible.y] = '0';
	return (screen);
}
