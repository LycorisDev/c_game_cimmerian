#include "cimmerian.h"

/*
	I had to remove `mlx_mouse_hide(man->mlx, man->window)` because it was 
	causing a leak.
*/
void	init_input_handling(t_man *man)
{
	mlx_hook(man->window, 17, 0L, on_close, man);
	mlx_hook(man->window, 4, 1L << 2, on_mouse_button, man);
	mlx_key_hook(man->window, on_key_press, man);
	mlx_loop_hook(man->mlx, game_loop, man);
	set_ivec2(&man->cursor, -1, -1);
	return ;
}

void	handle_input(t_man *man)
{
	//TODO
	/*
	MOVE CURSOR:
	mlx_mouse_get_pos(man->mlx, man->window, &pos.x, &pos.y);
	*/
	
	/* OPENGL:
	glfwPollEvents();
	update_player_transform(man);
	*/
	return ;
}
