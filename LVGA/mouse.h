//https://justdocodings.blogspot.com/2018/02/mouse-programming-in-turbo-cc.html


//int MouseXpos, MouseYpos, MouseBtn;
//union REGS in,out;

int InstallMouse();
int ShowMouse();
int HideMouse();
void GetMouseStatus();
int GetButton();
int GetPosX();
int GetPosY();
void SetMousePosi( int xpos, int ypos );
void RestrictMousePtr(int x1, int y1, int x2, int y2);

    