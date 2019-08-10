##################################################################
#University      : Electrical & Computer Engineering
#Course          : CE437 - Computer-Aided Design Algorithms
#Full Name       : Panagiotis Anastasiadis
#Registry Number : 2134
#Project         :	"CAD Algorithms Implementation On A TCL Shell"
##################################################################

#Project Makefile

CC = gcc
CFLAGS = -Wall -g
EXTRALIBS = -lreadline -ltcl8.6
OBJ = main.o custom_ls_less.o intersect_2.o supercube_2.o distance_2.o cube_cover_2.o sharp_2.o sharp.o OFF_f.o graph_operations.o alg_division.o r_kernels.o

.PHONY: clean

homework4: $(OBJ)
	$(CC) $(OBJ) -o homework4 $(EXTRALIBS)
	rm -rf *.o *.dSYM


main: main.c header_files/libraries.h header_files/tcl_commands.h
	$(CC) $(CFLAGS) -c main.c

custom_ls_less: custom_ls_less.c header_files/libraries.h
	$(CC) $(CFLAGS) -c custom_ls_less.c

intersect_2: intersect_2.c header_files/libraries.h
	$(CC) $(CFLAGS) -c intersect_2.c

supercube_2: supercube_2.c header_files/libraries.h
	$(CC) $(CFLAGS) -c supercube_2.c

distance_2: distance_2.c header_files/libraries.h
	$(CC) $(CFLAGS) -c distance_2.c

cube_cover_2: cube_cover_2.c header_files/libraries.h
	$(CC) $(CFLAGS) -c cube_cover_2.c

sharp_2: sharp_2.c header_files/libraries.h
	$(CC) $(CFLAGS) -c sharp_2.c

sharp: sharp.c header_files/libraries.h
	$(CC) $(CFLAGS) -c sharp.c

OFF_f: OFF_f.c header_files/libraries.h
	$(CC) $(CFLAGS) -c OFF_f.c

graph_operations: graph_operations.c header_files/libraries.h
	$(CC) $(CFLAGS) -c graph_operations.c

alg_division: alg_division.c header_files/libraries.h
	$(CC) $(CFLAGS) -c alg_division.c

r_kernels: r_kernels.c header_files/libraries.h
	$(CC) $(CFLAGS) -c r_kernels.c

clean:
	rm -rf *.o *.dSYM
