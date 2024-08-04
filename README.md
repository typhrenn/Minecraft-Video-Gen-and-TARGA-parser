# Minecraft Video Gen and TARGA file format parser

# Compilation:
You can compile the program by running a simple gcc command:
`gcc -o uwu -Iinclude src/*.c`
And run it by typing in:
`./uwu`
Its important to note that this program only works on linux systems and only generates the functions for the datapack not the datapack itself since i was too lazy to do that.

# Usage:

**You can generate the frames from video by using linux command:**
`ffmpeg -i FILE.mp3 -vf "fps=10,scale=out_color_matrix=bt709:out_range=full" -coder raw -pix_fmt rgb24 frames/frame_%d.tga`

The main function file should be in the same directory as `function`  and `frames` directory, if you want to have smaller framerate i would recommend changing `fps=10` to `fps=5` and change **line 137**:
`fprintf(mainfunc, "schedule function namespace:frame_%d %dt\n", i, i *2)`
to
`fprintf(mainfunc, "schedule function namespace:frame_%d %dt\n", i, i *4)

# Macros:

`#define WIDTH 240`
`#define HEIGHT 180`
`#define FRAMES 100`
`#define MHEIGHT 3`

You should change the macros to the following:
`WIDTH` to the width of the frames;
`HEIGHT` to the height of the frames;
`FRAMES` to the amount of frames wanted to be processed;
`MHEIGHT` to the height you want the video to be generated on in minecraft;
