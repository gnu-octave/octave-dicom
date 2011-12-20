#!/bin/bash
echo "Remember to edit the version number in the DESCRIPTION"
WDIR=$(pwd)

# make package
PKG=$(basename $WDIR)
echo "Package=$PKG" 
cd ..
rm -r -f ~/$PKG
svn export $PKG ~/$PKG
VLINE=$(cat ~/$PKG/DESCRIPTION | grep Version:)
PKG_VERSION=${VLINE#* }
ARCHNAME=$PKG-$PKG_VERSION.tar.gz
rm    ~/dicom/mkpkg.sh
rm -r ~/dicom/dcm_examples
echo "Version=$PKG_VERSION"
cd ~
tar czf $ARCHNAME $PKG/
ls $ARCHNAME
md5sum $ARCHNAME
uuencode $ARCHNAME < $ARCHNAME > $ARCHNAME.uue

# make docs
rm -r -f $PKG-html
octave -q --eval "pkg load generate_html; generate_package_html $PKG $PKG-html octave-forge"
tar czf $PKG-html.tar.gz $PKG-html
md5sum $PKG-html.tar.gz 
uuencode $PKG-html.tar.gz < $PKG-html.tar.gz > $PKG-html.tar.gz.uue

cd $WDIR
