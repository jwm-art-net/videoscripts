/* ----------------------------------------
 *  polyseg
 *      generate coordinates of a single
 *      segment of a regular polygon.
 *
 *      to build, for example using gcc:
 *          gcc polyseg.c -lm -o polyseg
 *
 * --------------------------------------*/


#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510
#endif



#define NO_ARG  0
#define HAS_ARG 1


enum {
    OUT_INT = 0,
    OUT_SVG,
    OUT_FLOAT
};

typedef struct params
{
    float   angle;
    float   radius;
    float   xoffset;
    float   yoffset;
    float   rotate;
    int     type;
} params;


params* params_new()
{
    params* p = malloc(sizeof(*p));
    p->angle = -1.0f;
    p->radius = 1.0f;
    p->xoffset = 0.0f;
    p->yoffset = 0.0f;
    p->rotate = 0.0f;
    p->type = OUT_INT;
}


typedef struct coords
{
    float cx, cy;
    float px1, py1;
    float px2, py2;
} coords;


coords* coords_new()
{
    coords* c = malloc(sizeof(*c));
    if (!c)
        return 0;
    c->cx = 0.0f;
    c->cy = 0.0f;
    c->px1 = 0.0f;
    c->py1 = 0.0f;
    c->px2 = 0.0f;
    c->py2 = 0.0f;
    return c;
}


void help(void)
{
    puts("polysegcoords - calculate coordinates for a segment of a regular\n"
         "                polygon with optional offset and/or rotation.\n"
         "options:\n"
         "   --sides, -s    specify number of sides of polygon\n"
         "   --angle, -a    alternatively specify segment angle\n"
         "   --radius, -r   specify polygon radius (default: 1.0)\n"
         "   --xoffset, -x  horizontal center of polygon (default: 0.0)\n"
         "   --yoffset, -y  vertical center of polygon (default: 0.0)\n"
         "   --int, -i      output data as integer coordinates (default)\n"
         "   --float, -f    output data as floating point coordinates\n"
         "   --svg, -p      output data as svg path\n"
         "   --rotate, -r   rotation angle (default: 0.0)\n\n"
         "note: one of either --sides, or --angle *must* be specified.\n");
}


params* process_args(int argc, char** argv)
{
    float sides = -1.0f;

    if (argc < 2) {
        help();
        return 0;
    }

    params* p = params_new();
    if (!p) {
        puts("failed to create paramater data\n");
        return 0;
    }

    while (1) {
        int optix = 0;
        static struct option long_opts[] = {
            { "sides",      HAS_ARG, 0, 's' },
            { "angle",      HAS_ARG, 0, 'a' },
            { "radius",     HAS_ARG, 0, 'r' },
            { "xoffset",    HAS_ARG, 0, 'x' },
            { "yoffset",    HAS_ARG, 0, 'y' },
            { "rotate",     HAS_ARG, 0, 'R' },
            { "int",        NO_ARG,  0, 'i' },
            { "float",      NO_ARG,  0, 'f' },
            { "svg",        NO_ARG,  0, 'p' },
            { "help",       NO_ARG,  0, 'h' },
            { 0,            0,       0, 0   }
        };

        int c = getopt_long(argc, argv, "s:a:r:x:y:R:fph", long_opts, &optix);

        if (c == -1)
            break;

        switch(c) {
          case 's': sides = atof(optarg);       break;
          case 'a': p->angle = atof(optarg);    break;
          case 'r': p->radius = atof(optarg);   break;
          case 'x': p->xoffset = atof(optarg);  break; 
          case 'y': p->yoffset = atof(optarg);  break;
          case 'R': p->rotate = atof(optarg);   break;
          case 'f': p->type = OUT_FLOAT;        break;
          case 'p': p->type = OUT_SVG;          break;
          case 'h':
          default:
            printf("Unrecognized option\n");
            help(); 
            sides = p->angle = -1.0f;
        }
    }

    if (sides <= 0.0f && p->angle <= 0.0f) {
        free(p);
        help();
        return 0;
    }

    if (p->angle < 0.0f)
        p->angle = 360.0f / sides;

    return p;
}


coords* calculate(params* p)
{
    coords* c = coords_new();
    if (!c) {
        puts("failed to create coordinate data\n");
        return 0;
    }

    double a1 = (p->rotate * M_PI) / 180.0f;
    double a2 = ((p->angle + p->rotate) * M_PI) / 180.0f;

    c->cx = p->xoffset;
    c->cy = p->yoffset;
    c->px1 = c->cx + p->radius * sin(a1);
    c->py1 = c->cy + p->radius * cos(a1);
    c->px2 = c->cx + p->radius * sin(a2);
    c->py2 = c->cy + p->radius * cos(a2);

    return c;
}


int output(coords* c, params* p)
{
    if (!c || !p)
        return 0;

    switch (p->type) {
      case OUT_SVG:
        printf("M %f %f L %f %f L %f %f z\n", c->cx, c->cy, c->px1, c->py1,
                                                      c->px2, c->py2);
        break;
      case OUT_FLOAT:
        printf("%f %f  %f %f  %f %f\n", c->cx, c->cy, c->px1, c->py1,
                                                      c->px2, c->py2);
        break;
      case OUT_INT:
      default:
        printf("%d %d  %d %d  %d %d\n",
            (int)round(c->cx), (int)round(c->cy),
            (int)round(c->px1), (int)round(c->py1),
            (int)round(c->px2), (int)round(c->py2));
    }

    return 1;
}


int main(int argc, char** argv)
{
    int ret = 0;
    params* p = process_args(argc, argv);

    if (!p)
        return 1;

    coords* c = calculate(p);

    if (!c) {
        puts("calculation error. abort\n");
        ret = 1;
    }
    else {
        if (!output(c, p)) {
            puts("data output error. abort\n");
            ret = 1;
        }
    }

    free(c);
    free(p);

    return 0;
}
