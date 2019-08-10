//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#include "header_files/libraries.h"

/*  Structure of the custom tcl command "ls" */

int custom_command_ls (ClientData dummy,                	   /* Not used. */
    				   Tcl_Interp *interp,              /* Current interpreter. */
    				   int objc,                        /* Number of arguments. */
                       Tcl_Obj *const objv[]) 
{

	int i;
	int check;
	char ls_plus_arguments[LINE_MAX];

	/*initialize array ls_plus_arguments with command  "ls " */ 

	strcpy (ls_plus_arguments,"ls "); 

	if (objc == 1) 	
	{

	/* if no argument is specified next to "ls" 
	   command just call the system function using string "ls" */	

       system("ls");
       return TCL_OK;
    }
    else 
    {

	/* 
	if there are arguments next to command "ls" (for example -a or -l) do the following for all arguments:
	   1.Set current object of Tcl, argument obj[i]
	   2.Put argument inside the the string ls_plus_arguments next to "ls " 
	     or next to last argument from right side  (like "ls -a ")
	   3.Put " " next to last argument
	   4.Pass the string to system function 
	*/

		for(i = 1;i < objc;i++) 	
		{
			Tcl_SetObjResult(interp, objv[i]);
			strcat (ls_plus_arguments, Tcl_GetStringResult(interp) );      
			strcat(ls_plus_arguments," ");
	    }

		Tcl_ResetResult (interp);
		check = system(ls_plus_arguments);

		/*Check if system function failed */
		
		if (check == - 1) 	
		{
			return TCL_ERROR;
		}

	}
		return TCL_OK;
}

/* Structure of custom Tcl command "less". Same as custom "ls" */

int custom_command_less (ClientData dummy,                /* Not used. */
							    Tcl_Interp *interp,              /* Current interpreter. */
							    int objc,                        /* Number of arguments. */
							    Tcl_Obj *const objv[]) 
{

	int i;
	int check;
	char less_plus_arguments[LINE_MAX];

	strcpy (less_plus_arguments,"less ");

	if (objc == 1) 
	{
      
       system("less");
       return TCL_OK;
    }
    else 
    {
		for(i = 1;i < objc;i++) 
		{
			Tcl_SetObjResult(interp, objv[i]);	
			strcat (less_plus_arguments, Tcl_GetStringResult(interp) );
			strcat(less_plus_arguments," ");
	    }

		Tcl_ResetResult (interp);
		check = system(less_plus_arguments);

		if (check == - 1) 
		{
			return TCL_ERROR;
		}
	}

	return TCL_OK;
}