#!/bin/bash
if [ $# -ne 1 ]; then
	exit 1;
fi
#dot $1 -Tpng -o _tmp.png && sz -y _tmp.png
dot $1 -Tpng -o _tmp.png
