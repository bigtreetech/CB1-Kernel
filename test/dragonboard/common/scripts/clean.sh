#!/bin/bash

targetdir=$(cd $(dirname $0) && pwd)/../..

if [ -d $targetdir/common/adb ]; then
	make -C $targetdir/common/adb clean
fi

if [ -d $targetdir ]; then
	make -C $targetdir/src clean
	rm -rf $targetdir/output/bin/
	rm -rf $targetdir/dragonboard/output/firmware
	rm -rf $targetdir/src/lib/libscript.a
	rm -rf $targetdir/*root*
fi

if [ -d $targetdir/dragonmat ]; then
	make -C $targetdir/dragonmat/src clean
	rm -rf $targetdir/dragonmat/output/bin/
	rm -rf $targetdir/dragonmat/*root*
	rm -rf $targetdir/dragonmat/src/lib/libscript.a
fi
