net use LPT2: /DELETE /y
net use LPT2: \\print-buc-1\Lexmark_W812_ps-01-A-003
dosbox.exe -conf server.conf

net use LPT2: /DELETE /y
del *.tmp
exit

