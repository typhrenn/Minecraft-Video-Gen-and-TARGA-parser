#include <stdio.h>
#include <stdlib.h>

long **getcolor(const char *sfile, int width, int height) 
{
    long size = width * height;
    long startbyte = 18;

    long **colors = malloc(width * sizeof(long *));
    if (!colors) {
        perror("Failed to allocate memory for grayscale");
        return NULL;
    }
    for (int i = 0; i < width; ++i) {
        colors[i] = malloc(height * sizeof(long) * 3);
        if (!colors[i]) {
            perror("Failed to allocate memory for grayscale row");
            for (int j = 0; j < i; ++j) {
                free(colors[j]);
            }
            free(colors);
            return NULL;
        }
    }

    FILE *file = fopen(sfile, "rb");
    if (!file) {
        perror("Failed to open file");
        for (int i = 0; i < width; ++i) {
            free(colors[i]);
        }
        free(colors);
        return NULL;
    }

    fseek(file, startbyte, SEEK_SET);

    unsigned char buffer;
    int curbyte = 0;
    int curcolr = 0;

    while (fread(&buffer, 1, 1, file) == 1) {
        if (curbyte >= width || curcolr >= 3) {
            break;
        }
        colors[curbyte][curcolr] = buffer;
        curcolr++;
        if (curcolr >= 3) {
            curcolr = 0;
            curbyte++;
        }
    }

    fclose(file);
    return colors;
}

long *getgrayscale(const char *sfile, int width, int height) 
{
    long size = width * height;
    long startbyte = 18;

    long *grayscale = malloc(size * sizeof(long));
    if (!grayscale) 
    {
        perror("Failed to allocate memory for grayscale");
        return NULL;
    }

    FILE *file = fopen(sfile, "rb");
    if (!file) 
    {
        perror("Failed to open file");
        free(grayscale);
        return NULL;
    }

    fseek(file, startbyte, SEEK_SET);

    unsigned char buffer[3];
    int curbyte = 0;

    while (fread(buffer, 1, 3, file) == 3)
    {
	curbyte++;
        if (curbyte > size) 
	{        	    
		break;
       	}
        grayscale[curbyte] = (buffer[0] + buffer[1] + buffer[2]) / 3;

    }

    fclose(file);
    return grayscale;
}
