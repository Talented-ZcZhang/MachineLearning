#ifndef ZZC_PICDRAW_H
#define ZZC_PICDRAW_H
#include "Zzc_Matrix.h"
#include <windows.h>
int load_data(ZMatrix *&data,ZMatrix *&label);
void Draw_Line(int x1, int y1, int x2, int y2);
void Z_Draw_Line(double x1, double y1, double x2, double y2);
void Draw_Point(double *xy, double *label, int n);
void Z_Draw_Cost(double *cost, int num);
#endif