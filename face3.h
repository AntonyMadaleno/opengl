#ifndef FACE3_H
#define FACE3_H
#endif

class Face3
{
    public:
    unsigned p0,p1,p2;
    Face3(const unsigned, const unsigned, const unsigned);

    unsigned* getPoints();
    unsigned getPoint(unsigned short);
};