#!/bin/sh
# this file is in the Public Domain -- AKFoerster

# This script sets up search paths for the uninstalled package
# It is not meant for installation!

PWD="$(pwd)"
localdir=$(dirname "$0")

if [ "$localdir" = "." ]
then localdir="$PWD"
else if [ -d "$PWD/$localdir" ] # relative?
then localdir="$PWD/$localdir" # make absolute
fi
fi

# for finding Lua modules
# the ;; at the end adds the default path
LUA_PATH="${LUA_PATH:+$LUA_PATH;}$localdir/lua/?.lua;;"

# for finding Lua binary modules
# the ;; at the end adds the default path
LUA_CPATH="${LUA_CPATH:+$LUA_CPATH;}$localdir/?.so;$localdir/lua/?.so;;"

# for finding data (images, sounds, ...)
AVTDATAPATH="${AVTDATAPATH:+$AVTDATAPATH;}$localdir/data;/usr/local/share/akfavatar;/usr/share/akfavatar"

# On HP-UX change LD_LIBRARY_PATH to SHLIB_PATH
# On AIX change LD_LIBRARY_PATH to LIBPATH
# On Darwin/MacOS X change LD_LIBRARY_PATH to DYLD_LIBRARY_PATH
LD_LIBRARY_PATH="${LD_LIBRARY_PATH:+$LD_LIBRARY_PATH:}$localdir"

export LUA_PATH LUA_CPATH AVTDATAPATH LD_LIBRARY_PATH

exec "$localdir/lua-akfavatar-bin" "$@"
