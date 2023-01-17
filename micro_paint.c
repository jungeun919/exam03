#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_info
{
    int width;
    int height;
    char background;
}   t_info;

typedef struct s_shape
{
    char type;
    float x;
    float y;
    float width;
    float height;
    char color;
}   t_shape;

static int ft_error(FILE *file, char *paint)
{
    if (file)
        fclose(file);
    if (paint)
        free(paint);
    write(1, "Error: Operation file corrupted\n", 32);
    return (1);
}

int main(int argc, char *argv[])
{
    FILE *file;
    t_info info;
    t_shape shape;
    char *paint;
    int i;
	int count;
    int x, y;

    if (argc != 2)
    {
        write(1, "Error: argument\n", 16);
        return (1);
    }
    file = fopen(argv[1], "r");
    if (!file)
        return (ft_error(file, NULL));
    
    //get paint board
    if (fscanf(file, "%d %d %c\n", &info.width, &info.height, &info.background) != 3)
        return (ft_error(file, NULL));
    if (info.width <= 0 || info.width > 300 || info.height <= 0 || info.height > 300)
        return (ft_error(file, NULL));
    paint = (char *)malloc(info.width * info.height);
    i = -1;
    while (++i < info.width * info.height)
        paint[i] = info.background;

    //draw shape
    while (1)
    {
		count = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.color);
		if (count == -1)
			break ;

        if (count != 6 || shape.width <= 0.0f || shape.height <= 0.0f || (shape.type != 'r' && shape.type != 'R'))
            return (ft_error(file, paint));
        
        shape.width += shape.x;
        shape.height += shape.y;

        y = 0;
        while (y < info.height)
        {
            x = 0;
            while (x < info.width)
            {
                if (x >= shape.x & x <= shape.width && y >= shape.y && y <= shape.height)
                    if (shape.type == 'R' || x < shape.x + 1 || x > shape.width - 1 || y < shape.y + 1 || y > shape.height - 1)
                        paint[y * info.width + x] = shape.color;
                x++;
            }
            y++;
        }
    }

    //print shape
    y = 0;
    while (y < info.height)
    {
        write(1, &paint[y * info.width], info.width);
        write(1, "\n", 1);
        y++;
    }
    fclose(file);
    free(paint);
    return (0);
}
