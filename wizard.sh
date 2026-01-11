#!/bin/sh
if [ -d build ]; then
	echo "Run me from build, not top directory"
	exit 1
fi

if [ "x$USE_WINE" = "x" ]; then
	USE_WINE=false
fi

if $USE_WINE; then
	if [ "x$WINE" = "x" ]; then
		W=wine
	else
		W=$WINE
	fi
	E=.exe
fi

$W ./engine/tools/gs_makepak$E base.pak ../engine/data
$W ./engine/tools/gs_makepak$E game.pak ../data
