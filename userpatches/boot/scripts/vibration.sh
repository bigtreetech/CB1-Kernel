#!/bin/bash

#================================
# sudo chmod 666 /sys/class/gpio/export
# sudo echo 79 > /sys/class/gpio/export
# cd /sys/class/gpio/gpio79
# sudo chmod 666 /sys/class/gpio/export
# sudo chmod 666 direction value
# sudo echo out > direction

#================================

sudo echo 1 > /sys/class/gpio/gpio79/value
sleep 0.04
sudo echo 0 > /sys/class/gpio/gpio79/value
