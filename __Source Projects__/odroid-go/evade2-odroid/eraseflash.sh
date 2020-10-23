#!/bin/bash
$IDF_PATH/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/cu.SLAB_USBtoUART --baud 921600 --before default_reset --after hard_reset erase_flash
