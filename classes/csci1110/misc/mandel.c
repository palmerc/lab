#include <stdio.h>
 
int main() {
   int maxCols = 1024;
   int maxRows = 768;
   int maxIterations = 2048;
   int maxSize = 4;
   float Xmin = -2.0;
   float Xmax = 1.2;
   float Ymin = -1.2;
   float Ymax = 1.2;
 
   double X, Y;
   double Xsquare, Ysquare;
   double currentP;
   double currentQ[767];
   double deltaP, deltaQ;
   int color;
   int currentRow, currentCol;
 
   deltaP = (Xmax - Xmin)/(double)maxCols;
 
   deltaQ = (Ymax - Ymin)/(double)maxRows;
 
   currentQ[0] = Ymax;
   for (currentRow = 1; currentRow < maxRows; currentRow++) {
      currentQ[currentRow] = currentQ[currentRow-1] - deltaQ;
   }
   currentP = Xmin;
 
   for (currentCol = 0; currentCol < maxCols; currentCol++) {
      for (currentRow = 0; currentRow < maxRows; currentRow++) {
         X = 0.0;
         Y = 0.0;
         Xsquare = 0.0;
         Ysquare = 0.0;
         color = 0;
         while ((color <= maxIterations) && (Xsquare + Ysquare <= maxSize)) {
            Xsquare = X * X;
            Ysquare = Y * Y;
            Y = (2*X*Y) + currentQ[currentRow];
            X = (Xsquare - Ysquare) + currentP;
            color++;
         }
   //    gl_setpixel(currentCol, currentRow, color % 256);
      }
      currentP = currentP + deltaP;
   //  gl_copyscreen(physicalscreen);
   }
 
   return 0;
}

