#!/bin/bash
let "n = $1 - 1"
for i in `seq 0 $n`
  do ../build/bin/video_conference $1 $i &
done
