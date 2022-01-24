//https://justdocodings.blogspot.com/2018/02/mouse-programming-in-turbo-cc.html


#include <conio.h>
#include <dos.h>
#include <stdlib.h>

    int MouseXpos, MouseYpos, MouseBtn;
    union REGS in,out;

    int InstallMouse()
    {
        // Initialize the mouse
        in.x.ax = 0;
        int86(0x33,&in,&out);
        return out.x.ax;
    }
    int ShowMouse()
    {
        // Show mouse pointer
        in.x.ax = 1;
        int86(0x33,&in,&out);
        return 1;
    }
    int HideMouse()
    {
        // Hide mouse pointer
        in.x.ax = 2;
        int86(0x33,&in,&out);
        return 1;
    }
    void GetMouseStatus()
    {
        // Get mouse position and button status
        in.x.ax = 3;
        int86(0x33,&in,&out);
        MouseXpos = out.x.cx;
        MouseYpos = out.x.dx;
        MouseBtn = out.x.bx;
    }
    int GetButton()
    {
        // Get mouse button status
        return MouseBtn;
    }

    int GetPosX()
    {
        // Get mouse X-Coordinate
        return MouseXpos;
    }

    int GetPosY()
    {
        // Get mouse Y-Coordinate
        return MouseYpos;
    }

    void SetMousePosi( int xpos, int ypos ){
        // Set mouse pointer position 
        in.x.ax = 4;
        in.x.cx = xpos;
        in.x.dx = ypos;
        int86(0x33,&in,&out);
    }

    void RestrictMousePtr(int x1, int y1, int x2, int y2)
    {
        // Set horizontal limits for pointer
        in.x.ax = 7;
        in.x.cx = x1;
        in.x.dx = x2;
        int86(0x33,&in,&out);

        // Set vertical limits for pointer
        in.x.ax = 8;
        in.x.cx = y1;
        in.x.dx = y2;
        int86(0x33,&in,&out);

    }
/*
    void main()
    {
        int gd=DETECT, gm, errorcode;
        int xpos, ypos;

        clrscr();

        if(!InstallMouse())
        {
            //cout<<"\n\n\t Mouse driver not loaded.";
            //cout<<"\n\n\n\t Press any key to exit...";
            getch();
            exit(1);
        }

        //cout<<"\n\n\t Mouse driver detected.";
        //cout<<"\n\n\n\t Press any key to continue.";
        getch();

         // initialize graphics and local variables
        initgraph(&gd,&gm,"c://app//tc//bgi");

        // read result of initialization
        errorcode = graphresult();

        if (errorcode != grOk)// an error occurred
        {
            //cout<<"Graphics error :: "<<grapherrormsg(errorcode);
            //cout <<"\n press any key to halt: ";
            getch();
            exit(1);// terminate with an error code
        }

        xpos = getmaxx()/2;
        ypos = getmaxy()/2;

        // draw rectangle

        setcolor(4);
        rectangle(100,100,getmaxx()-100,getmaxy()-100);
        RestrictMousePtr(100,100,getmaxx()-100,getmaxy()-100);
        SetMousePosi(xpos,ypos);
        ShowMouse();
        do{
            GetMouseStatus();
            gotoxy(5,3);
            //cout<<"Mouse X-Coordinate : "<<GetPosX();
            gotoxy(5,4);
            //cout<<"Mouse Y-Coordinate : "<<GetPosY();
            gotoxy(5,5);
            //cout<<"Button Pressed     : "<<GetButton();
            gotoxy(30,5);
            //cout<<"                                  ";
            gotoxy(30,5);

            switch(GetButton())
            {
                case 0:
                //cout<<"(Button not pressed)";
                break;
                case 1:
                //cout<<"(Left button pressed)";
                break;
                case 2:
                //cout<<"(Right button pressed)";
                break;
                case 4:
                //cout<<"(Middle button pressed)";
                break;
            }
        }while(!kbhit());
        HideMouse();
        closegraph();
    }
	*/