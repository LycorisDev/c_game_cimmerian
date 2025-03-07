#include "cimmerian.h"

#define MAP_SIZE_X 24
#define MAP_SIZE_Y 24
#define PLAYER_POS_X 22
#define PLAYER_POS_Y 12

static t_map	*create_map(void);
static void		free_map(t_map **map);
static t_vec2	get_cardinal_dir(char c);

static int		map_walls[] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,2,5,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,2,5,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,0,0,0,6,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

static int		map_buildings[] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

int	initialize_maps(void)
{
	g_man.map = create_map();
	return (!!g_man.map);
}

void	free_maps(void)
{
	free_map(&g_man.map);
	return ;
}

static t_map	*create_map(void)
{
	int		i;
	t_map	*map;

	map = calloc(1, sizeof(t_map));
	if (!map)
		return (0);
	map->size.x = MAP_SIZE_X;
	map->size.y = MAP_SIZE_Y;
	map->start_pos.x = PLAYER_POS_X;
	map->start_pos.y = PLAYER_POS_Y;
	map->start_dir = get_cardinal_dir('N');
	map->dof = 0;
	map->fog_width = get_fog_width(map->dof);
	map->fog_color = get_color_rgba(17, 15, 35, 255);
	set_ivec2(&map->minimap_offset, 563, 7);
	set_ivec2(&map->minimap_center, 599, 43);
	map->minimap_radius = 36;
	map->minimap_zoom = 9;
	map->minimap_cell_amount = map->minimap_radius / map->minimap_zoom * 2;
	compose_skybox(map, get_image("src_skybox"), map->fog_color);
	if (!map->skybox)
	{
		free_map(&map);
		return (0);
	}
	compose_background(map);
	if (!map->background)
	{
		free_map(&map);
		return (0);
	}
	map->cells = malloc(map->size.x * map->size.y * sizeof(t_cell));
	if (!map->cells)
	{
		free_map(&map);
		return (0);
	}
	t_img *img;
	i = 0;
	while (i < map->size.x * map->size.y)
	{
		map->cells[i].is_obstacle = map_walls[i] > 0;
		map->cells[i].is_door = map_walls[i] == 5;
		map->cells[i].is_goal = map_walls[i] == 6;
		map->cells[i].is_indoors = map_buildings[i];
		map->cells[i].height = !map->cells[i].is_obstacle ? 0 : 1.0;
		if (map_walls[i] == 6)
			map->cells[i].height = 3.0;
		map->cells[i].tex_floor = map_buildings[i] ? get_image("tex_floor_indoors") : get_image("tex_floor");
		map->cells[i].tex_ceiling = map_buildings[i] ? get_image("tex_ceiling") : 0;
		img = 0;
		if (map_walls[i] == 1)
			img = get_image("tex_wall_01");
		else if (map_walls[i] == 2)
			img = get_image("tex_wall_02");
		else if (map_walls[i] == 3)
			img = get_image("tex_wall_03");
		else if (map_walls[i] == 4)
			img = get_image("tex_wall_04");
		else if (map_walls[i] == 5)
			img = get_image("tex_doors");
		else if (map_walls[i] == 6)
			img = get_image("solid_color");
		map->cells[i].tex_north = img;
		map->cells[i].tex_east = img;
		map->cells[i].tex_south = img;
		map->cells[i].tex_west = img;
		++i;
	}
	map->sprites = malloc(NBR_SPR * sizeof(t_spr));
	if (!map->sprites)
	{
		free_map(&map);
		return (0);
	}
	img = get_image("chalice");
	i = 0;
	while (i < 8)
	{
		map->sprites[i].img = img;
		++i;
	}
	set_vec2(&map->sprites[0].pos,  20.5, 11.5);
	set_vec2(&map->sprites[1].pos,  18.5,  4.5);
	set_vec2(&map->sprites[2].pos,  10.0,  4.5);
	set_vec2(&map->sprites[3].pos,  10.0, 12.5);
	set_vec2(&map->sprites[4].pos,   3.5,  6.5);
	set_vec2(&map->sprites[5].pos,   3.5, 20.5);
	set_vec2(&map->sprites[6].pos,   3.5, 14.5);
	set_vec2(&map->sprites[7].pos,  14.5, 20.5);
	img = get_image("pillar");
	while (i < 12)
	{
		map->sprites[i].img = img;
		++i;
	}
	set_vec2(&map->sprites[8].pos,  18.5, 10.5);
	set_vec2(&map->sprites[9].pos,  18.5, 11.5);
	set_vec2(&map->sprites[10].pos, 18.5, 12.5);
	set_vec2(&map->sprites[11].pos,  8.5,  7.0);
	img = get_image("barrel");
	while (i < 20)
	{
		map->sprites[i].img = img;
		++i;
	}
	set_vec2(&map->sprites[12].pos, 21.5,  1.5);
	set_vec2(&map->sprites[13].pos, 15.5,  1.5);
	set_vec2(&map->sprites[14].pos, 16.0,  1.8);
	set_vec2(&map->sprites[15].pos, 16.2,  1.2);
	set_vec2(&map->sprites[16].pos,  3.5,  2.5);
	set_vec2(&map->sprites[17].pos,  9.5, 15.5);
	set_vec2(&map->sprites[18].pos, 10.0, 15.1);
	set_vec2(&map->sprites[19].pos, 10.5, 15.8);
	return (map);
}

static void	free_map(t_map **map)
{
	free((*map)->cells);
	free_image((*map)->skybox);
	free_png((*map)->background);
	free((*map)->sprites);
	free(*map);
	*map = 0;
	return ;
}

static t_vec2	get_cardinal_dir(char c)
{
	t_vec2	dir;

	dir.x = 0;
	dir.y = 0;
	if (c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	if (c == 'N')
		dir.y = -1;
	else if (c == 'S')
		dir.y = 1;
	else if (c == 'W')
		dir.x = -1;
	else if (c == 'E')
		dir.x = 1;
	return (dir);
}
