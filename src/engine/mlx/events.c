#include "cimmerian.h"

//TODO: Analyse this whole file

# define BTN_CLICK_LEFT  1
# define BTN_CLICK_WHEEL 2
# define BTN_CLICK_RIGHT 3
# define BTN_SCROLL_UP   4
# define BTN_SCROLL_DOWN 5
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ENTER  65293
# define KEY_ESCAPE 65307
# define KEY_LEFT   65361
# define KEY_UP     65362
# define KEY_RIGHT  65363
# define KEY_DOWN   65364
# define KEY_F11    65480

int	on_close(t_man *man)
{
	deinit(man);
	exit(0);
	return (0);
}

int	on_mouse_button(int button, int x, int y, t_man *man)
{
	if (button == BTN_CLICK_LEFT)
	{
	}
	else if (button == BTN_SCROLL_UP)
	{
	}
	else if (button == BTN_SCROLL_DOWN)
	{
	}
	return (0);
}

/*
	else if (keycode >= KEY_A && keycode <= (KEY_A + 25))
	printf("%c (%d)\n", keycode - KEY_A + 'A', keycode);
*/
int	on_key_press(int keycode, t_man *man)
{
	t_ivec2	move;

	if (keycode == KEY_ESCAPE)
		return (on_close(man));
	else if (keycode == KEY_F11)
	{
		//on_resize(man);
	}
	else
	{
		set_ivec2(&move, 0, 0);
		if (keycode == KEY_W || keycode == KEY_UP)
			move.y = -1;
		else if (keycode == KEY_A || keycode == KEY_LEFT)
			move.x = -1;
		else if (keycode == KEY_S || keycode == KEY_DOWN)
			move.y = 1;
		else if (keycode == KEY_D || keycode == KEY_RIGHT)
			move.x = 1;
		if (!man->click_pos.x && !man->click_pos.y)
			move_player(man, move);
	}
	return (0);
}

/*
static void	on_resize(t_man *man)
{
	t_ivec2	size;
	int		thickness;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	thickness = size.x / RES_WIDTH;
	if (thickness > 1)
	{
		--thickness;
		set_ivec2(&size, RES_WIDTH * thickness, RES_HEIGHT * thickness);
	}
	if (man->size.x == size.x)
		set_ivec2(&size, RES_WIDTH, RES_HEIGHT);
	mlx_destroy_window(man->mlx, man->window);
	mlx_destroy_image(man->mlx, man->frame[0].img);
	mlx_destroy_image(man->mlx, man->frame[1].img);
	mlx_destroy_image(man->mlx, man->frame[2].img);
	if (!create_window(man, size.x, size.y))
	{
		deinit(man);
		exit(1);
	}
	return ;
}
*/
