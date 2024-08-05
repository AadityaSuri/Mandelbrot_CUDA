CC = gcc
NVCC = nvcc
CFLAGS = -Wall -O3
CUDA_FLAGS = -O3
INCLUDE = -I.

# Specify the target executable name
TARGET = mandelbrot

# Specify the object files
OBJS = main.o utils/bitmap.o

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(NVCC) $(CUDA_FLAGS) $(OBJS) -o $(TARGET)

# Compile main.cu
main.o: main.cu utils/bitmap.h
	$(NVCC) $(CUDA_FLAGS) $(INCLUDE) -c main.cu

# Compile bitmap.c
utils/bitmap.o: utils/bitmap.c utils/bitmap.h
	$(CC) $(CFLAGS) $(INCLUDE) -c utils/bitmap.c -o utils/bitmap.o

# Clean up
clean:
	rm -f $(TARGET) *.o utils/*.o

# Phony targets
.PHONY: all clean