#ifndef MATH_EXTRA_H
# define MATH_EXTRA_H

/* # define PI_OVER_180 (M_PI / 180) */
/* # define PI_OVER_180_INVERSE (180 / M_PI) */
/* # define RAD_1 (M_PI * 2 / 360) */
/* # define RAD_45 (M_PI / 4) */
/* # define RAD_60 (M_PI / 3) */
/* # define RAD_90 (M_PI / 2) */
/* # define RAD_180 (M_PI) */
/* # define RAD_270 (M_PI / 2 * 3) */
/* # define RAD_360 (M_PI * 2) */
# define PI_OVER_180 0.017453293
# define PI_OVER_180_INVERSE 57.295779515
# define RAD_1 0.017453293
# define RAD_45 0.785398163
# define RAD_60 1.047197551
# define RAD_90 1.570796327
# define RAD_180 M_PI
# define RAD_270 4.71238898
# define RAD_360 6.283185307

typedef struct s_vec2
{
	float	x;
	float	y;
}	t_vec2;

typedef struct s_ivec2
{
	int	x;
	int	y;
}	t_ivec2;

typedef struct s_uivec2
{
	unsigned int	x;
	unsigned int	y;
}	t_uivec2;

/* Angles ------------------------------------------------------------------- */

float	deg2rad(float deg);
float	get_angle_from_dir(t_vec2 dir);
float	norm_angle(float angle);
float	dot(t_vec2 a, t_vec2 b);
t_vec2	rotate_vec2(t_vec2 v, float angle);

/* Clamp -------------------------------------------------------------------- */

int		clamp(int n, int min, int max);
float	fclamp(float n, float min, float max);
int		norm(int n);

/* Distance ----------------------------------------------------------------- */

float	dist(t_vec2 a, t_vec2 b);
int		is_dist_grid_aligned_and_within_threshold(t_ivec2 a, t_ivec2 b,
			int threshold);

/* Min-Max ------------------------------------------------------------------ */

int		imin(int a, int b);
int		imax(int a, int b);

/* Sqrt Approximation ------------------------------------------------------- */

float	sqrt_approx(float n);

/* Vectors ------------------------------------------------------------------ */

void	set_vec2(t_vec2 *v, float x, float y);
void	set_ivec2(t_ivec2 *v, int x, int y);
int		do_segments_intersect(t_vec2 start1, t_vec2 end1, t_vec2 start2,
			t_vec2 end2);

#endif
