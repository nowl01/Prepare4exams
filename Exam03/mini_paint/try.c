#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_circle
{
    char type;
    float x;
    float y;
    float radius;
    char c;
}t_circle;

typedef struct  s_zone
{
    float width;
    float height;
    char ch;
}t_zone;

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_putstr(char *str)
{
    while (*str)
    {
        ft_putchar(*str);
        str++;
    }
}

int ft_free(FILE *file, char *draw, char *msg)
{
    fclose(file);
    if (draw)
        free(draw);
    if (msg)
        ft_putstr(msg);
    return 1;
}

char *get_zone(FILE *file, t_zone *zone)
{
    int ret;
    int i;
    char *draw;

    if ((ret = fscanf(file, " %f %f %c\n", &zone->width, &zone->height, &zone->ch)) != 3)
        return NULL;
    if (ret == -1)
        return NULL;
    if (zone->width <= 0 || zone->height <= 0 || zone->width > 300 || zone->height > 300)
        return NULL;
    if (!(draw = malloc(sizeof(char) * zone->width * zone->height)))
        return NULL;
    i = 0;
    while (i < zone->height *zone->width)
        draw[i++] = zone->ch;
    return draw; 
}

int is_in_circle(int x, int y, t_circle *circ)
{
    float distance;

    distance = sqrtf(powf(x - circ->x, 2.) + powf(y - circ->y, 2.));
    if (distance <= circ->radius)
    {
        if (circ->radius - distance < 1.)
            return 2;
        return 1;
    }
    return 0;
}

int drawing_shape(FILE *file, t_zone *zone, char *draw)
{
    int ret1;
    int ret;
    int i;
    int j;
    t_circle circ;

    ret = 0;
    while ((ret = fscanf(file, " %c %f %f %f %c\n", &circ.type, &circ.x, &circ.y, &circ.radius, &circ.c)) == 5)
    {
        if (circ.radius <= 0.00000000 || (circ.type != 'r' && circ.type != 'R'))
            return 0;
        j = 0;
        while (j < zone->height)
        {
            i = 0;
            while (i < zone->width)
            {
                ret1 = is_in_circle(i, j, &circ);
                if ((ret1 == 2 && circ.type == 'c') || (circ.type == 'C' && ret1))
                    draw[j * (int)zone->width + i] = circ.c;
                i++;
            }
            j++;
        }
        return 1;
    }
    if (ret != 5)
        return 0;
    return 1;
}

int main(int ac, char **av)
{
    int i;
    char *draw;
    FILE *file;
    t_zone zone;

    if (ac != 2)
    {
        ft_putstr("Error: argument\n");
        return 1;
    }
    if (!(file = fopen(av[1], "r")))
    {
        ft_putstr("Error: Operation file corrupted\n");
        return 1;
    }
    if (!(draw = get_zone(file, &zone)))
        return(ft_free(file, NULL, "Error: Operation file corrupted\n"));
    if (!(drawing_shape(file, &zone, draw)))
        return (ft_free(file, NULL, "Error: Operation file corrupted\n"));
    i = 0;
    while (i < zone.height)
    {
        write(1, draw + (i *(int)zone.width), zone.width);
        write(1, "\n", 1);
        i++;
    }
    ft_free(file, draw, NULL);
    return 0;
}