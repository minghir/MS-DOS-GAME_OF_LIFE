del *.exe
del *.obj
del *.swp
del dbg.txt
cls
REM  c:\app\tc\bin\tcc -Ic:\app\tc\include;c:\app\tc\src\pong\lvga -Lc:\app\tc\lib pong_vga.c lvga\keyb.c lvga\lvga.c


rem c:\app\tc\bin\tcc -O -G -ml -Ic:\app\tc\include;c:\app\tc\src\pong\lvga -Lc:\app\tc\lib pongvga2.c lvga\keyb.c lvga\lvga.c
rem c:\app\tc\bin\tcc -O -G -ml -Ic:\app\tc\include;c:\app\tc\src\pong\lvga -Lc:\app\tc\lib cfgtst.c lvga\keyb.c lvga\lvga.c lvga\ini.c
rem c:\app\tc\bin\tcc -O -G -ml -Ic:\app\tc\include;c:\app\tc\src\pong\lvga -Lc:\app\tc\lib sprtst.c lvga\keyb.c lvga\lvga.c

c:\app\tc\bin\tcc -O -G -ml -Ic:\app\tc\include;c:\app\tc\src\pong\lvga -Lc:\app\tc\lib dir_v.c
rem c:\app\tc\bin\tcc -O -G -ml -Ic:\app\tc\include;c:\app\tc\src\pong\lvga -Lc:\app\tc\lib color.c lvga\keyb.c lvga\lvga.c lvga\ini.c
rem c:\app\tc\bin\tcc -O -G -ml -Ic:\app\tc\include;c:\app\tc\src\pong\lvga -Lc:\app\tc\lib testg.c lvga\keyb.c lvga\lvga.c lvga\ini.c