#include "cimmerian.h"

static void		parse_segments(t_img *file, char **lines, size_t *i);

int	set_img_file_obj(t_img *file, char **lines, size_t *i)
{
	while (lines[*i])
	{
		if (is_field(lines[*i], "path"))
		{
			file->path = get_string_value(lines[*i]);
			//
			free(file->path);
			file->path = 0;
			//
		}
		else if (is_field(lines[*i], "path_shadow"))
		{
			file->path_shadow = get_string_value(lines[*i]);
			//
			free(file->path_shadow);
			file->path_shadow = 0;
			//
		}
		else if (is_field(lines[*i], "segments"))
			parse_segments(file, lines, i);
		else if (strchr(lines[*i], '"'))
			break ;
		++*i;
	}
	
	return (!!lines[*i]);
}

static void	parse_segments(t_img *file, char **lines, size_t *i)
{
	size_t	start;
	size_t	end;

	// We have the start from `"segments": [` to the closing "]".
	start = *i;
	while (lines[*i])
	{
		++*i;
		if (lines[*i - 1][0] == '}' && lines[*i] && lines[*i][0] == ']')
			break ;
	}
	if (!lines[*i])
		--*i;
	end = *i;

	/*
            {
                "id": "cursor",
                "size": [6, 6],
                "shadow_offset": [0, 0],
                "still_frame": 0,
                "cycle_time_in_ms": 150,
                "cycle_len": 2,
                "cycle": [
                    [0, 0],
                    [7, 0]
                ]
            }
	*/

	// Count the segments (file->segment_len)

	// Allocate the seg array
	// seg->seg = calloc(file->segment_len, sizeof(t_img_seg));

	// Populate the seg array
	return ;
}
