# Yonatan Benchababt 345184295

CC = gcc

FLAGS = -std=c99 

PYTHON = python3

TARGET = fmm

C_SRCS = main.c fmm.c utilities.c 
PY_SRCS = create-matrices.py
HEADERS = fmm.h

OBJ = $(C_SRCS:.c=.o)

${TARGET}: ${C_SRCS}
	${CC} -O3 -o ${TARGET} ${C_SRCS}

run: ${TARGET}
	./${TARGET}

clean:
	rm -f *.o fmm