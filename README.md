# Meditation Game

Created by Mark Youngman

To build/run you need:
- gcc/g++
- SDL2
- SDL2-image

```
$ mkdir build
$ cd build
$ ../code/build.sh
$ ./meditation_game
```

### To build with emscripten

```
mkdir build
cd build
emcc ../code/meditation_game.cpp -o index.html -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=['PNG'] -s USE_LIBPNG=1 -s ALLOW_MEMORY_GROWTH=1 -D__EMSCRIPTEN__ --preload-file ../data
emrun --port 8080 .
```
