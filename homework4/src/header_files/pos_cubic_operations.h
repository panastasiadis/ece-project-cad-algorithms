//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#ifndef POS_CUB_OPERATIONS_H_INCLUDED
#define POS_CUB_OPERATIONS_H_INCLUDED

//Declaration of operation intersect

int custom_cube_intersect_2 (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of operation supercube

int custom_supercube_2 (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of operation distance

int custom_distance_2 (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of operation cube cover

int custom_cube_cover_2 (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of operation sharp_2

int custom_sharp_2 (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of operation sharp

int custom_sharp (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of operation OFF_f

int custom_OFF_f (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);



#endif