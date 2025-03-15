#include "cimmerian.h"

void	init_input_handling(t_man *man)
{
	glfwSetKeyCallback(man->window, physical_key_callback);
	glfwSetInputMode(man->window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetScrollCallback(man->window, scroll_callback);
	glfwSetMouseButtonCallback(man->window, mouse_callback);
	glfwSetCursorPosCallback(man->window, cursor_pos_callback);
	set_ivec2(&man->cursor, -1, -1);
	return ;
}

void	handle_input(t_man *man)
{
	glfwPollEvents();
	update_player_transform(man);
	return ;
}
