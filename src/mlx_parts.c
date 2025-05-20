#include "so_long.h"

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