//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#ifndef ALG_DIVISION_R_KERNELS_H_INCLUDED
#define ALG_DIVISION_R_KERNELS_H_INCLUDED

//Declaration of function that calculates algebraic division of 2 functions

int custom_alg_division (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of function R_KERNELS ( R_KERNELS algorithm implementation )

int custom_r_kernels (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of function cubes. Cubes Function, given a cube list and a literal, returns those cubes that contain the given literal 

Tcl_Obj *cubes (Tcl_Interp *interp, char **stringFunction, int numOfElementsFunction, int literalPosition,int isComplement  );

//Declaration of main r_kernels algorithm. Recursive Function.  

Tcl_Obj *r_kernels_algorithm (Tcl_Interp *interp,  Tcl_Obj *objv, char **stringFunction, int numOfElementsFunction, int maxLength ); 

#endif
