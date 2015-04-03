#ifndef _IMAGE_LOADER_H
#define _IMAGE_LOADER_H

unsigned char* loadBMP(const char* fname,int *width, int *height);
void unload_BMP(unsigned char* pixels);

#endif // _IMAGE_LOADER_H
