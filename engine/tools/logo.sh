#!/bin/sh
rm -f logo*.png
povray +W512 +H512 +Oresource/logo/logo.png resource/logo/logo.pov
povray +W128 +H128 +KFF45 resource/logo/logo.pov
convert -delay 10 logo*.png resource/logo/logo.gif
rm -f logo*.png
