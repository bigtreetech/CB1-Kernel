#!/bin/bash

echo 229 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio229/direction
while [ 1 ]
do
    echo 1 > /sys/class/gpio/gpio229/value
    sleep 0.5
    echo 0 > /sys/class/gpio/gpio229/value
    sleep 0.5
done
