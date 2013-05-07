#!/bin/sh

git init
git add .
sync
git config color.status auto
git config color.diff true
git config core.editor vim
git config user.name "Lindell Xu"
git config user.email lxu@vetronicsltd.com.cn
git config core.paper "less -N"
git config color.diff true
git config alias.co checkout
git commit -a -s
