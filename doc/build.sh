#!/bin/sh

rm -rf ecru-introduction
latex2html -split 0 -no_navigation -info 0 ecru-introduction.tex
