#include "olafur.h"

int	set_png_file_obj(t_png *file, char **lines)
{
	static size_t	i;

	while (lines[i])
	{
		if (is_field(lines[i], "path"))
			file->path = get_string_value(lines[i]);
		else if (is_field(lines[i], "path_shadow"))
			file->path_shadow = get_string_value(lines[i]);
		else if (is_field(lines[i], "segments"))
			parse_segments(file, lines, &i);
		else if (lines[i][0] == '}')
		{
			++i;
			break ;
		}
		++i;
	}
	if (!lines[i])
	{
		i = 0;
		return (0);
	}
	return (!!lines[i]);
}

void	free_and_reset_png_file_obj(t_png *file)
{
	size_t	i;

	free(file->path);
	free(file->path_shadow);
	free(file->buf);
	free(file->buf_shadow);
	i = 0;
	while (i < file->segment_len)
	{
		free(file->seg[i].id);
		free(file->seg[i].cycle);
		++i;
	}
	free(file->seg);
	memset(file, 0, sizeof(t_png));
	return ;
}
