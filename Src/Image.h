#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <string>

typedef struct 
{
    unsigned char* data;
    int dataLen;
    int width;
    int height;
    int components;
    int scale_denom;
    int quality;
}CommPicInfo;

void writeJpg(const char *fileName, CommPicInfo *picInfo);

#endif