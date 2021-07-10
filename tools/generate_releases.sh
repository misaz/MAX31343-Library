#!/bin/bash
mkdir ../release

for i in PSoC6 STM32 MAX32625 AVR Arduino; do
    if [ -f ../src/MAX31343_PlatformSpecific_$i.c ]; then
        mkdir -p ../release/$i
        cp ../src/MAX31343.c ../release/$i
        cp ../src/MAX31343.h ../release/$i
        cp ../src/MAX31343_PlatformSpecific_$i.c ../release/$i/MAX31343_PlatformSpecific.c
        cp ../src/MAX31343_PlatformSpecific_$i.h ../release/$i/MAX31343_PlatformSpecific.h
    fi
done;
