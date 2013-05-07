#!/bin/sh

file=$1
shift

        echo   "$file"   ; 
        mplayer -vc dummy -af resample=44100 -ao pcm:waveheader \
                "$file" -ao pcm:file="$file.wav";

