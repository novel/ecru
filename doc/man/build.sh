#!/bin/sh

rm -fr html
mkdir html

for i in *.tex; do
	base=`echo $i|sed -e 's|.tex$||'`
	tex="$base.tex"
	html="$base.html"
	latex2man -H $tex html/$html
done
