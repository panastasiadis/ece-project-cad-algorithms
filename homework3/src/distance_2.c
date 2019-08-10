//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#include "header_files/libraries.h"

/*  Structure of the operation distance for 2 cubes */

int custom_distance_2 ( 
								ClientData dummy,               /* Not used. */
    							Tcl_Interp *interp,             /* Current interpreter. */
    							int objc,                       /* Number of arguments. */
    							Tcl_Obj *const objv[]			 /* Argument objects. */
    						 )         
    
{

	char *intersect;
	char distance_result[LINE_MAX];

	static int cube_intersect_2_result;
	int intersectLength;
	int i;
	int pairsOfZeros = 0;


	//For operation distance_2 of two cubes we just need to calculate total number of "00" fields in the encoding of intersect.
	//For this reason we extract the result of intersect by calling custom_cube_intersect_2 function.

	cube_intersect_2_result = custom_cube_intersect_2 (dummy, interp, objc, objv);

	//If intersect result was error, print error message for distance_2

	if (cube_intersect_2_result == TCL_ERROR)
	{
		printf("\nFailed To Calculate Distance!");
		return TCL_ERROR;
	}

	//Allocate memory for intersect result variable.  

	intersect = (char *) malloc (strlen(Tcl_GetStringResult(interp)) + 1);

	//Check for malloc errors.

	if (intersect == NULL)
	{
		printf ("Failed To Allocate Memory!");
		return TCL_ERROR;
	}

	//Copy the result as string to the new memory space. Store the string length, two

	strcpy (intersect, Tcl_GetStringResult(interp));
	intersectLength = strlen(intersect);

	//In the for-Loop, we calculate the number of 00 fields inside the result of intersect operation. 

	for ( i = 0; i < intersectLength - 1; i = i + 2 )
	{
		if ( *( intersect + i ) == '0' && *( intersect + i + 1 ) == '0' )
		{
			pairsOfZeros = pairsOfZeros + 1;
		}
	}

	//Clear the result for interp. 

	Tcl_ResetResult (interp);

	//Transfer integer value of distance result to new char array as string.

	sprintf (distance_result, "%d", pairsOfZeros);

	//Set distance result as the new result for interp. 

	Tcl_SetResult (interp, distance_result, TCL_VOLATILE);

	//Free memory space that was previously allocated with malloc.

	free (intersect);

	return TCL_OK;
}