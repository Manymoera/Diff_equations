# Настройки компилятора
CXX = g++
CXXFLAGS = -O3 -march=native -ffast-math -fopenmp -I./source
TARGET = bin/euler

# Папки
SRCDIR = source
BINDIR = bin
DATADIR = data

# Список исходников
SOURCES = $(SRCDIR)/euler.cpp

# Основная цель
all: prepare $(TARGET)

# Создание необходимых папок
prepare:
	@mkdir -p $(BINDIR)
	@mkdir -p $(DATADIR)

# Компиляция
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Очистка проекта
clean:
	rm -rf $(BINDIR) $(DATADIR)