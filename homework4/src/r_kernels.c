//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#include "header_files/libraries.h"

//Final Result as a list of kernels cube lists (global)

Tcl_Obj **kernelsResult;

//Counter of final result elements (global)
	
int resultCounter = 0;


////////////////////////////////////////////////////////////////////---> Implementation of the R_KERNELS algorithm as a TCL custom command.

int custom_r_kernels (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[] ) 
  {

  	const char allowed_characters[] = "01";

  	Tcl_Obj **functionAddr;
  	int numOfElementsFunction;

  	char *stringCube;
  	int cubeLength;

  	char **stringFunction;

  	int len;
  	int maxLength = -1;
  	int diff;


  	//Check number of arguments. User can specify only one function-cube list.

  	if (objc != 2)
	 {
	   	printf("Wrong Number of Arguments! For Cube List Use Format { f1 f2 .. fn }.");
	    return TCL_ERROR;
	 }

//----------------------------------Check arguments for syntax errors-------------------------------------------------------------------//

	//Extract elements of argument - function

	Tcl_ListObjGetElements ( interp, objv[1], &numOfElementsFunction, &functionAddr );

	//Check function - cube list for syntax errors

	for (int i = 0; i < numOfElementsFunction; i++)
	  {

	  	//Extract string values of each cube in dividend

		stringCube = Tcl_GetStringFromObj (*( functionAddr + i ), &cubeLength);

		//Save the length of the longest string element of function.

		if ( cubeLength > maxLength )
		 {
			maxLength = cubeLength;
		 }

		len = strspn ( stringCube, allowed_characters );

		if ( (len != cubeLength) ||  (cubeLength % 2 != 0 ) ) 
		 { 
			printf ("Wrong Type Of Argument! Only Positional Cube Notation Is Accepted!");
			return TCL_ERROR;
		 }

		//Check each cube for 00 fields. Cubes must not cotain 00 value among the string, otherwise they are invalid.

		for (int j = 0; j < cubeLength - 1; j = j + 2 )
		  {
			if ( *( stringCube + j ) == '0' && *( stringCube + j + 1) == '0' )
			 {
				printf("Invalid Cube! Found One Or More '00' Fields In The Encoding.");
				return TCL_ERROR;
			 }
		  }	
	  } 

	//Allocate space for function as string

	stringFunction = malloc (sizeof (char *) * numOfElementsFunction);

	//check malloc for errors.

	if ( stringFunction == NULL )
	 { 
		printf ("Failed To Allocate Memory!");
		return TCL_ERROR;
	 }

	//Make size of all cubes in function cube list equal by adding "11" where its neccessary

	for (int i = 0; i < numOfElementsFunction; i++)
	  {
		stringFunction[i] = (char *) malloc (maxLength + 1);

		//check malloc for errors

		if (stringFunction[i] == NULL)
		 {
			printf ("Failed To Allocate Memory!");
			return TCL_ERROR;
		 }

		strcpy( stringFunction[i], Tcl_GetStringFromObj (*(functionAddr + i), &cubeLength));

		diff = maxLength - cubeLength;

		//Add "11" to increase size 

		for (int j = 0; j < diff; j++)
		  {
			strcat(stringFunction[i], "1");
		  }
	  }

//------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------Execute main R_Kernels Algorithm | Begin Recursive Process------------------------//
	
	//Begin recursive process of r_kernels algorithm. First call of the recursive function.

	r_kernels_algorithm (interp, objv[1], stringFunction, numOfElementsFunction, maxLength );
	
	//Final Result: increment counter of elements, allocate extra space in memory and initialize the list for element to be added.  

	resultCounter++;
	kernelsResult = realloc (kernelsResult, sizeof (Tcl_Obj *)*(resultCounter));
	kernelsResult[resultCounter - 1] = Tcl_NewListObj (0, NULL);

	//append list of kernels of first function (given as argument next to executed command "r_kernels") as the last element to final result 
	
	Tcl_ListObjAppendList(interp, kernelsResult[resultCounter - 1], objv[1]);

//------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------Display Results------------------------------------------------//

	//Print Result of R_Kernels algorithm.

	printf( "\nKernels:\n" );

	for (int i = 0; i < resultCounter; i++)
	 {
		printf("%d. %s\n", i, Tcl_GetString(kernelsResult[i]));
	 }

	//Reset counter of result elements to zero. 

	resultCounter = 0;
//------------------------------------------------------------------------------------------------------------------------------------//

  	return (TCL_OK);
  }


////////////////////////////////////////////////////////////////////////////--> Cubes function. Return cubes that contain specific literal.

Tcl_Obj *cubes (Tcl_Interp *interp, char **stringFunction, int numOfElementsFunction, int literalPosition,int isComplement  ) 
  {

  	Tcl_Obj *cubeList;
  	Tcl_Obj *cubeElement;

  	//Initialize list of cubes that are to be returned.

	cubeList = Tcl_NewListObj (0, NULL);

	//For each element of given function 

  	for (int i = 0; i < numOfElementsFunction; i++)
	  {

	  	//if isComplement == 0 and for the given position (literalPosition input) in cube's string, check if there is '01' for each cube. 

	  	if ( isComplement == 0 ) 
	  	 {
	  	 	if ( *( stringFunction[i] + literalPosition ) == '0' && *( stringFunction[i] + literalPosition + 1) == '1' )
			 {

			 	//if '01' value was found in cube's string value, append this cube to cubeList result.

				cubeElement = Tcl_NewObj();
				Tcl_AppendStringsToObj ( cubeElement, stringFunction[i], NULL );
				Tcl_ListObjAppendElement ( interp, cubeList, cubeElement );
			 }
	  	 }
	  	else //check if there is '10' for every cube as above. 
	  	 {
	  	 	if ( *( stringFunction[i] + literalPosition ) == '1' && *( stringFunction[i] + literalPosition + 1) == '0' )
			 {

			 	//if '10' value was found in cube's string value, append this cube to cubeList result.			 	

				cubeElement = Tcl_NewObj();
				Tcl_AppendStringsToObj ( cubeElement, stringFunction[i], NULL );
				Tcl_ListObjAppendElement ( interp, cubeList, cubeElement );
			 }
	  	 }	
	  }

	//return cube list that contain given literal. 

	return (cubeList);
  }


/////////////////////////////////////////////////////////////////////////////////////////////---> Main recursive kernels algorithm function

Tcl_Obj *r_kernels_algorithm (Tcl_Interp *interp, Tcl_Obj *objv, char **stringFunction, int numOfElementsFunction, int maxLength ) 
  {

	Tcl_Obj *cubeList;

	Tcl_Obj *kernelList;
	Tcl_Obj *kernel;

	Tcl_Obj **cubeAddr;
	int numOfCubes;		

	Tcl_Obj **kernelAddr;
	int numOfKernelElements;

	int isComplement;

	int timesEqual;

	char *stringCompareInitial;
	char *stringCompare;

	char *coKernel;
	char *stringKernel;

	char **stringKernelList;

	int found = 0;

	//Allocate memory for necessary string arrays that are needed for comparisons below 

	stringCompareInitial = (char *) malloc (maxLength + 1);
	stringCompare        = (char *) malloc (maxLength + 1);
	stringKernel         = (char *) malloc (maxLength + 1);

	//Allocate memory for string array of coKernel.  

	coKernel = (char *) malloc (maxLength + 1);

	//Set '\0' for the last element because coKernel will be constructed character by character 

	*(coKernel + maxLength) = '\0'; 

	//for each literal a,b,c,d... and after that for each literal a',b',c',d'...

	for (isComplement = 0; isComplement < 2; isComplement++) 
	  {

	  	//for each literal a,b,c,d... or for each a',b',c',d'... (depends on isComplement Value)

		for (int i = 0; i < maxLength - 1; i = i + 2)
		  {

		  	//call function cubes and find all cubes that contain literal i 

		  	cubeList = cubes (interp, stringFunction, numOfElementsFunction, i, isComplement);

		  	//extract address and number of total cubes from cubes function result

		  	Tcl_ListObjGetElements ( interp, cubeList, &numOfCubes, &cubeAddr );

		  	//find kernels only when cubes function result returns 2 or more than 2 cubes 

		  	if ( numOfCubes >= 2 ) 
		  	 {	

		  	 	//-------------------------------------- Calculate co - Kernel --------------------------------------------------------------------------//

		  	 	//stringCompareInitial contains first cube's value among of all the cubes that cubes-function returned. 

			  	strcpy(stringCompareInitial, Tcl_GetString(*(cubeAddr + 0)));

			  	//to extract largest cube among the returned cubes, check which literals they have in common. 

			  	for (int j = 0; j < maxLength - 1; j = j + 2)
			  	 {
			  	 	timesEqual = 0;

			  	 	//compare every cube with first cube's value (stringCompareInitial) for each literal 

			  		for (int l = 1; l < numOfCubes; l++)
			  		 {

			  		 	//extract every cube's value (except the first one) to stringCompare array 

			  			strcpy(stringCompare, Tcl_GetString(*(cubeAddr + l)));

			  			//each time a cube has the same value with first cube, increment timesEqual counter.

			  			if ( ( *(stringCompare + j) == *(stringCompareInitial + j) ) && ( *(stringCompare + j + 1) == *(stringCompareInitial + j + 1) ) ) 
			  			 {
			  			 	timesEqual ++;
			  			 }
			  		 }

			  		//if cubes has the same value for literal j, then timesEqual will have value equal to number of returned cubes minus 1 (first cube). 

			  		if ( timesEqual == numOfCubes - 1 )
			  		 {

			  			//create largest cube or coKernel by adding common literal in position j (cubic notation) 

						*( coKernel + j ) = *( stringCompareInitial + j );
						*( coKernel + j + 1 ) = *( stringCompareInitial + j + 1 );
			  		 }
			  		else
			  	     {

			  	     	//evey time a literal is not common to all returned cubes, add "11" in position j of co Kernel's value.

			  	     	*( coKernel + j  ) = '1';
						*( coKernel + j + 1 ) = '1';
			  	     }
			  	 }

			  	//---------------------------------------------------------------------------------------------------------------------------------------//
			    //-------------------------------------- Calculate kernel as list of cubes---------------------------------------------------------------//

			  	//Initialize list of Kernels.

			  	kernelList = Tcl_NewListObj (0, NULL);

			  	//kernels List will obiously have same number of elements as returned cubes but without coKernel vaule in each of them

			  	for (int j = 0; j < numOfCubes; j++)
			  	 {

			  	 	//at first, for each kernel (in string format) add value of returned cube.  

			  	 	strcpy(stringKernel,Tcl_GetString(*(cubeAddr + j)));

			  		//To remove coKernel value, add "11" every time a kernel literal is common to coKernel 

			  		for (int l = 0; l < maxLength - 1; l = l + 2)
			  		{
			  			if ( ( *( stringKernel + l ) == *( coKernel + l ) ) && ( *( stringKernel + l + 1 ) == *( coKernel + l + 1 ) ) )
			  			 {
			  				*( stringKernel + l ) = '1';
			  				*( stringKernel + l + 1 ) = '1';
			  			 }
			  		}

					//Append kernel element to kernel List

			  		kernel = Tcl_NewObj();
					Tcl_AppendStringsToObj ( kernel, stringKernel, NULL );
					Tcl_ListObjAppendElement ( interp, kernelList, kernel );
			  	 }

			  	//extract kernel list address and number of elements from compelete kernel list

			  	Tcl_ListObjGetElements ( interp, kernelList, &numOfKernelElements, &kernelAddr );

			  	//allocate memory for stringKernelList. stringKernelList will contain kernel list elements as strings 

			  	stringKernelList = malloc (sizeof (char *) * numOfKernelElements);

			  	//add string value of each kernel element to stringKernelList

			  	for (int j = 0; j < numOfKernelElements; j++)
			  	 {
			  	 	stringKernelList[j] = (char *) malloc (maxLength + 1);
			  	 	strcpy(stringKernelList[j], Tcl_GetString(*(kernelAddr + j)));
			  	 }

				//---------------------------------------------------------------------------------------------------------------------------------------//
			  	//-------------------------------------- Recursive process and final result manipulation-------------------------------------------------//
			  	
			  	//Begin recursive process

			  	r_kernels_algorithm (interp, kernelList, stringKernelList, numOfKernelElements, maxLength);

			  	found = 0;

			  	//if current list of kernels was added previously to final result, do nothing

			  	for (int j = 0; j < resultCounter; j++)
				 {
					if (strcmp(Tcl_GetString(kernelList), Tcl_GetString(kernelsResult[j])) == 0)
					{
						found = 1;
					}
				 }

				//add current list of cubes to final Kernel result only if it is new to final result 

				if ( found == 0 ) 
				 {

				 	//Final Result: increment counter of elements, allocate extra space in memory and initialize the list for element to be added.  

				  	resultCounter++;
					kernelsResult = realloc (kernelsResult, sizeof (Tcl_Obj *)*(resultCounter));
					kernelsResult[resultCounter - 1] = Tcl_NewListObj (0, NULL);

					//append list of kernels to final result

				   	Tcl_ListObjAppendList(interp, kernelsResult[resultCounter - 1], kernelList);
				 }  

				//---------------------------------------------------------------------------------------------------------------------------------------// 	  
			 }

		  }
	  }

	//free unnecessary dynamically allocated memory

	free(coKernel);
	free(stringKernel);
	free(stringCompare);
	free(stringCompareInitial);

	//return input function

	return (objv);
  }