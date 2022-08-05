CXX = g++

# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install

# Collected from : https://www.willusher.io/sdl2%20tutorials/2013/08/15/lesson-0-linux-command-line
# -Wl -> Stands for Verbose
#  So one can use

SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include
# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
# to create object file from .cpp files
CXXFLAGS = -Wall -std=c++17 -pthread $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)

## When use the following line
## uncomment the :XY1: below
## Because the OBJ MandelBrot.o should be created 
## by the :XY1:.

# OBJS = MandelBrot.o delete_2.o
OBJS = delete_2.o
EXE = SDL_Lesson0

all: $(EXE)

$(EXE): $(OBJS)  
	$(CXX) $(CXXFLAGS) -o $@  $(OBJS) $(LDFLAGS)

## :XY1:
# mandelbrot.o: mandelbrot.cpp mandelbrot.h
#	$(CXX) $(CXXFLAGS) -c $< -o $@
	
#g++ -c file_name is used to only compile and assemble the file_name and not link the object code to produce executable file. 
#It will generate a file_name.o object code file in present working directory.


delete_2.o: delete_2.cpp mandelbrot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
#g++ -c file_name is used to only compile and assemble the file_name and not link the object code to produce executable file. 
#It will generate a file_name.o object code file in present working directory.

clean:
	rm *.o && rm $(EXE)