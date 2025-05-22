/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nix <nix@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:18:48 by nix               #+#    #+#             */
/*   Updated: 2025/05/21 19:11:25 by nix              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minilibx-linux/mlx.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


typedef struct s_list
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img;
	int		height;
	int		width;
	int		i;
	int		linelen;
	int		size;
	char	buf;
	int		wall1;
	int		wall2;
	int		extras;
	char	**map;
}	t_list;

int generating_sprite(t_list *lst)
{
	
	switch (lst->buf)
	{
	case ('1'):
		if (!lst->wall2)
			lst->img = mlx_xpm_file_to_image(lst->mlx_ptr,
				"Walldown.xpm", &lst->width, &lst->height);
		else if (lst->linelen && lst->wall2 == lst->linelen)
			lst->img = mlx_xpm_file_to_image(lst->mlx_ptr,
				"Wall.xpm", &lst->width, &lst->height);
		break;
	case ('0'):
		lst->img = mlx_xpm_file_to_image(lst->mlx_ptr, 
			"Floor.xpm", &lst->width, &lst->height);
	case ('C'):
	case ('E'):
	}
}


void	*read_map_size(t_list *lst, int fd)
{
	while (read(fd, &lst->buf, 1) > 0 && lst->buf != '0' && lst->buf != '1' && 
		lst->buf != 'C' && lst->buf != 'E' && lst->buf != 50 && lst->buf != 10)
	{
		if (lst->buf == '1' && !lst->linelen)
			lst->wall1++;
		if (lst->buf == '1' && lst->linelen && lst->i <= lst->linelen)
			lst->wall2++;
		else
			lst->wall2 = 0;
		if (lst->buf == '\n' && lst->linelen && lst->i != lst->linelen + 1)
			return (perror("wrong format"), NULL);
		if (lst->i == 0 && lst->buf != '1' 
			|| lst->i == lst->linelen && lst->buf != '1')
			return (printf("Zz%czZ", lst->buf), perror("missing Wall"), NULL);
		if (lst->buf == '\n' && !lst->linelen)
			lst->linelen = lst->i - 1;
		if (lst->buf == '\n' && lst->linelen && lst->size++)
			lst->i = -1;
		if (generating_sprite(lst) == 0)
			return (perror("generating failed"), NULL);
		lst->i++;
	}
	return (0);
}

int	render_frame(t_list *lst)
{
	mlx_clear_window(lst->mlx_ptr, lst->win_ptr); // Fenster leeren
	mlx_put_image_to_window(lst->mlx_ptr, lst->win_ptr, lst->img, lst->width, lst->height); // Sprite neu zeichnen
	return (0);
}

void	Kill_Display(t_list *lst)
{
	mlx_destroy_display(lst->mlx_ptr);
	free(lst->mlx_ptr);
}

int	playermovement(int keysym, t_list *lst)
{
	if (keysym == 65307) // ESC Taste
	{
		mlx_destroy_window(lst->mlx_ptr, lst->win_ptr);
		exit(0);
	}
	if (keysym == 119) // W
		lst->height -= 50;
	if (keysym == 97)  // A
		lst->width -= 50;
	if (keysym == 115) // S
		lst->height += 50;
	if (keysym == 100) // D
		lst->width += 50;

	// Nach Tastendruck Szene neu zeichnen
	render_frame(lst);
	return (0);
}

int	main(void) // -I/opt/X11/include -Imlx -Lmlx -lmlx -L/usr/lib/X11 -lXext -lX11
{
	t_list	lst;

	lst.width = 400;
	lst.height = 500;
	//lst.map = m;

	lst.mlx_ptr = mlx_init();
	lst.win_ptr = mlx_new_window(lst.mlx_ptr, 2000 ,2000 ,"hier könnte ihre Werbung stehen");

	lst.img = mlx_xpm_file_to_image(lst.mlx_ptr, "Walls.xpm", &lst.width, &lst.height);
	render_frame(&lst);
	mlx_key_hook(lst.win_ptr, playermovement, &lst);
	mlx_loop(lst.mlx_ptr);
	return (0);
}

