Download and extract icu(https://github.com/unicode-org/icu) to directory "icu".

# Windows
1. Open icu\icu4c\source\allinone\allinone.sln in VisualStudio2017 and build (Release|Win32)
1. Move all files under icu\icu4c\lib to icu\icu4c\bin
1. rename directory "icu" to "icu.msvc"

# MingW
1. Open msys32
1. pacman -S python
1 export PATH=Y:/G/Qt/Tools/mingw730_32/bin:Y:/G/Qt/Tools/mingw730_32/i686-w64-mingw32:/usr/bin **OR** export PATH=/C/local/Qt/Tools/mingw730_32/bin:/usr/bin

1. ~~copy Y:\G\Qt\Tools\mingw730_32\bin\mingw32-make.exe to make.exe~~
2. cd icu/icu4c/source
3. ./configure --prefix=$PWD/../
4. mingw32-make -j 4
4. mingw32-make install
5. move lib/* to bin/
6. rename icu to icu.mingw730_32

Check 'Add build library search path to PATH' in Qt's run settings within 'Project'




