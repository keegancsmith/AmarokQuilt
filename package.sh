#!/bin/bash

version=$(fgrep ':Version:' src/README | awk '{ print $2 }')

shopt -s extglob
rm -rf /tmp/amarokquilt
mkdir /tmp/amarokquilt
cp src/!(moc_*).@(cpp|h|pro) /tmp/amarokquilt
cp src/README /tmp/amarokquilt
cd /tmp/
rst2html.py --stylesheet /Documents/webdevel/homepage/stylesheet.css \
    amarokquilt/README amarokquilt/AmarokQuilt.html
tar cjf amarokquilt-$version.tar.bz2 amarokquilt
