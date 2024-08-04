#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <targaops.h>

#define WIDTH 240
#define HEIGHT 180
#define FRAMES 100
#define MHEIGHT 3

struct BlockValue
{
    const char *rBlock;
    char Identifier;
    short x_axis;
    short y_axis;
};

struct BlockValue block(long color, short x, short y)
{
    struct BlockValue rtrn;

    if (color < 0)
    {
        rtrn.rBlock = "UNKNOWN - NUMBER TOO SMALL";
        rtrn.Identifier = '?';
    }
    else if (color < 51)
    {
        rtrn.rBlock = "black_wool";
        rtrn.Identifier = 'b';
    }
    else if (color < 126)
    {
        rtrn.rBlock = "gray_wool";
        rtrn.Identifier = 'g';
    }
    else if (color < 201)
    {
        rtrn.rBlock = "light_gray_wool";
        rtrn.Identifier = 'l';
    }
    else if (color <= 255)
    {
        rtrn.rBlock = "white_wool";
        rtrn.Identifier = 'w';
    }
    else
    {
        rtrn.rBlock = "UNKNOWN - NUMBER TOO BIG";
        rtrn.Identifier = '?';
    }

    rtrn.x_axis = x;
    rtrn.y_axis = y;

    return rtrn;
}

int main()
{
    char frame[50];
    char function[50];

    struct BlockValue oldest = { "UNKNOWN", '?', -1, -1 };
    struct BlockValue last = { "UNKNOWN", '?', -1, -1 };

    for (int f = 1; f <= FRAMES; f++)
    {
        snprintf(frame, sizeof(frame), "frames/frame_%d.tga", f);
        snprintf(function, sizeof(function), "functions/frame_%d.txt", f);

        long *grayscale = getgrayscale(frame, WIDTH, HEIGHT);
        if (!grayscale)
        {
            fprintf(stderr, "Failed to get grayscale for frame %d: %s\n", f, strerror(errno));
            continue;
        }

        FILE *curframe = fopen(function, "w");
        if (!curframe)
        {
            fprintf(stderr, "Failed to open function file for frame %d: %s\n", f, strerror(errno));
            free(grayscale);
            continue;
        }

        for (short x = 0; x < HEIGHT; x++)
        {
            for (short y = 0; y < WIDTH; y++)
            {
                struct BlockValue current = block(grayscale[(WIDTH * x) + y], x, y);

                if (oldest.Identifier == '?' || last.Identifier == '?')
                {
                    oldest = current;
                    last = current;
                }
                else
                {
                    if (last.Identifier == oldest.Identifier)
                    {
                        if (current.Identifier == oldest.Identifier)
                        {
                            last = current;
                        }
                        else
                        {
                            fprintf(curframe, "fill %d %d %d %d %s\n", oldest.x_axis, oldest.y_axis, last.x_axis, last.y_axis, oldest.rBlock);
                            oldest = current;
                            last = current;
                        }
                    }
                    else
                    {
                        oldest = current;
                        last = current;
                    }
                }
            }
        }

        if (oldest.Identifier != '?' && last.Identifier != '?' && last.Identifier == oldest.Identifier)
        {
            fprintf(curframe, "fill %d %d %d %d %s\n", oldest.x_axis, oldest.y_axis, last.x_axis, last.y_axis, oldest.rBlock);
        }

        fclose(curframe);
        free(grayscale);
    }
	FILE *mainfunc = fopen("functions/mainfunc.mcfunction", "w");
	fprintf(mainfunc, "tellraw @a {\"text\":\"Github: typhrenn\",\"bold\":true,\"italic\":true,\"color\":\"aqua\"}");
	fprintf(mainfunc, "tellraw @a {\"text\":\"Github repository: https://github.com/typhrenn/Minecraft-Video-Gen-and-TARGA-parser\",\"bold\":true,\"italic\":true,\"color\":\"aqua\"}");
	for (int i = 1; i <= FRAMES; i++)
	{
		fprintf(mainfunc, "schedule function namespace:frame_%d %dt\n", i, i *2);
	}

    return 0;
}
