#!/bin/bash
for i in $(seq 1 4)
do
    ./MyCalo run$i.mac 2>&1 | tee MyCalo$i.log
done
echo "Finito"
