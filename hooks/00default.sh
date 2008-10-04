#!/bin/sh
#
# 'sheval' - ecru hook which evals text line by line.
#  E.g. if you have set property 'prop: `uname'' it will
#  expand to `prop: FreeBSD`, etc
#

cat $1|while read line;do eval echo "$line";done > $1
exit 0
