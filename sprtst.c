
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>


#include "LVGA\\lvga.h"
#include "LVGA\\keyb.h"


/**************************************************************************
 *  Main                                                                  *
 *    Draws opaque and transparent bitmaps                                *
 **************************************************************************/

void main()
{
  int i,x,y;
  BITMAP bmp;

  load_bmp("rocket.bmp",&bmp);        /* open the file */

  //set_mode(VGA_256_COLOR_MODE);       /* set the video mode. */
  initialize_graphics(true);

  /* draw the background */
  for(i=0;i<200;i++)
    memset(&VGA[320*i],i,SCREEN_WIDTH);

  wait(25);

  /* draw a tiled bitmap pattern on the left */
  for(y=0;y<=SCREEN_HEIGHT-bmp.height;y+=bmp.height)
    for(x=0;x<=(SCREEN_WIDTH)/2-bmp.width;x+=bmp.width)
      draw_bitmap(&bmp,x,y);

  wait(25);

  /* draw a tiled transparent bitmap pattern on the right */
  for(y=0;y<=SCREEN_HEIGHT-bmp.height;y+=bmp.height)
    for(x=SCREEN_WIDTH-bmp.width;x>=SCREEN_WIDTH/2;x-=bmp.width)
      draw_transparent_bitmap(&bmp,x,y);

  wait(100);

  free(bmp.data);                     /* free up memory used */

  set_mode(TEXT_MODE);                /* set the video mode back to
                                         text mode. */

  return;
}
