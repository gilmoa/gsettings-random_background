#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

#define CMD "gsettings set org.gnome.desktop.background picture-uri file://"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("usage: %s [absolute_path_to_wallpapers] [seconds]\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    char *path = argv[1];
    int t = atoi(argv[2]);
    if(t < 1)
    {
        printf("[seconds] must be at least 1.\n");
        exit(EXIT_SUCCESS);
    }

    if(path[strlen(path) - 1] != '/')
        strcat(path, "/");

    DIR *dir = opendir(path);
    if(dir == NULL)
    {
        printf("opendir() failed with: [%s]\n", path);
        exit(EXIT_FAILURE);
    }

    int array_step_size = 10;
    int array_size = array_step_size;
    char **imgs = malloc(array_step_size * sizeof(char*));
    if(imgs == NULL)
    {
        printf("Unable to alloc memory.\n");
        exit(EXIT_FAILURE);
    }

    struct dirent *file;
    int i = 0;

    while((file = readdir(dir)) != NULL)
    {
        if(i >= array_size)
        {
            array_size += array_step_size;
            imgs = realloc(imgs, array_size * sizeof(char*));
            if(imgs == NULL)
            {
                printf("Unable to realloc memory.\n");
                exit(EXIT_FAILURE);
            }
        }

        if(*(file->d_name) == '.')
            continue;

        imgs[i] = malloc((strlen(path) + strlen(file->d_name)) + 1);
        strcpy(imgs[i], path);
        strcat(imgs[i], file->d_name);

        printf("-%s- Loaded [%s]\n", argv[0], imgs[i]);

        i++;
    }

    printf("-%s- Total loaded: %d.\n", argv[0], i);

    srand(time(NULL));
    int rnd;
    char *cmd;

    while(1)
    {
        rnd = rand() % i;
        cmd = malloc(62 + strlen(imgs[rnd]) + 1);
        strcpy(cmd, CMD);
        strcat(cmd, imgs[rnd]);

        system(cmd);
        printf("-%s- Current: [%s]\n", argv[0], imgs[rnd]);

        free(cmd);
        sleep(t);
    }

    exit(EXIT_SUCCESS);
}
