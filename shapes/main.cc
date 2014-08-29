#include "shape.hh"

#include <cstdio>


int main(int argc, char** argv)
{
    shape* s = new shape('a', 4, 75);

    if (s->set_attach("") < 0) {
        printf("failed to set_attach for shape.\n");
        return -1;
    }

    return 0;
}
