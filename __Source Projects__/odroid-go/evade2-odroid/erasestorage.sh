 $IDF_PATH/components/esptool_py/esptool/esptool.py --chip esp32 \ 
    --port "/dev/cu.SLAB_USBtoUART" --baud 921600 erase_region 0xf00000 0x100000
