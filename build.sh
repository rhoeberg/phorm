
mkdir -p build

cp -R assets build/

cd build

g++ ../code/main.cpp \
    -g \
    -D OSX \
    -std=c++14 \
    -I ../3rdparty \
    -framework opengl \
    -lGLEW \
    -lglfw3 \
    -framework IOKit \
    -framework CoreVideo \
    -framework Cocoa \
    -framework CoreAudio \
    -o test

cd ..
