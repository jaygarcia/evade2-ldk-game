if [[ ! -x "$(command -v tools/mkspiffs-mac)" ]] ; then
	echo "mkspiffs is not found!. aborting!"
	exit 1
fi

nameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    CYGWIN*)    machine=Cygwin;;
    MINGW*)     machine=MinGw;;
    *)          machine="UNKNOWN:${unameOut}"
esac

if [[ machine == "Linux" ]]; then
	echo "Todo: get linux working"
	exit 1
fi
musicDir="./nsf_music/"
port=/dev/cu.SLAB_USBtoUART

echo "Flashing $musicDir to $port..."
tools/mkspiffs-mac -c $musicDir -b 4096 -p 256 -s 0x110000 spiffs.bin


python $IDF_PATH/components/esptool_py/esptool/esptool.py --chip esp32 --port $port --baud 921600 write_flash -z 0x110000 spiffs.bin

