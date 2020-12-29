#include "parsing.h"
#include "libft.h"

int ft_r(char *line, t_scene *scene)
{
    if (*line == 'R')
        line++;
    else 
        return(-1);
    while (*line && (*line == ' '))
        line++;
    if ((scene->r_x= ft_atoi_rt(line)) == 0)
        return(-1);
    while (*line >= '0' && *line <= '9')
        line++;
    if ((scene->r_y= ft_atoi_rt(line)) == 0)
        return(-1);
    return(1);
}
char *ft_parsing_position(char *line, t_coord *coord)
{
    coord->x = ft_atoi_rt(line);
    while (*line && *line != ',')
        line++;
    line++;
    coord->y = ft_atoi_rt(line);
    while (*line && *line != ',')
        line++;
    line++;
     coord->z = ft_atoi_rt(line);
    while (*line && (((*line >= '0') && (*line <= '9')) || (*line == '.') || (*line == '-')))
        line++;
    return(line);
}
char *ft_parsing_rgb(t_rgb *rgb, char *line)
{
    rgb->r = ft_atoi_rt(line);
    while (*line && (*line != ','))
        line++;
    if (*line)
        line++;
    rgb->g = ft_atoi_rt(line);
     while (*line && (*line != ','))
        line++;
    if (*line)
        line++;
     rgb->b = ft_atoi_rt(line);
    while (*line && ((*line >= '0') && (*line <= '9')))
        line++;
    return(line);
}

int ft_c(char *line, t_scene *scene)
{  
    if (*line == 'c')
        line++;
    else 
        return(-1);
    while (*line && (*line == ' '))
        line++;
    line = ft_parsing_position(line, &scene->camera.origin);
    while (*line && (*line == ' '))
        line++;
    line = ft_parsing_position(line, &scene->camera.direction);
     while (*line && (*line == ' '))
        line++;
    scene->fov = ft_atoi_rt(line);
    return(1);
}
int ft_a(char *line, t_scene *scene)
{
    if (*line == 'A')
        line++;
    else
        return(-1);
    while (*line && (*line == ' '))
        line++;
    if ((scene->amb_light.ratio = ft_atoi_rt(line)) < 0)
        return(-1);
     while ((*line >= '0' && *line <= '9') || (*line == '.'))
        line++;
    ft_parsing_rgb(&scene->amb_light.rgb,line);
    return(1);
}
int ft_l(char *line,t_light *light)
{
    if (*line != 'l')
        return (-1);
    line++;
    while (*line && (*line == ' '))
        line++;
    line = ft_parsing_position(line,&light->pos);
    while (*line && (*line == ' '))
        line++;
    light->i = ft_atoi_rt(line);
    while (*line && ((*line >= '0' && *line <= '9') || (*line == '.')))
        line++;
    ft_parsing_rgb(&light->rgb,line);
    return(1);

}
int ft_s(char *line, t_scene *scene)
{
    if (*line != 's')
        return(-1);
    line++;
    if (*line != 'p')
        return(-1);
    line++; 
    t_sphere *sphere = malloc(sizeof(t_sphere));
    while (*line && (*line == ' '))
        line++;
    line = ft_parsing_position(line,&sphere->origin);
    while (*line && (*line == ' '))
        line++;
    sphere->rayon = ft_atoi_rt(line);
    while (*line && ((*line >= '0' && *line <= '9') || (*line == '.')))
        line++;
    while (*line && (*line == ' '))
        line++;
    line = ft_parsing_rgb( &sphere->rgb,line);
    ft_lstadd_front(scene->list,ft_lstnew((void *)sphere, 0));
    return(1);
}
int ft_p(char *line, t_scene *scene)
{
    if (*line != 'p')
        return(-1);
    line++;
    if (*line != 'l')
        return(-1);
    line++; 
    t_plan *plan = malloc(sizeof(t_plan));
    while (*line && (*line == ' '))
        line++;
    line = ft_parsing_position(line,&plan->center);
    while (*line && (*line == ' '))
        line++;
    line = ft_parsing_position(line,&plan->direction);
    while (*line && (*line == ' '))
        line++;
    line = ft_parsing_rgb( &plan->rgb,line);
    ft_lstadd_front(scene->list,ft_lstnew((void *)plan, 1));
    return(1);
}

t_scene *main_parsing(void)
{
    char *line[100];
    int i;
    int nb;
    i = 0;
int fd;
fd=open("scenes/scene.rt",O_RDONLY);
while (get_next_line(fd, &line[i]) > 0)
    i++;
nb = ++i ;
i = 0;

t_scene *scene;
scene = malloc(sizeof(t_scene));

t_light *light = malloc(sizeof(t_light));
scene->light = light;
ft_r(line[0],scene);
ft_a(line[1],scene);
ft_c(line[3],scene);
ft_l(line[4],light);

scene->list = malloc(sizeof(t_list *));
i = 6;
while (i < nb)
{
    if (*line[i] == 's')
        ft_s(line[i],scene);
    if (*line[i] == 'p')
        ft_p(line[i],scene);
    i++;
}
/*
printf("ok");
t_list *tmp= *(scene->list);
while (tmp != NULL)
{


if (tmp->type == 1)
{
    t_plan *plan= (t_plan *)tmp->object;
    printf("%f\n",plan->center.x);
    printf("%f\n",plan->center.y);
    printf("%f\n",plan->center.z);
     printf("%f\n",plan->direction.x);
     printf("%f\n",plan->direction.y);
     printf("%f\n",plan->direction.z);
     printf("%f\n",plan->rgb.r);
     printf("%f\n",plan->rgb.g);
     printf("%f\n",plan->rgb.b);
}
tmp = tmp->next;
}

tmp = tmp->next;
sphere_ptn = (t_sphere *)tmp->object;*/

/*
printf("\n%f\n",sphere_ptn->origin.x);
printf("%f\n",sphere_ptn->origin.y);
printf("%f\n\n",sphere_ptn->origin.z);
printf("%f\n\n",sphere_ptn->rayon);
printf("%f\n",sphere_ptn->rgb.r);
printf("%f\n",sphere_ptn->rgb.g);
printf("%f\n",sphere_ptn->rgb.b);
printf("\n\n ---------\n");*/

    return(scene);
}