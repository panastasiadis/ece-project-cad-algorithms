//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#include "header_files/libraries.h"

/*  Structure of the operation off for a given function */

int custom_OFF_f ( 
								ClientData dummy,               /* Not used. */
    							Tcl_Interp *interp,             /* Current interpreter. */
    							int objc,                       /* Number of arguments. */
    							Tcl_Obj *const objv[]			 /* Argument objects. */
    						 )         
    
{
	Tcl_Obj *argsForSharp[3];
	Tcl_Obj *sharpUniFunction;

	char universe[] = {"11"};
	static int sharp_result;

		//check for wrong number of arguments.

		if (objc != 2)
	    {
	    	printf("Wrong Number of Arguments! For Function Use Format { fi f2 .. fn }.");
	    	return TCL_ERROR;

	    }

	    //idea of OFF_f is to call sharp function with cube1-argument1 to be the universe of cubes - 1111 # function.

	    //first argument to be inserted into sharo is the word sharp.

	    argsForSharp[0] =  Tcl_NewStringObj ("sharp", strlen("sharp") + 1);

	    //second argument is universe -string {11}

		argsForSharp[1] =  Tcl_NewStringObj (universe, strlen(universe));

		//third argument is the function that was typed by the user in command line.

		argsForSharp[2] = Tcl_NewObj();

		Tcl_AppendObjToObj (argsForSharp[2] ,objv[1]);

		//call sharp function.

		sharp_result = custom_sharp (dummy, interp, 3, argsForSharp);

		//if there was error in sharp, print error message for OFF_f, too.

		if (sharp_result == TCL_ERROR)
		{
			printf("\nFailed To Find OFF Set Of Giving Function!");
			return TCL_ERROR;
		}

		//Pass the result to the interpeter.

		sharpUniFunction = Tcl_GetObjResult(interp);
		Tcl_SetObjResult (interp, sharpUniFunction);
		

		return TCL_OK;
}