#!/bin/bash
for i in $(seq 1 4)
do
    (time ./MyCalo run$i.mac) > MyCalo$i.log 2>&1
done
