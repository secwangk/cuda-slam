#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
const int wide = 500;
const int height = 556;
  /*
   for (y = lasty; y >= starty; y--) 
    for (x = startx; x <= lastx; x++) {
      if (map[x][y] == 254) 
	fprintf(printFile, "%c%c%c", 255, 0, 0);
      else if (map[x][y] == 253) 
	fprintf(printFile, "%c%c%c", 0, 255, 200);
      else if (map[x][y] == 252) 
	fprintf(printFile, "%c%c%c", 255, 55, 55);
      else if (map[x][y] == 251) 
	fprintf(printFile, "%c%c%c", 50, 150, 255);
      else if (map[x][y] == 250) 
	fprintf(printFile, "%c%c%c", 250, 200, 200);
      else if (map[x][y] == 0) 
	fprintf(printFile, "%c%c%c", 100, 250, 100);
      else
	fprintf(printFile, "%c%c%c", map[x][y], map[x][y], map[x][y]);
    }
   */   
int main()
{
	FILE* printFile = fopen("Sample.ppm","w");
	
	fprintf(printFile, "P6\n # particles.ppm \n %d %d\n",
		wide, height);
	fprintf(printFile, "255\n");
	for(int y=height - 1; y>= 0; y--)
		for(int x = 0; x < wide; x++)
		{
			if(y >= height-50)
				fprintf(printFile,"%c%c%c",255,0,0);
			else if( y >= height - 100)
				fprintf(printFile,"%c%c%c",0,255,200);
			else if( y >= height - 150)
				fprintf(printFile,"%c%c%c",255,55,55);
			else if( y >= height - 200)
				fprintf(printFile, "%c%c%c", 50, 150, 255);
			else if (y >= height -  250) 
				fprintf(printFile, "%c%c%c", 250, 200, 200);
			else if ( y >= height -300 ) 
				fprintf(printFile, "%c%c%c", 100, 250, 100);								
			else
				fprintf(printFile, "%c%c%c", y,y,y);
		}
	fclose(printFile);
	system("convert Sample.ppm Sample.png");
	return 0;
}
