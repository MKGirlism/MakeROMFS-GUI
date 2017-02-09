#!/bin/bash

echo "古いオブジェクトファイルを削除した。"
echo "(Deleted old object files.)"
rm -rf *.o polarssl/*.o

echo " "
echo "Cコードをコンパイル中。。。"
echo "(Compiling C code...)"
gcc -c -std=c99 -static-libgcc -static-libstdc++ -static -o polarssl/aes.o polarssl/aes.c
gcc -c -std=c99 -static-libgcc -static-libstdc++ -static -o polarssl/base64.o polarssl/base64.c
gcc -c -std=c99 -static-libgcc -static-libstdc++ -static -o polarssl/bignum.o polarssl/bignum.c
gcc -c -std=c99 -static-libgcc -static-libstdc++ -static -o polarssl/rsa.o polarssl/rsa.c
gcc -c -std=c99 -static-libgcc -static-libstdc++ -static -o polarssl/sha1.o polarssl/sha1.c
gcc -c -std=c99 -static-libgcc -static-libstdc++ -static -o polarssl/sha2.o polarssl/sha2.c

echo "C++コードをコンパイル中。。。"
echo "(Compiling C++ code...)"
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o ByteBuffer.o ByteBuffer.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o crypto.o crypto.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o exefs.o exefs.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o exefs_code.o exefs_code.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o gui_winapi.o gui_winapi.cpp -mwindows
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o ivfc.o ivfc.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o main.o main.cpp -mwindows
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o ncch_header.o ncch_header.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o ncsd_header.o ncsd_header.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o oschar.o oschar.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o project_snake_exception.o project_snake_exception.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o romfs.o romfs.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o romfs_dir_filter.o romfs_dir_filter.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o romfs_dir_scanner.o romfs_dir_scanner.cpp
g++ -c -std=c++11 -static-libgcc -static-libstdc++ -static -o smdh.o smdh.cpp

echo "最後コンパイル中。。。"
echo "(Final compilation...)";
g++ -static-libgcc -static-libstdc++ -static -o makeromfs-gui.exe polarssl/*.o *.o -mwindows

echo "完了した！"
echo "(Done!)"
