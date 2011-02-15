#!/bin/bash

# Script for playing Fuego on 9x9 CGOS with 1 core / 10000 playouts

FUEGO="../../build/opt-9/fuegomain/fuego"
VERSION=$(cd ../..; svnversion) || exit 1
DEFAULT_NAME=Fuego-$VERSION-10000sim

echo "Enter CGOS name (default=$DEFAULT_NAME):"
read NAME
if [[ "$NAME" == "" ]]; then
    NAME="$DEFAULT_NAME"
fi
echo "Enter CGOS password for $NAME:"
read PASSWORD

GAMES_DIR="games/$NAME"
mkdir -p "$GAMES_DIR"

cat <<EOF >config-9-10000.gtp
# This file is auto-generated by play-9-10000.sh. Do not edit.

go_param debug_to_comment 1
go_param auto_save $GAMES_DIR/$NAME-

uct_max_memory 256000000
uct_param_search number_threads 1
uct_param_player max_games 10000
uct_param_player early_pass 0

# Set CGOS rules (Tromp-Taylor, positional superko)
go_rules cgos

sg_param time_mode real
EOF

# Append 2>/dev/stderr to invocation, otherwise cgos3.tcl will not pass
# through stderr of the Go program
./cgos3.patched.tcl "$NAME" "$PASSWORD" \
  "$FUEGO --size 9 --config config-9-10000.gtp 2>/dev/stderr" \
  gracefully_exit_server-9-10000
