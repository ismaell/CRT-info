#!/bin/sh
make &&
mkdir -p qmusica
cp crtinfo.rom qmusica/2400.ci4
~/mame/qmusica64 -rp . qmusica
