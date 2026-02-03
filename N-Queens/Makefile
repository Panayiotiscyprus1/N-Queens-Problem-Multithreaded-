CC := gcc

CFLAGS  := -Wall -Wextra -O2 -I.
PTHREAD := -pthread
OMP     := -fopenmp
# OMP     := -Xpreprocessor -fopenmp -lomp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include

COMMON_SRC := board.c safe.c
COMMON_OBJ := $(COMMON_SRC:.c=.o)

SEQ_OBJ := sequential/queens_seq.o
PTH_OBJ := HW1/queens_pthreads.o
OMP_OBJ := HW2/queens_omp.o

TARGETS := queens_seq queens_pthreads queens_omp

.PHONY: all clean

all: $(TARGETS)

# ================= Executables =================

queens_seq: $(SEQ_OBJ) $(COMMON_OBJ)
	$(CC) $^ -o $@

queens_pthreads: $(PTH_OBJ) $(COMMON_OBJ)
	$(CC) $^ -o $@ $(PTHREAD)

queens_omp: $(OMP_OBJ) $(COMMON_OBJ)
	$(CC) $^ -o $@ $(OMP)

# ================= Object files =================

sequential/queens_seq.o: sequential/queens_seq.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

HW1/queens_pthreads.o: HW1/queens_pthreads.c header.h
	$(CC) $(CFLAGS) $(PTHREAD) -c $< -o $@

HW2/queens_omp.o: HW2/queens_omp.c header.h
	$(CC) $(CFLAGS) $(OMP) -c $< -o $@

board.o: board.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

safe.o: safe.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

# ================= Cleanup =================

clean:
	rm -f $(TARGETS) \
	      *.o \
	      HW1/*.o \
	      HW2/*.o \
	      sequential/*.o
