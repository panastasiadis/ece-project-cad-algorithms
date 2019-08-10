//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#include "header_files/libraries.h"

/*  Structure of the operation cube cover for 2 cubes */

int custom_cube_cover_2 ( 
									 ClientData dummy,               /* Not used. */
    								 Tcl_Interp *interp,             /* Current interpreter. */
    								 int objc,                       /* Number of arguments. */
    								 Tcl_Obj *const objv[]			 /* Argument objects. */
    							   )         
    
{

	char *cube1;
	char *cube2;
	char coverResult[LINE_MAX];

	int numOfListArguments;
    Tcl_Obj *newListPtr;
    Tcl_Obj **ArrayListAddr;

    int cube1Length;
    int cube2Length;
    int longestString;
	int len1;
	int len2;
	int diff;

	int bitsGreaterCube1 = 0;
	int bitsGreaterCube2 = 0;

	int i;
	int returnValue;

   	const char allowed_characters[] = "01";


		//Check for number of arguments. Cubes can only be two.

	    if (objc != 3)
	    {
	    	printf("Wrong Number of Arguments! You Must Specify Exactly Two Cubes For This Operation.");
	    	return TCL_ERROR;

	    }

	    //Create a list with command arguments as elements.

		newListPtr = Tcl_NewListObj(objc, objv);

		//Get access to the list by obtaining an address of the arguments array and a number of the sum of arguments.   

		Tcl_ListObjGetElements (interp, newListPtr, &numOfListArguments, &ArrayListAddr );
		
		// Set as Tcl Result the first argument and dynamically allocate memory (size of argument) to a pointer.

	  	Tcl_SetObjResult(interp, *(ArrayListAddr + 1));
	  	cube1 = (char *) malloc (strlen(Tcl_GetStringResult(interp)) + 1);

	  	//Check for errors in malloc function.

		if (cube1 == NULL)
		{
				printf ("Failed To Allocate Memory!");
				return TCL_ERROR;
		}  	

		//Store argument 1 - first cube string value to the allocated memory space.
		//Store the size of string, too.

	  	strcpy (cube1, Tcl_GetStringResult(interp));
		cube1Length = strlen (cube1); 

		//Do the same for argument 2 - second cube.

		Tcl_SetObjResult(interp, *(ArrayListAddr + 2));
		cube2 = (char *) malloc (strlen(Tcl_GetStringResult(interp)) + 1);

		if (cube2 == NULL)
			{
				printf ("Failed To Allocate Memory!");
				return TCL_ERROR;
			}

		strcpy (cube2, Tcl_GetStringResult(interp));
		cube2Length = strlen (cube2); 

		//Call strspn function in order to check cube1 and cube2 for invalid values.
		//If strspn manage to match entire string with the allowed characters {0,1}, result will be equal to the length of string.
		//Otherwise it will be smaller meaning that string contains invalid characters.  

		len1 = strspn (cube1, allowed_characters);
		len2 = strspn (cube2, allowed_characters);

		// Check the returned values of strspn {len1, len2} and also check for cube length if it is even or odd.
		// Valid values are even according to positional cubic notation.

		if ((len1 != cube1Length) || (cube1Length % 2 != 0 ) || (len2 != cube2Length) ||  (cube2Length % 2 != 0 ) ) {
			printf ("Wrong Type Of Argument! Only Positional Cube Notation Is Accepted!");
			free(cube1);
			free(cube2);
			Tcl_ResetResult (interp);
			return TCL_ERROR;
		}

		//Last Check. Cubes must not cotain 00 value among the string, otherwise they are invalid.

		for ( i = 0; i < cube1Length - 1; i = i + 2 )
		{
				if ( *( cube1 + i ) == '0' && *( cube1 + i + 1) == '0' )
				{
					printf("Invalid Cube! Found One Or More '00' Fields In The Encoding.");
					Tcl_ResetResult (interp);
					free(cube1);
					free(cube2);
					return TCL_ERROR;
				}
		}

		for ( i = 0; i < cube2Length - 1; i = i + 2 )
		{
				if ( *( cube2 + i ) == '0' && *( cube2 + i + 1) == '0' )
				{
					printf("Invalid Cube! Found One Or More '00' Fields In The Encoding.");
					Tcl_ResetResult (interp);
					free(cube1);
					free(cube2);
					return TCL_ERROR;
				}
		}

		//if the two cubes are the same cube, cover operation is pointless so we return the function. 

		if ( strcmp(cube1, cube2) == 0 )
		{
			sprintf (coverResult, "Cube %s and Cube %s are the same cube.", cube2, cube1);
			Tcl_SetResult (interp, coverResult, TCL_VOLATILE);
			free(cube1);
			free(cube2);
			return TCL_OK;
		}

		//Now we check which cube has bigger length. If a cube has lesser elements, we should fill the gap with "11". 

		if (cube1Length > cube2Length ) 
		{
			diff = cube1Length - cube2Length;
			longestString = cube1Length;

			//Reallocate memory of smaller cube. The new allocated space must be equal to other cube size.

			cube2 = (char *) realloc (cube2, cube1Length + 1);

			//Check for errors in realloc

			if (cube2 == NULL)
			{
				printf ("Failed To Allocate Memory!");
				return TCL_ERROR;
			}

			//Fill cube encoding with 1.

			for (i = 0; i < diff; ++i)
			{
				strcat(cube2, "1");
			}

		}

		else if (cube2Length > cube1Length)
		{
			diff = cube2Length - cube1Length;
			longestString = cube2Length;

			cube1 = (char *) realloc (cube1, cube2Length + 1);

			if (cube1 == NULL)
			{
				printf ("Failed To Allocate Memory!");
				return TCL_ERROR;
			}

			for (i = 0; i < diff; ++i)
			{
				strcat(cube1, "1");
			}
		}
		else
		{
			// If cube 1 and 2 have the same size.

			longestString = cube1Length;
		}

		//A cube covers another cube when the bits of the former are greater than or equal to those of the latter. The following loop exexcutes cover operation.

		for ( i = 0; i < longestString + 1; i++ )
		{
		 
			if ( (( *(cube1 + i) == '1' ) && ( *(cube2 + i) == '0' )) )
		  	{
			
		  		bitsGreaterCube1 = bitsGreaterCube1 + 1;

		  	}
		  	else if ( (( *(cube2 + i) == '1' ) && ( *(cube1 + i) == '0' )) )
		  	{
		  	
		  		bitsGreaterCube2 = bitsGreaterCube2 + 1;

		  	}
		  	else if ( (( *(cube2 + i) == '1' ) && ( *(cube1 + i) == '1' )) ||  (( *(cube2 + i) == '0' ) && ( *(cube1 + i) == '0' )) )
		  	{
		  		bitsGreaterCube1 = bitsGreaterCube1 + 1;
		  		bitsGreaterCube2 = bitsGreaterCube2 + 1;
		  	}
		}

		//If either cube1 or cube2 covers the other, bitsGreater counters will be equal to cube length. Otherwise there is no covering. 

		if (bitsGreaterCube1 != longestString && bitsGreaterCube2 != longestString  )
		{
			sprintf (coverResult, "Cube %s Does Not Cover Cube %s Or The Opposite.", cube1, cube2);
			returnValue = 0;
		}
		if ( bitsGreaterCube1 == longestString )
		{
			sprintf (coverResult, "Cube %s Covers Cube %s.", cube1, cube2);
			returnValue = 1;
		}
		else if ( bitsGreaterCube2 == longestString )
		{
			sprintf (coverResult, "Cube %s Covers Cube %s.", cube2, cube1);
			returnValue = 2;
		}

		//Set result of intersect as Tcl Result, in order to appear in the cell.

		Tcl_SetResult (interp, coverResult, TCL_VOLATILE);

		//Free unneeded allocated space.

		free(cube1);
		free(cube2);

    return returnValue;
}