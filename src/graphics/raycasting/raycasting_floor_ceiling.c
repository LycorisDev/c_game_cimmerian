#include "cimmerian.h"

void	cast_floor_and_ceiling(t_frame *f, t_map *m)
{
	int	x;
	int	y;

	y = f->size.y / 2 + 1;
	while (y < f->size.y)
	{
		double rayDirX0 = g_man.player.dir.x - g_man.player.plane.x;
		double rayDirY0 = g_man.player.dir.y - g_man.player.plane.y;
		double rayDirX1 = g_man.player.dir.x + g_man.player.plane.x;
		double rayDirY1 = g_man.player.dir.y + g_man.player.plane.y;

		int p = y - f->size.y / 2 + 1;

		double posZ = 0.5 * g_man.res.h_mod * f->size.y;

		double rowDistance = posZ / p;

		if (rowDistance > m->dof)
		{
			++y;
			continue ;
		}

		double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / f->size.x;
		double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / f->size.x;

		double floorX = g_man.player.pos.x + rowDistance * rayDirX0;
		double floorY = g_man.player.pos.y + rowDistance * rayDirY0;

		x = 0;
		while (x < f->size.x)
		{
			// Stop rendering if outside the map
			if (!(floorX < 0 || floorX > m->size.x || floorY < 0 || floorY > m->size.y))
			{
				int cellX = (int)floorX;
				int cellY = (int)floorY;

				// floor
				t_img *tex = m->cells[cellY * m->size.x + cellX].tex_floor;
				int tx;
				int ty;
				t_color color;
				if (tex)
				{
					tx = (int)(tex->size.x * (floorX - cellX)) % tex->size.x;
					ty = (int)(tex->size.y * (floorY - cellY)) % tex->size.y;
					color = ((t_color *)tex->buf)[tex->size.x * ty + tx];
					color.r /= 2;
					color.g /= 2;
					color.b /= 2;
					apply_wall_fog(&color, m->fog_color, rowDistance, m->dof);
					draw_point(f, color, x, y);
				}

				// ceiling
				tex = m->cells[cellY * m->size.x + cellX].tex_ceiling;
				if (tex)
				{
					tx = (int)(tex->size.x * (floorX - cellX)) % tex->size.x;
					ty = (int)(tex->size.y * (floorY - cellY)) % tex->size.y;
					color = ((t_color *)tex->buf)[tex->size.x * ty + tx];
					color.r /= 2;
					color.g /= 2;
					color.b /= 2;
					apply_wall_fog(&color, m->fog_color, rowDistance, m->dof);
					draw_point(f, color, x, f->size.y - y - 1);
				}
			}

			floorX += floorStepX;
			floorY += floorStepY;
			++x;
		}
		++y;
	}
	return ;
}
