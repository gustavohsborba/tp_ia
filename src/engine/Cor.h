#ifndef AIDEMO14_1_RGB_H
#define AIDEMO14_1_RGB_H

typedef unsigned char uchar;

struct Cor {
    uchar r, g, b;
    Cor(uchar r, uchar g, uchar b):r(r),g(g),b(b){}
    Cor(): Cor(0,0,0){}
};

#endif
