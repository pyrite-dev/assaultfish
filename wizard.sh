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
	$W ./engine/tools/gs_makepak$E ../data/mdl/$N.gmdl ../resource/model/$N
done
$W ./engine/tools/gs_makepak$E base.gpak ../engine/data
$W ./engine/tools/gs_makepak$E game.gpak ../data
