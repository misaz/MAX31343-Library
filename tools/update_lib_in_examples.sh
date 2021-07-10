#!/bin/bash

for i in $(find ../examples/ -name MAX31343.c); do
    cp ../src/MAX31343.c $i
done

for i in $(find ../examples/ -name MAX31343.h); do
    cp ../src/MAX31343.h $i
done

for i in  ../examples/*/*_PSoC6/MAX31343_PlatformSpecific.c; do
    cp ../src/MAX31343_PlatformSpecific_PSoC6.c $i
done

for i in ../examples/*/*_PSoC6/MAX31343_PlatformSpecific.h; do
    cp ../src/MAX31343_PlatformSpecific_PSoC6.h $i
done

for i in  ../examples/*/*_STM32/Core/Src/MAX31343_PlatformSpecific.c; do
    cp ../src/MAX31343_PlatformSpecific_STM32.c $i
done

for i in  ../examples/*/*_STM32/Core/Inc/MAX31343_PlatformSpecific.h; do
    cp ../src/MAX31343_PlatformSpecific_STM32.h $i
done
