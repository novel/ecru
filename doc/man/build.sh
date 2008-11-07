#!/bin/sh

rm -fr html man1
mkdir html man1

for i in *.tex; do
	base=`echo $i|sed -e 's|.tex$||'`
	tex="$base.tex"
	html="$base.html"
	man1="$base.1"
	latex2man -H $tex html/$html
	latex2man -M $tex man1/$man1
done
