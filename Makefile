# C++ compiler executable
CXX=g++
# C++ compiler flags
CXXFLAGS=-lz -O2 -static
# Output file extension (for example .exe for Windows)
O_EXT=

gd_crypt$(O_EXT): Main.cpp Base64.hpp GDGameSave.hpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

xcomp_linux_win:
	make CXX=x86_64-w64-mingw32-g++ O_EXT=.exe

clean:
	rm ./gd_crypt
