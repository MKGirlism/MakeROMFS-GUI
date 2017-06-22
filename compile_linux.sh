#!/bin/bash

echo "古いオブジェクトファイルを削除した。"
echo "(Deleted old object files.)"
rm -rf *.o polarssl/*.o

#echo "OSを発見。。。"
#echo "(Detecting OS...)"
#OS=$(lsb_release -si)
#NAME=$(os_release -si)

#a="発見したOSは"
#b=$OS
#3c="だ。"
#d=$a$b$c
#echo $d

#e="(Detected OS is "
#f=$OS
#g=".)"
#h=$e$f$g
#echo $h

#echo " "
#echo "g++、gcc、gtkmmは必要です。"
#echo "(g++, gcc, and gtkmm are required.)"

#if [ $OS = "Debian" ]; then
#	declare -a NEEDED_SOFTWARE=(gcc g++ libgtkmm-3.0-dev)
#	sudo apt-get update
	
#	for SOFTWARE in ${NEEDED_SOFTWARE[@]}; do
#		sudo apt-get -y install $SOFTWARE
#	done
#elif [ $OS = "Ubuntu" ]; then
#	declare -a NEEDED_SOFTWARE=(gcc g++ libgtkmm-3.0-dev)
#	sudo apt-get update
	
#	for SOFTWARE in ${NEEDED_SOFTWARE[@]}; do
#		sudo apt-get -y install $SOFTWARE
#	done
#elif [ $OS = "ManjaroLinux" ]; then
#	declare -a NEEDED_SOFTWARE=(gcc-multilib gtkmm3)
#	sudo pacman -Sy
	
#	for SOFTWARE in ${NEEDED_SOFTWARE[@]}; do
#		yes | sudo pacman -S $SOFTWARE
#	done
#elif [ $NAME = "Fedora" ]; then
#	declare -a NEEDED_SOFTWARE=(gcc gcc-c++ gtkmm30)
#	for SOFTWARE in ${NEEDED_SOFTWARE[@]}; do
#		sudo yum -y install $SOFTWARE
#	done
#else
#	echo "不明なOSを発見した。。。"
#	echo "(Unknown OS detected...)"
#	exit
#fi

echo " "
echo "Cコードをコンパイル中。。。"
echo "(Compiling C code...)"
gcc -c -std=c99 -o polarssl/aes.o polarssl/aes.c
gcc -c -std=c99 -o polarssl/base64.o polarssl/base64.c
gcc -c -std=c99 -o polarssl/bignum.o polarssl/bignum.c
gcc -c -std=c99 -o polarssl/rsa.o polarssl/rsa.c
gcc -c -std=c99 -o polarssl/sha1.o polarssl/sha1.c
gcc -c -std=c99 -o polarssl/sha2.o polarssl/sha2.c

echo "C++コードをコンパイル中。。。"
echo "(Compiling C++ code...)"
g++ -c -std=c++11 -o ByteBuffer.o ByteBuffer.cc
g++ -c -std=c++11 -o crypto.o crypto.cc
g++ -c -std=c++11 -o exefs.o exefs.cc
g++ -c -std=c++11 -o exefs_code.o exefs_code.cc
g++ -c -std=c++11 -o gui_gtkmm.o gui_gtkmm.cc `pkg-config gtkmm-3.0 --cflags --libs`
g++ -c -std=c++11 -o ivfc.o ivfc.cc
g++ -c -std=c++11 -o main.o main.cc `pkg-config gtkmm-3.0 --cflags --libs`
g++ -c -std=c++11 -o ncch_header.o ncch_header.cc
g++ -c -std=c++11 -o ncsd_header.o ncsd_header.cc
g++ -c -std=c++11 -o oschar.o oschar.cc
g++ -c -std=c++11 -o project_snake_exception.o project_snake_exception.cc
g++ -c -std=c++11 -o romfs.o romfs.cc
g++ -c -std=c++11 -o romfs_dir_filter.o romfs_dir_filter.cc
g++ -c -std=c++11 -o romfs_dir_scanner.o romfs_dir_scanner.cc
g++ -c -std=c++11 -o smdh.o smdh.cc

echo "最後コンパイル中。。。"
echo "(Final compilation...)";
g++ -o makeromfs-gui polarssl/*.o *.o `pkg-config gtkmm-3.0 --cflags --libs`

echo "完了した！"
echo "(Done!)"
