//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#ifndef GRAPH_FUNCTIONS_H_INCLUDED
#define GRAPH_FUNCTIONS_H_INCLUDED

//Declaration of exracting graph elements function 

int extract_GraphLine_Elements ( char *new_line , int *node, int *nodeConnectedTo, int *weight  );

//Declaration of read_graph function

int custom_read_graph ( ClientData dummy, Tcl_Interp *interp, int objc,Tcl_Obj *const objv[] );      

//Declaration of draw_graph function

int custom_draw_graph ( ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[] );

//Declaration of write graph function

int custom_write_graph ( ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[] );

//Declaration of graph critical path operation

int custom_graph_critical_path ( ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[] );

//Declaration of back trace function

Tcl_Obj *back_trace (Tcl_Interp *interp,  int slackEnableFlag, Tcl_Obj **fanInList, int *distance ,int max_distance, Tcl_Obj *rat_argument );

#endif