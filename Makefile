CXX=g++
CXXFLAGS=-lz

gd_decr: Main.cpp Base64.hpp GDGameSave.hpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm ./gd_decr
