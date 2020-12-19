#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mlx.h"
#include "function_maths.h"

typedef struct    s_vars
{
    void          *mlx;
    void          *win;
}                 t_vars;

typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}
int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

int ft_parsing_scene(t_scene *scene)
{
    scene->h = 600;
    scene->fov = 60 *PI / 180;
    scene->w = 600;
    scene->light = malloc(sizeof(t_light));
    if (scene->light == 0)
        return(-1);
    scene->light->pos=malloc(sizeof(t_coord));
    if (scene->light->pos == 0)
    {
        free(scene->light);
        return(-1);
    }
    scene->ray = malloc(sizeof(t_ray));
    if (scene->ray == 0)
    {
        free(scene->light->pos);
        free(scene->light);
        return(-1);
    }
    scene->ray->origin = malloc(sizeof(t_coord));
    if (scene->ray->origin == 0)
    {
        free(scene->light->pos);
        free(scene->light);
        free(scene->ray);
        return(-1);
    }
    scene->ray->direction = malloc(sizeof(t_coord));
    if (scene->ray->direction == 0)
    {
        free(scene->light->pos);
        free(scene->light);
        free(scene->ray->origin);
        free(scene->ray);
        return(-1);
    }
    ft_coord(0,0, 150,scene->light->pos);
    scene->light->i = 1000;
    ft_coord(0, 0,0, scene->ray->origin);
    return(1);

}


int    ft_list_sphere(t_sphere **s)
{
    
    t_sphere *ptn;

    ptn = malloc(sizeof(t_sphere));

    ptn->rayon = 20;
    ptn->origin = malloc(sizeof(t_coord));
    ft_coord(0,0, 150,ptn->origin);
    ptn->mirror = 0;
    ptn->clear = 0;
    ptn->color= malloc(sizeof(t_coord));
    ft_coord(70,0,3,ptn->color);
    ptn->next = NULL;
    
     ptn->next = malloc(sizeof(t_sphere));
    ptn->next->rayon = 10;
    ptn->next->origin = malloc(sizeof(t_coord));
    ft_coord(0,-30, 130,ptn->next->origin);
    ptn->next->color= malloc(sizeof(t_coord));
    ft_coord(0.3,50,70,ptn->next->color);
    ptn->next->mirror = 0;
    ptn->next->next = NULL;
    /*


    ptn->next = malloc(sizeof(t_sphere));
    ptn->next->rayon = 35;
    ptn->next->origin = malloc(sizeof(t_coord));
    ft_coord(30,0, 200,ptn->next->origin);
    ptn->next->color= malloc(sizeof(t_coord));
    ft_coord(1,0,1,ptn->next->color);
    
    ptn->next->mirror =0;

    ptn->next = malloc(sizeof(t_sphere));
    ptn->next->mirror = 0;
    ptn->next->rayon = 2000;
    ptn->next->clear = 0;
    ptn->next->origin = malloc(sizeof(t_coord));
    ft_coord(0,-2000-50, 80,ptn->next->origin);
    ptn->next->color= malloc(sizeof(t_coord));
    ft_coord(0,0.2,0.7,ptn->next->color);
    ptn->next->next = NULL;
*/
    ptn->next->next= malloc(sizeof(t_sphere));
    ptn->next->next->mirror = 0;
    ptn->next->next->rayon = 100;
    ptn->next->next->clear = 0;
    ptn->next->next->origin = malloc(sizeof(t_coord));
    ft_coord(0,-150, 130,ptn->next->next->origin);
    ptn->next->next->color= malloc(sizeof(t_coord));
    ft_coord(60,156,0.1,ptn->next->next->color);
    ptn->next->next->next=NULL;
    /*
    ptn->next->next->next = malloc(sizeof(t_sphere));
    ptn->next->next->next->mirror = 0;
    ptn->next->next->next->multicolor = 1;
    ptn->next->next->next->clear = 0;
    ptn->next->next->next->rayon = 30;
    ptn->next->next->next->origin=malloc(sizeof(t_coord));
    ft_coord(0,-5,80,ptn->next->next->next->origin);
        ptn->next->next->next->color= malloc(sizeof(t_coord));
    ft_coord(0,1,0,ptn->next->next->next->color);
    ptn->next->next->next->next = NULL;


        ptn->next->next->next->next = malloc(sizeof(t_sphere));
    ptn->next->next->next->next->mirror = 0;
    ptn->next->next->next->next->clear = 0;
    ptn->next->next->next->next->rayon = 2000;
    ptn->next->next->next->next->origin=malloc(sizeof(t_coord));
    ft_coord(-2500,0,0,ptn->next->next->next->next->origin);
        ptn->next->next->next->next->color= malloc(sizeof(t_coord));
    ft_coord(0.1,0.1,0.2,ptn->next->next->next->next->color);
    ptn->next->next->next->next->next =NULL;*/
    *s = ptn;
    return(1);
}
void ft_initialize_img(t_vars *vars, t_data *img, t_scene *scene)
{
    vars->mlx = mlx_init();
    vars->win = mlx_new_window(vars->mlx, scene->h, scene->w, "Hello world!");
    img->img = mlx_new_image(vars->mlx, scene->h, scene->w);
    img->addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel), &(img->line_length), &(img->endian));
}

float ft_ombre(t_sphere **list, t_sphere *sphere, t_coord *pos,t_coord *normal, double dist, t_scene *scene)
{
        t_ray ray_reflect;
        t_sphere *tmp;
        double t_inter;

        ray_reflect.direction = malloc(sizeof(t_coord));
        ray_reflect.origin = malloc(sizeof(t_coord));
        ft_vectors_substract(pos,scene->light->pos, ray_reflect.direction);
        ft_normalize(ray_reflect.direction);
        tmp = *list;
        
        ft_vectors_mult(normal,0.001,ray_reflect.origin);
        ft_vectors_add(ray_reflect.origin,pos,ray_reflect.origin);
                    while (tmp != NULL)
                    {  
                        if ((ft_visibilite(tmp, &ray_reflect, &t_inter) == 1 )) //  intersection
                                    if (t_inter *t_inter *t_inter < dist)
                                           return(0);
                            tmp = tmp->next;
                    }
        free(ray_reflect.direction);
        return(1);
}

typedef struct s_palette{
    double r;
    double g;
    double b;
    double intensity;
}t_palette;


double ft_color_intensity(t_palette *color, t_sphere **list, t_scene *scene, t_ray *ray, int *nb_rebond)
{
   
    double t_min = -1;
    t_sphere *sphere;
    sphere = *list;
    float V ;
double dist;
double intensity;

if (*nb_rebond == 0)
    return(0);
    
    t_coord *pos = malloc(sizeof(t_coord));
    t_coord *normal = malloc(sizeof(t_coord));
    t_coord *l =malloc(sizeof(t_coord));
    t_sphere *min;


       while (sphere != NULL)
            {
                 if (intersection_sphere(sphere,ray,pos,normal,&t_min) == 1 )
                     min = sphere;
                 sphere =sphere->next;
            }
            sphere = min;
            if (sphere != NULL)
{


                    ft_vectors_substract(pos,scene->light->pos, l);
                    dist = ft_norm2(l);
                    ft_normalize(l);
                    V = ft_ombre(list, sphere, pos,normal, dist, scene);
                   
                    color->intensity = ((scene->light->i / PI)* ft_max(ft_scal_produce(l,normal),255) * V) / dist ;
                    color->intensity = (color->intensity < 0) ? 0 : color->intensity;
                    color->intensity = (color->intensity > 255) ? 255 : color->intensity;
          
                  if (color->intensity < 10)
                        {
                    t_ray aleatoire;
                    aleatoire.direction = malloc(sizeof(t_coord));
                    aleatoire.origin = malloc(sizeof(t_coord));
                     ft_vectors_mult(normal,0.001, aleatoire.origin);
                    ft_vectors_add(aleatoire.origin,pos,aleatoire.origin);
                    while (ft_norm2(aleatoire.direction) < 1)
                        ft_coord(generate_nb(),generate_nb(),generate_nb(),aleatoire.direction);
                    
                    *nb_rebond = *nb_rebond - 1;
    
                    
                  intensity += ft_color_intensity(color,list, scene, &aleatoire,nb_rebond);
                  color->intensity= intensity ;
                  color->intensity = (color->intensity < 0) ? 0 : color->intensity;
                    color->intensity = (color->intensity > 255) ? 255 : color->intensity;}
                     color->r= sphere->color->x;
                    color->g= sphere->color->y;
                    color->b = sphere->color->z;
                return (color->intensity);
                }
                else
                    {
                        color->intensity = 1;
                        color->r= 0;
                    color->g= 0;
                    color->b = 0;
                        return (1);
                    }
}




int             main(void)
{
    t_vars   vars;
   t_data  img;
    int     i= 0;
    int     j = 0;
    t_scene *scene = malloc(sizeof(t_scene));
    if (scene == 0 || ft_parsing_scene(scene) == -1)
    {
        free(scene);
        return(0);
    }
    t_sphere **list = malloc(sizeof(t_sphere *));
    if (list == 0 || ft_list_sphere(list) == -1)
        {
            free(list);
            return(0);
        }
    t_sphere *sphere = *list;

    ft_initialize_img(&vars, &img, scene);
    

t_ray *ray_reflect= malloc(sizeof(t_ray));
ray_reflect->direction = malloc(sizeof(t_coord));

t_palette color;

int nb_rebond = 10;
int k;
t_palette color_f;
while (i < scene->h)
{
    while (j < scene->w)
    {

       k = 0;
         double r1 = 0.5;
            double r2 = 0.5;
        while (k < nrays)
        {
            r1 +=0.1;
            r2-= 0.1;
            double R = sqrt(-2 *log(r1));
            double dx = R*cos(2 * PI *log(r2));
            double dy = R*sin(2 * PI*log(r2));
            if (nrays ==1)
            {
                dx = 0;
                dy=0;
            }
        ft_coord(i-(scene->h/2)+ dx,-j+(scene->w/2)+ dy, (scene->h/(2*tan(scene->fov/2))), scene->ray->direction);
        ft_normalize(scene->ray->direction);
        ft_color_intensity(&color, list,scene, scene->ray, &nb_rebond);
        
        nb_rebond = 10;
        color_f.r += color.r;
        color_f.g += color.g;
        color_f.b += color.b;
        color_f.intensity += color.intensity;
        k++;

        }
        color_f.r /= nrays;
        color_f.g /= nrays;
        color_f.b /= nrays;
        color_f.intensity/= nrays;
        my_mlx_pixel_put(&img, i, j,create_trgb(150,color_f.r * color_f.intensity,color_f.g * color_f.intensity,color_f.b * color_f.intensity));
        color.r = 0;
        color.g = 0;
        color.intensity = 0;
        color.b = 0;
        color_f.r =0;
        color_f.g =0;
        color_f.b =0;
        color_f.intensity  =0;

        j++;
    }
    printf("ok");
         j = 0;
    i++;
    }
    mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
    mlx_loop(vars.mlx);
}