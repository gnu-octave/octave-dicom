#!/bin/bash
echo "Remember to edit the version number in the DESCRIPTION"
WDIR=$(pwd)
PKG=$(basename $WDIR)
echo "Package=$PKG" 
cd ..
VLINE=$(cat $PKG/DESCRIPTION | grep Version:)
PKG_VERSION=${VLINE#* }
ARCHNAME=$PKG-$PKG_VERSION.tar.gz
rm -r -f ~/$PKG
svn export $PKG ~/$PKG
echo "Version=$PKG_VERSION"
tar czf $ARCHNAME $PKG/
ls $ARCHNAME
md5sum $ARCHNAME
uuencode $ARCHNAME < $ARCHNAME > $ARCHNAME.uue
cd $PKG
