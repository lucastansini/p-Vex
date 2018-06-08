#!/bin/sh

sed -i -e 's/{sim:/\n&/g' $1
sed -i -e 's/ sim:/\n&/g' $1
sed -i -e 's/ \[[0-9]*\]/ /' $1

