sdcc --code-loc 0x0000 --xram-loc 0x0000 crtinfo.c
objcopy crtinfo.ihx ../crtinfo.u4 -I ihex -O binary
dd if=/dev/zero of=/tmp/zeroes-64k.rom bs=1 count=65536
cat ../crtinfo.u4 /tmp/zeroes-64k.rom > /tmp/tmp.rom
split /tmp/tmp.rom -b 65536
ls -lh xaa
mv xaa ~/ROM_DUMPS/FSanches/qmusica/2400.ci4
