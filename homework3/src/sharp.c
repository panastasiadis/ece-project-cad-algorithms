//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#include "header_files/libraries.h"

/*  Structure of the operation sharp  */

int custom_sharp ( 
									 ClientData dummy,               /*  Not used.            */
    								 Tcl_Interp *interp,             /*  Current interpreter. */
    								 int objc,                       /*  Number of arguments. */
    								 Tcl_Obj *const objv[]			 /*  Argument objects.    */
    			 )         
    
{

	Tcl_Obj *newListPtr;
    Tcl_Obj **ArrayListAddr;

	Tcl_Obj *cube2list;
	Tcl_Obj **cube2ListAddr;

	Tcl_Obj *args[3];

	Tcl_Obj **sharpSumList;
	Tcl_Obj *sharp2Result;
	Tcl_Obj **sharpSumListAddr;

	Tcl_Obj **firstListAddr;

	Tcl_Obj *finalResult;

	Tcl_Obj *finalResultList;
	Tcl_Obj **FinalResultListAddr;

	char **correctStringElementCube2;

	char *cube1;
	char *stringCube2;

	char *product;
	char *nextElemOfProduct;

	int numOfElementsCube2;
	int numOfListArguments;
	int numOfElementsSharpSum;
	int numOfElementsFinalResult;

    int cube1Length;
 	int cube2Length;

 	int maxLengthCube2 = -1;

    int longestString;
	int len1;
	int len2;
	int diff;

	int thereIsNoResultLength;
	int cartesianProductLength = 1;
	int firstListLength;

	int index;
	int elemInArray;
	int flag;

	int noResultEnable = 1;

	int i,j,k;

   	const char allowed_characters[] = "01";

		//Check number of arguments. Cubes can only be two.

	    if (objc != 3)
	    {
	    	printf("Wrong Number of Arguments! To Insert Function Into Second Argument Use Format {f1 f2 .. fn}.");
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
		len1 = strspn (cube1, allowed_characters);

		if ( (len1 != cube1Length) || (cube1Length % 2 != 0 ) ) 
		{
			printf ("Wrong Type Of Argument! Only Positional Cube Notation Is Accepted!");
			free(cube1);
			Tcl_ResetResult (interp);
			return TCL_ERROR;
		}

		//Check cube 1 for 00 fields. Cubes must not cotain 00 value among the string, otherwise they are invalid.

		for ( i = 0; i < cube1Length - 1; i = i + 2 )
		{
			if ( *( cube1 + i ) == '0' && *( cube1 + i + 1) == '0' )
			{
				printf("Invalid Cube! Found One Or More '00' Fields In The Encoding.");
				Tcl_ResetResult (interp);
				free(cube1);
				return TCL_ERROR;
			}
		}

		//Reset result to clear interp from value of tcl_getstringresult that was called previously.

		Tcl_ResetResult(interp);

		//Create new list for fuction of argument 2 and make the elements of the function elements of a list. 

		cube2list = Tcl_NewListObj(0, NULL);

		//Get access to elements of the list. Specifically the number of the elements and an address to the location of each.

		Tcl_ListObjAppendList ( interp, cube2list, *(ArrayListAddr + 2) );
		Tcl_ListObjGetElements ( interp, cube2list, &numOfElementsCube2, &cube2ListAddr );


		//check each function element for invalid traits. 

		for ( i = 0; i < numOfElementsCube2; i++)
		{

			stringCube2 = Tcl_GetStringFromObj (*(cube2ListAddr + i), &cube2Length);

			//Save the length of the longest string element of function.

			if ( cube2Length > maxLengthCube2 )
			{
				maxLengthCube2 = cube2Length;
			}

			len2 = strspn ( stringCube2, allowed_characters );

			if ( (len2 != cube2Length) ||  (cube2Length % 2 != 0 ) ) 
			{
				printf ("Wrong Type Of Argument! Only Positional Cube Notation Is Accepted!");
				Tcl_ResetResult (interp);
				free(cube1);
				return TCL_ERROR;
			}

			for ( j = 0; j < cube2Length - 1; j = j + 2 )
			{
				if ( *( stringCube2 + j ) == '0' && *( stringCube2 + j + 1) == '0' )
				{
					printf("Invalid Cube! Found One Or More '00' Fields In The Encoding.");
					Tcl_ResetResult (interp);
					free(cube1);
					return TCL_ERROR;
				}
			}	

		}

		//create a char list of strings of all the elements of the function. 

		correctStringElementCube2 = malloc (sizeof (char *) * numOfElementsCube2);

		//check malloc for errors.

		if (correctStringElementCube2 == NULL)
		{
				printf ("Failed To Allocate Memory!");
				return TCL_ERROR;
		}

		//If-condition below corrects if needed all the arguments (cube1 and function cubes) by adding 11 fields in order all of the cubes to have the same length.

		if (cube1Length > maxLengthCube2 ) 
		{

			//if cube1 is the longest string, make size of cubes of the function equal to cube1 size (by adding 11 fields in the end).  

			longestString = cube1Length;

			for (i = 0; i < numOfElementsCube2; i++)
			{
				correctStringElementCube2[i] = (char *) malloc (longestString + 1);
				correctStringElementCube2[i] = Tcl_GetStringFromObj (*(cube2ListAddr + i), &cube2Length);
				diff = longestString - cube2Length;

				//check malloc for errors

				if (correctStringElementCube2[i] == NULL)
				{
					printf ("Failed To Allocate Memory!");
					return TCL_ERROR;
				}

				for (j = 0; j < diff; j++)
				{
					strcat(correctStringElementCube2[i], "1");
				}
			}
		}
		else if ( maxLengthCube2 > cube1Length )
		{

			//if  the longest string belongs to a cube of the function, make size of all cubes (cube1 and function) equal to longest cube size (by adding 11 fields in the end).  

			longestString = maxLengthCube2;

			for (i = 0; i < numOfElementsCube2; i++)
			{

				correctStringElementCube2[i] = (char *) malloc (maxLengthCube2 + 1);
				correctStringElementCube2[i] = Tcl_GetStringFromObj (*(cube2ListAddr + i), &cube2Length);

				diff = maxLengthCube2 - cube2Length;

				//check malloc for errors.

				if (correctStringElementCube2[i] == NULL)
				{
					printf ("Failed To Allocate Memory!");
					return TCL_ERROR;
				}

				for (j = 0; j < diff; j++)
				{
					strcat(correctStringElementCube2[i], "1");
				}
			}

			diff = maxLengthCube2 - cube1Length;
			cube1 = (char *) realloc (cube1, maxLengthCube2 + 1);

			//check malloc for errors.

			if (cube1 == NULL)
			{
				printf ("Failed To Allocate Memory!");
				return TCL_ERROR;
			}

			for ( i = 0; i < diff; ++i)
			{
				strcat(cube1, "1");
			}
		}

		else
		{

			//if cube1 and at least one of cubes of the function have equally the longest string, correct function only. 

			longestString = maxLengthCube2;

			for (i = 0; i < numOfElementsCube2; i++)
			{

				correctStringElementCube2[i] = (char *) malloc (maxLengthCube2 + 1);
				correctStringElementCube2[i] = Tcl_GetStringFromObj (*(cube2ListAddr + i), &cube2Length);

				diff = maxLengthCube2 - cube2Length;

				//check malloc for errors.

				if (correctStringElementCube2[i] == NULL)
				{
					printf ("Failed To Allocate Memory!");
					return TCL_ERROR;
				}

				for (j = 0; j < diff; j++)
				{
					strcat(correctStringElementCube2[i], "1");
				}
			}
		}

		//create objects which will pass as arguments for sharp_2 function.
		//argument one is word sharp_2

		args[0] =  Tcl_NewStringObj ("sharp_2", strlen("sharp_2") + 1);

		//arg1  will always be the cube1 argument.

		args[1] =  Tcl_NewStringObj (cube1, longestString);

		//create a list of all the list results of sharp_2. elements will be element1 = (cube1 # f1), element2 = (cube1 # f2) ...

		sharpSumList = malloc (sizeof (Tcl_Obj *) * numOfElementsCube2);

		//check malloc for errors.

		if ( sharpSumList == NULL )
		{	
			printf ("Failed To Allocate Memory!");
			return TCL_ERROR;
		}


		//call sharp 2 and store function result lists

		for (i = 0; i < numOfElementsCube2; i++)
		{	
			sharpSumList[i] = Tcl_NewListObj (0,NULL);

			args[2] = Tcl_NewStringObj (correctStringElementCube2[i], longestString);

			custom_sharp_2 (dummy, interp, 3, args);
			sharp2Result = Tcl_GetObjResult(interp);

			Tcl_ListObjAppendList (interp, sharpSumList[i], sharp2Result);
			Tcl_ResetResult(interp);
		}

		//if at least one of sharp_2 result was empty sharp operation will be empty too.
		//In this case return , with no result message.

		for (i = 0; i < numOfElementsCube2; i++)
		{

			if ( strcmp( Tcl_GetStringFromObj( sharpSumList[i], &thereIsNoResultLength ), "There Is No Result." ) == 0 )
			{
					free(correctStringElementCube2);
					free (cube1);
					free (sharpSumList);
					Tcl_SetResult (interp, "There Is No Result.", TCL_VOLATILE);
					return TCL_OK;
			}
		}

		//calculate number of elements of sharp list result.  
		//final result will be the cartesian product of all shap_2 results.

		for (i = 0; i < numOfElementsCube2; i++) {

			Tcl_ListObjGetElements ( interp, sharpSumList[i], &numOfElementsSharpSum, &sharpSumListAddr );
			cartesianProductLength = cartesianProductLength * numOfElementsSharpSum;

			//store separately addr and number of elements of first list. 

			if (i == 0) {
				firstListLength = numOfElementsSharpSum;
				firstListAddr = sharpSumListAddr;
			}
		}

		//create new list for final result

		finalResultList = Tcl_NewListObj (0,NULL);

		//allocate memory for product which is going to be one of the elements of the cartesian product.

		product = (char *) malloc (longestString + 1);

		//allocate memory for next value of product for the AND operation.

		nextElemOfProduct =  (char *) malloc (longestString + 1);

		//check malloc for errors.

		if ( product == NULL ) 
		{
			printf ("Failed To Allocate Memory!");
			return TCL_ERROR;
		}
		else if ( nextElemOfProduct == NULL ) 
		{
			printf ("Failed To Allocate Memory!");
			return TCL_ERROR;
		}  

		//calculate the cartesian product.

		for (i = 0; i < cartesianProductLength; i++)
		{

			//The idea of the code is, to have a counter, which creates an index over all elements of the resultset, 
			//and calculate the element, bound to that value.

			index = i;
			elemInArray = index % firstListLength;

			//store final list element i , first value of the sum of products. 

			strcpy ( product, Tcl_GetStringFromObj ( *( firstListAddr + elemInArray ), NULL ) );

			index = index / firstListLength;
			flag = 0;

			finalResult = Tcl_NewObj ();

			for (j = 1; j < numOfElementsCube2; j++)
			{

				Tcl_ListObjGetElements ( interp, sharpSumList[j], &numOfElementsSharpSum, &sharpSumListAddr );

				elemInArray = index % numOfElementsSharpSum;
				index = index / numOfElementsSharpSum;

				strcpy ( nextElemOfProduct , Tcl_GetStringFromObj ( *( sharpSumListAddr + elemInArray ), NULL ) );

				//calculate the value of the element i in final list- sum of products by executing AND operation.

				flag = 0;

				for ( k = 0; k < longestString + 1; k++ )
				{

					if ( ( *(product + k) == '0' ) || ( *(nextElemOfProduct + k) == '0' ) )
				  	{
					
						*(product + k) = '0';

				  	}
				  	else if (( *(product + k) == '1' ) && ( *(nextElemOfProduct + k) == '1' ) )
				  	{
				  	
				  		*(product + k) = '1';

				  	}
				}

			}

			//check for 00 fields in product value. flag index indicates whether the next value destined to be stored in final list is valid or not.

			for (j = 0; j < longestString; j = j + 2)
			{
			 	if  ( *(product + j) == '0' && *(product + j + 1) == '0' ) 
			 	{
			 		flag = 1;
			 	}
			} 

			//check if the value product was previously inserted into the list. If there is, reject the product using flag.  

			if ( flag != 1) 
			{
				Tcl_ListObjGetElements ( interp, finalResultList, &numOfElementsFinalResult, &FinalResultListAddr );
				for (j = 0; j < numOfElementsFinalResult; j++)
				{
					if ( strcmp ( product, Tcl_GetStringFromObj ( *( FinalResultListAddr + j ), NULL ) ) == 0 ) 
					{
						flag = 1;
					}
				}
			}

			//if product passed the above condtions, it is valid and should be inserted into the final list.

			if (flag == 0) 
			{
				noResultEnable = 0;
				Tcl_AppendStringsToObj (finalResult, product, NULL );
				Tcl_ListObjAppendElement (interp, finalResultList, finalResult);
			}
		}

		//if none of the products passed the conditions above, noResultEnable variable is 1 and so final list is empty.
		//In this case, print no result message, otherwise return to the interpeter the final list. 

		if (noResultEnable == 0 )
		{
			Tcl_SetObjResult (interp, finalResultList);
		}
		else
		{
			Tcl_SetResult (interp, "There Is No Result.", TCL_VOLATILE);
		}
		
		//free allocated space before return.

		free(correctStringElementCube2);
		free (cube1);
		free (sharpSumList);
		free(product);
		free (nextElemOfProduct);

		return TCL_OK;
}
