g++ ./src/*.cpp  -fPIC -shared -O3 -o ./lib/libframebuffer.so $(pkg-config --cflags --libs opencv4)
