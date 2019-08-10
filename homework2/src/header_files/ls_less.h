//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#ifndef LS_LESS_H_INCLUDED
#define LS_LESS_H_INCLUDED

//Declaration of tcl_shell system command ls

 int custom_command_ls (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

//Declaration of tcl_shell system command less

 int custom_command_less (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

#endif