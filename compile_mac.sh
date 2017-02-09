#!/bin/bash

echo "古いオブジェクトファイルを削除した。"
echo "(Deleted old object files.)"
rm -rf *.o polarssl/*.o

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
g++ -c -std=c++11 -o ByteBuffer.o ByteBuffer.cpp
g++ -c -std=c++11 -o crypto.o crypto.cpp
g++ -c -std=c++11 -o exefs.o exefs.cpp
g++ -c -std=c++11 -o exefs_code.o exefs_code.cpp
g++ -c -std=c++11 -o gui_cocoa.o gui_cocoa.mm -x objective-c -Wno-import -framework Cocoa
g++ -c -std=c++11 -o ivfc.o ivfc.cpp
g++ -c -std=c++11 -o main.o main.cpp -x objective-c -Wno-import -framework Cocoa
g++ -c -std=c++11 -o ncch_header.o ncch_header.cpp
g++ -c -std=c++11 -o ncsd_header.o ncsd_header.cpp
g++ -c -std=c++11 -o oschar.o oschar.cpp
g++ -c -std=c++11 -o project_snake_exception.o project_snake_exception.cpp
g++ -c -std=c++11 -o romfs.o romfs.cpp
g++ -c -std=c++11 -o romfs_dir_filter.o romfs_dir_filter.cpp
g++ -c -std=c++11 -o romfs_dir_scanner.o romfs_dir_scanner.cpp
g++ -c -std=c++11 -o smdh.o smdh.cpp

echo "最後コンパイル中。。。"
echo "(Final compilation...)";
g++ -o makeromfs-gui polarssl/*.o *.o -x objective-c -Wno-import -framework Cocoa

echo "完了した！"
echo "(Done!)"
