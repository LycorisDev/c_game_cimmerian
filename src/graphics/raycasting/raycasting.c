#include "cimmerian.h"

static void	perform_dda(t_map *m, double cam_x, t_list **list);
static int	is_obstacle_see_through(t_map *m, t_ray *r);
static void	set_line(t_frame *f, t_map *m, int x, t_ray *r);

void	raycasting(t_frame *f, t_map *m)
{
	int		x;
	t_list	*list;
	double	*z_buffer;
	int		spr_order[NBR_SPR];
	double	spr_dist[NBR_SPR];

	cast_floor(f, m);

	z_buffer = malloc(f->size.x * sizeof(double));
	int i = 0;
	while (i < f->size.x)
		z_buffer[i++] = LLONG_MAX;

	sort_sprites(spr_order, spr_dist, NBR_SPR);

	list = 0;
	x = 0;
	while (x < f->size.x)
	{
		perform_dda(m, 2 * x / (double)f->size.x - 1, &list);
		while (list)
		{
			t_ray *ray = (t_ray *)list->data;
			if (is_obstacle_see_through(m, ray))
			{
				cast_ceiling_x(f, m, z_buffer, x);
				cast_sprites(f, m, z_buffer, spr_order, spr_dist, x);
			}
			if (ray->perp_wall_dist < z_buffer[x])
				z_buffer[x] = ray->perp_wall_dist;
			set_line(f, m, x, ray);
			draw_wall(f, m, ray);
			list_del_one(&list, free);
		}
		cast_ceiling_x(f, m, z_buffer, x);
		cast_sprites(f, m, z_buffer, spr_order, spr_dist, x);
		++x;
	}
	free(z_buffer);
	return ;
}

static void	perform_dda(t_map *m, double cam_x, t_list **list)
{
	t_vec2	delta_dist;
	t_ivec2	step;
	t_vec2	side_dist;
	t_ray	*r;
	double	biggest_height;

	r = calloc(1, sizeof(t_ray));
	if (!r)
		return ;
	r->ray_dir.x = g_man.player.dir.x + g_man.player.plane.x * cam_x;
	r->ray_dir.y = g_man.player.dir.y + g_man.player.plane.y * cam_x;
	r->m_index.x = (int)g_man.player.pos.x;
	r->m_index.y = (int)g_man.player.pos.y;
	delta_dist.x = (r->ray_dir.x == 0) ? 1e30 : abs_f(1 / r->ray_dir.x);
	delta_dist.y = (r->ray_dir.y == 0) ? 1e30 : abs_f(1 / r->ray_dir.y);
	if (r->ray_dir.x < 0)
	{
		step.x = -1;
		side_dist.x = (g_man.player.pos.x - r->m_index.x) * delta_dist.x;
	}
	else
	{
		step.x = 1;
		side_dist.x = (r->m_index.x + 1.0 - g_man.player.pos.x) * delta_dist.x;
	}
	if (r->ray_dir.y < 0)
	{
		step.y = -1;
		side_dist.y = (g_man.player.pos.y - r->m_index.y) * delta_dist.y;
	}
	else
	{
		step.y = 1;
		side_dist.y = (r->m_index.y + 1.0 - g_man.player.pos.y) * delta_dist.y;
	}
	biggest_height = 0;
	while (1)
	{
		if (side_dist.x < side_dist.y)
		{
			side_dist.x += delta_dist.x;
			r->m_index.x += step.x;
			r->side = 0;
		}
		else
		{
			side_dist.y += delta_dist.y;
			r->m_index.y += step.y;
			r->side = 1;
		}
		if (r->m_index.x < 0 || r->m_index.y < 0
			|| r->m_index.x >= m->size.x || r->m_index.y >= m->size.y)
			break ;
		else if (m->cells[r->m_index.y * m->size.x + r->m_index.x].is_obstacle)
		{
			t_cell	*cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
			int	add_to_list = 0;

			if (is_obstacle_see_through(m, r))
				add_to_list = 1;
			else if (cell->height > biggest_height)
			{
				biggest_height = cell->height;
				add_to_list = 1;
			}

			if (add_to_list)
			{
				if (r->side == 0)
					r->perp_wall_dist = (side_dist.x - delta_dist.x);
				else
					r->perp_wall_dist = (side_dist.y - delta_dist.y);
				if (r->perp_wall_dist > m->dof)
					break ;
				list_add_front(list, list_new(r));
				r = calloc(1, sizeof(t_ray));
				r->ray_dir = ((t_ray *)(*list)->data)->ray_dir;
				r->m_index = ((t_ray *)(*list)->data)->m_index;
				r->side = ((t_ray *)(*list)->data)->side;
			}
		}
	}
	free(r);
	return ;
}

static int	is_obstacle_see_through(t_map *m, t_ray *r)
{
	t_img	*tex;
	t_cell	*cell;

	tex = 0;
	cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
	if (r->side == 1 && r->ray_dir.y > 0)
		tex = cell->tex_north;
	else if (r->side == 1 && r->ray_dir.y < 0)
		tex = cell->tex_south;
	else if (r->side == 0 && r->ray_dir.x > 0)
		tex = cell->tex_west;
	else if (r->side == 0 && r->ray_dir.x < 0)
		tex = cell->tex_east;
	if (!tex)
		return (1);
	return (tex->is_see_through[tex->cycle_index]);
}

static void	set_line(t_frame *f, t_map *m, int x, t_ray *r)
{
	t_cell	*cell;
	int		offset;

	cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
	r->line_height_cubic = (int)(f->size.y / r->perp_wall_dist * g_man.res.h_mod);
	r->line_height = r->line_height_cubic * cell->height;
	offset = r->line_height_cubic * (1.0 - cell->height) * 0.5;
	r->unclamped_line_height.x = -r->line_height / 2 + f->size.y / 2 + offset;
	r->unclamped_line_height.y = r->line_height / 2 + f->size.y / 2 + offset;
	r->coord1.x = x;
	r->coord1.y = max(r->unclamped_line_height.x, 0);
	r->coord2.x = x;
	r->coord2.y = min(r->unclamped_line_height.y, f->size.y - 1);
	return ;
}
