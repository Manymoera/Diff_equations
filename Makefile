CXX = g++
CXXFLAGS_NO_OPTIMIZATION = -I./source
CXXFLAGS_BASE = -O3 -march=native -ffast-math -I./source
CXXFLAGS_OMP = $(CXXFLAGS_BASE) -fopenmp

TARGET_NO_OPTIMIZATION = bin/euler_no_optimization
TARGET_SERIAL = bin/euler
TARGET_OMP = bin/euler_OpenMP

all: prepare $(TARGET_NO_OPTIMIZATION) $(TARGET_SERIAL) $(TARGET_OMP)

prepare:
	@mkdir -p bin
	@mkdir -p data

$(TARGET_NO_OPTIMIZATION): source/algo_serial.cpp
	$(CXX) $(CXXFLAGS_NO_OPTIMIZATION) source/algo_serial.cpp -o $(TARGET_NO_OPTIMIZATION)

$(TARGET_SERIAL): source/algo_serial.cpp
	$(CXX) $(CXXFLAGS_BASE) source/algo_serial.cpp -o $(TARGET_SERIAL)

$(TARGET_OMP): source/algo_OpenMP.cpp
	$(CXX) $(CXXFLAGS_OMP) source/algo_OpenMP.cpp -o $(TARGET_OMP)

clean:
	rm -rf bin data