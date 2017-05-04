sdcc --code-loc 0x0000 --xram-loc 0x0000 crtinfo.c
objcopy crtinfo.ihx ../crtinfo.u4 -I ihex -O binary
cp ../crtinfo.u4 ~/ROM_DUMPS/FSanches/qmusica/2400.ci4
