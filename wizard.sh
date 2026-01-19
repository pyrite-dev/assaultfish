#!/bin/sh
if [ -d build ]; then
	echo "Run me from build, not top directory"
	exit 1
fi

if [ "x$USE_WINE" = "x" ]; then
	USE_WINE=false
fi

if [ ! "x$WINE" = "x" ]; then
	W=$WINE
	E=.exe
fi

ls -d ../resource/model/* | while read a; do
	N=`echo $a | rev | cut -d/ -f1 | rev`
	echo "----- $N.gsm"
	$W ./engine/tools/gs_makepak$E ../data/mdl/$N.gsm ../resource/model/$N
done
echo "----- base.gsp"
$W ./engine/tools/gs_makepak$E base.gsp ../engine/data
echo "----- game.gsp"
$W ./engine/tools/gs_makepak$E game.gsp ../data
