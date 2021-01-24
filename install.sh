#! /bin/bash

ROOT=`pwd`
LDIR=$ROOT/lib
BUILD_DIR=$ROOT/build
LSOURCES=$ROOT/libsrc

GLEW_SRC=$LSOURCES/glew-2.1.0/build/cmake
GLEW_SRC_FILE=$LSOURCES/glew/glew.c
GLFW_SRC=$LSOURCES/glfw-3.3.2

mkdir -p $LDIR

echo building GLEW
cd $BUILD_DIR
cmake $GLEW_SRC
cd $BUILD_DIR
make glew_s
cp lib/libGLEW.a $LDIR
rm -rf $BUILD_DIR/*
echo done
echo

# echo Building GLFW
# cd $BUILD_DIR
# cmake -DBUILD_SHARED_LIBS=OFF $GLFW_SRC
# cd $BUILD_DIR/src
# make
# cp libglfw3.a $LDIR

# cp $(BUILD_DIR)/src/libglfw3.a $(LDIR)


# $(CC) -c -o $(BUILD_DIR)/glew.o $(GLEW_SRC_FILE)
# ar rcs $(LDIR)/libglew.a $(BUILD_DIR)/glew.o





rm -rf $BUILD_DIR/*
