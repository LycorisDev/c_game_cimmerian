#include "cimmerian.h"

static int		is_colliding(t_man *man, t_cell *cell, char opposite_cardinal);
static t_cell	*get_cell(t_map *m, int x, int y);
static int		is_in_available_cell(int collision[4]);

int	push_back_on_collision(t_man *man, t_map *m, t_vec2 pos, t_vec2 delta)
{
	int	collision[4];

	collision[0] = is_colliding(man, get_cell(m, pos.x,
				pos.y + man->player.radius), 'S') && delta.y > 0;
	if (collision[0])
		pos.y = floor(pos.y + man->player.radius) - man->player.radius;
	collision[1] = is_colliding(man, get_cell(m, pos.x,
				pos.y - man->player.radius), 'N') && delta.y < 0;
	if (collision[1])
		pos.y = ceil(pos.y - man->player.radius) + man->player.radius;
	collision[2] = is_colliding(man, get_cell(m, pos.x + man->player.radius,
				pos.y), 'E') && delta.x > 0;
	if (collision[2])
		pos.x = floor(pos.x + man->player.radius) - man->player.radius;
	collision[3] = is_colliding(man, get_cell(m, pos.x - man->player.radius,
				pos.y), 'W') && delta.x < 0;
	if (collision[3])
		pos.x = ceil(pos.x - man->player.radius) + man->player.radius;
	man->player.pos = pos;
	return (is_in_available_cell(collision));
}

static int	is_colliding(t_man *man, t_cell *cell, char opposite_cardinal)
{
	char	portal_face;

	portal_face = 0;
	if (!man->player.is_in_portal && cell && cell->portal)
		portal_face = cell->portal->src_cardinal;
	return (!cell || (cell->is_obstacle
			&& (!portal_face || portal_face == opposite_cardinal)
			&& (!man->player.is_in_portal || !cell->portal)));
}

static t_cell	*get_cell(t_map *m, int x, int y)
{
	if (x < 0 || y < 0 || x >= m->size.x || y >= m->size.y)
		return (0);
	return (&m->cells[y][x]);
}

static int	is_in_available_cell(int collision[4])
{
	int	i;
	int	nbr_collisions;

	i = 0;
	nbr_collisions = 0;
	while (i < 4)
	{
		nbr_collisions += collision[i];
		++i;
	}
	return (nbr_collisions < 3);
}
