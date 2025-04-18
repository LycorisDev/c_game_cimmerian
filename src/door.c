#include "cimmerian.h"

static void	close_last_door(t_man *man, t_map *map, t_list **opened_doors,
				int max_dist);
static void	open_new_door(t_man *man, t_list **opened_doors, int max_dist);
static int	is_closed_door(t_man *man, int x, int y);
static void	add_door_to_list(t_man *man, int x, int y, t_list **list);

void	door_routine(t_man *man)
{
	static t_list	*opened_doors;

	close_last_door(man, man->maps[man->curr_map], &opened_doors, 2);
	open_new_door(man, &opened_doors, 2);
	return ;
}

static void	close_last_door(t_man *man, t_map *map, t_list **opened_doors,
	int max_dist)
{
	t_list	*head;
	t_list	*curr;
	t_ivec2	player;
	t_ivec2	door;

	set_ivec2(&player, (int)man->player.pos.x, (int)man->player.pos.y);
	curr = *opened_doors;
	head = curr;
	while (curr)
	{
		door = *((t_ivec2 *)curr->data);
		if (!is_dist_grid_aligned_and_within_threshold(player, door, max_dist))
		{
			map->cells[door.y][door.x].is_visible = 1;
			map->cells[door.y][door.x].is_obstacle = 1;
			if (curr == head)
				head = (curr)->next;
			list_del_one(&curr, free);
		}
		else
			curr = curr->next;
	}
	*opened_doors = head;
	return ;
}

static void	open_new_door(t_man *man, t_list **opened_doors, int max_dist)
{
	int		i;
	t_ivec2	player;

	set_ivec2(&player, (int)man->player.pos.x, (int)man->player.pos.y);
	i = -max_dist;
	while (i <= max_dist)
	{
		if (is_closed_door(man, player.x + i, player.y))
		{
			add_door_to_list(man, player.x + i, player.y, opened_doors);
			break ;
		}
		else if (is_closed_door(man, player.x, player.y + i))
		{
			add_door_to_list(man, player.x, player.y + i, opened_doors);
			break ;
		}
		++i;
	}
	return ;
}

static int	is_closed_door(t_man *man, int x, int y)
{
	t_map	*map;

	map = man->maps[man->curr_map];
	if (x < 0 || x >= map->size.x || y < 0 || y >= map->size.y)
		return (0);
	return (map->cells[y][x].is_door && map->cells[y][x].is_obstacle);
}

static void	add_door_to_list(t_man *man, int x, int y, t_list **list)
{
	t_map	*map;
	t_ivec2	*coord;
	t_list	*node;

	map = man->maps[man->curr_map];
	coord = calloc(1, sizeof(t_ivec2));
	if (coord)
	{
		set_ivec2(coord, x, y);
		node = list_new(coord);
		if (!node)
			free(coord);
		else
			list_add_back(list, node);
	}
	map->cells[y][x].is_visible = 0;
	map->cells[y][x].is_obstacle = 0;
	return ;
}
