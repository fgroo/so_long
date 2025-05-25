#include "so_long.h"

char	**initialize_map(char **colsstring, int cols, int rows)
{
	int		i;
	int		j;
	char	**screen;

	i = -1;
	screen = (char **)malloc(sizeof(char *) * (((size_t)rows) + 1));
	if (!screen)
		return (perror("Error\nmalloc fail"), NULL);
	screen[rows] = NULL;
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

t_map	gnl_engine(int ac, char	*av)
{
	int		fd;
	t_map	gnl;

	if (ac != 2 || open(av, O_RDONLY) == -1)
		return (perror("Error\nbad input"), exit(1), (t_map){0});
	fd = open(av, O_RDONLY);
	gnl = (t_map){0};
	gnl.string[0] = get_next_line(fd);
	gnl.cols = ft_strlen_mod(gnl.string[gnl.rows]);
	if (!gnl.string[0] || gnl.cols == 0 || gnl.cols > 1921)
		return (perror("Error\ninvalid input"), (t_map){0});
	while (++gnl.rows < 1000 && ft_strlen_mod(gnl.string[gnl.rows]) < gnl.cols)
	{
		gnl.string[gnl.rows] = get_next_line(fd);
		if (!ft_strlen_mod(gnl.string[gnl.rows])
			|| ft_strlen_mod(gnl.string[gnl.rows]) > gnl.cols)
			return (close(fd), perror("Error\n"), combine_structs
				((t_comps *){0}, &gnl), exit (1), (t_map){0});
		if (ft_strlen_mod(gnl.string[gnl.rows]) < gnl.cols - 1)
			return (close(fd), perror("Error\n"), combine_structs
				((t_comps *){0}, &gnl), exit (1), (t_map){0});
		else if (!gnl.string[gnl.rows][gnl.cols - 1])
			break ;
	}
	return (close(fd), ++gnl.rows, --gnl.cols, gnl);
}

char	**rdy_for_floodfill(char **screen, t_comps map_components)
{
	if (!*screen)
		return (map_components.error_flag = 1, screen);
	screen[map_components.player.x][map_components.player.y] = '0';
	screen[map_components.exit.x][map_components.exit.y] = '0';
	screen[map_components.collectible.x][map_components.collectible.y] = '0';
	return (screen);
}
