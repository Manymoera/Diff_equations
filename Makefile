CXX = g++
CXXFLAGS_NO_OPTIMIZATION = -I./source
CXXFLAGS_CMD_O3 = -O3  -I./source
CXXFLAGS_CMD_O3_NATIVE_MARCH = -O3 -march=native -I./source
CXXFLAGS_CMD_O3_NATIVE_MARCH_FFAST = -O3 -march=native -ffast-math -I./source
CXXFLAGS_OMP = $(CXXFLAGS_CMD_O3_NATIVE_MARCH_FFAST) -fopenmp

TARGET_NO_OPTIMIZATION = bin/euler_no_optimization
TARGET_CMD_O3 = bin/euler_O3
TARGET_CMD_O3_NATIVE_MARCH = bin/euler_O3_native_march
TARGET_CMD_O3_NATIVE_MARCH_FFAST = bin/euler_O3_native_march_ffast
TARGET_OMP = bin/euler_OpenMP
TARGET_OMP_XORSHIFT = bin/euler_OpenMP_xorshift

TARGETS = $(TARGET_NO_OPTIMIZATION) \
          $(TARGET_CMD_O3) \
          $(TARGET_CMD_O3_NATIVE_MARCH) \
          $(TARGET_CMD_O3_NATIVE_MARCH_FFAST) \
          $(TARGET_OMP) \
		  $(TARGET_OMP_XORSHIFT)

all: prepare $(TARGETS)

prepare:
	@mkdir -p bin
	@mkdir -p data

$(TARGET_NO_OPTIMIZATION): source/algo_serial.cpp
	$(CXX) $(CXXFLAGS_NO_OPTIMIZATION) source/algo_serial.cpp -o $(TARGET_NO_OPTIMIZATION)

$(TARGET_CMD_O3): source/algo_serial.cpp
	$(CXX) $(CXXFLAGS_CMD_O3) source/algo_serial.cpp -o $(TARGET_CMD_O3)

$(TARGET_CMD_O3_NATIVE_MARCH): source/algo_serial.cpp
	$(CXX) $(CXXFLAGS_CMD_O3_NATIVE_MARCH) source/algo_serial.cpp -o $(TARGET_CMD_O3_NATIVE_MARCH)

$(TARGET_CMD_O3_NATIVE_MARCH_FFAST): source/algo_serial.cpp
	$(CXX) $(CXXFLAGS_CMD_O3_NATIVE_MARCH_FFAST) source/algo_serial.cpp -o $(TARGET_CMD_O3_NATIVE_MARCH_FFAST)

$(TARGET_OMP): source/algo_OpenMP.cpp
	$(CXX) $(CXXFLAGS_OMP) source/algo_OpenMP.cpp -o $(TARGET_OMP)

$(TARGET_OMP_XORSHIFT): source/algo_OpenMP_xorshift.cpp
	$(CXX) $(CXXFLAGS_OMP) source/algo_OpenMP_xorshift.cpp -o $(TARGET_OMP_XORSHIFT)

clean:
	rm -rf bin data

plot:
	@echo "Generating performance plots..."
	@python3 source/plot_bench.py