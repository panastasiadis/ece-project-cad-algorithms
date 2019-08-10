//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#include "header_files/libraries.h"

int custom_alg_division (ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) 
  {

  	const char allowed_characters[] = "01";

  	Tcl_Obj **List1Addr;
  	int numOfElementsList1;

  	Tcl_Obj **List2Addr;
  	int numOfElementsList2;

  	Tcl_Obj **listAddrV;
	int numOfElementsV;

	Tcl_Obj **listAddrV_tmp;
	int numOfElementsV_tmp;

	Tcl_Obj**listAddrPQ;
    int numOfElementsPQ;

    Tcl_Obj *remainderList;
    Tcl_Obj *elementRemainder;

    Tcl_Obj **listAddrRemainder;
    int numOfElementsRemainder;

    Tcl_Obj **quotientListAddr;
    int numOfQuotientElements;

    Tcl_Obj *listPQ;
	Tcl_Obj *elementPQ;

    Tcl_Obj *quotientList;
    Tcl_Obj *quotientElement;

  	Tcl_Obj **V;
  	Tcl_Obj *elementV;

	char *stringQuotient;
	char *stringElementPQ;

	char *stringCompare1;
    char *stringCompare2;

    int found;
    int timesFound = 0;
    int foundInPQ;  
    int exitComparison;

  	char *stringElementV;

  	char *stringCube;
  	int cubeLength;

  	char **dividend;
  	char **divisor;

  	int len;
  	int maxLengthList1 = -1;
  	int maxLengthList2 = -1;

  	int longestString;
  	int diff;

  	int missingLiteral;

  	int k;

  	//Check number of arguments. User can specify only two functions-cube lists.

  	if (objc != 3)
	 {
	   	printf("Wrong Number of Arguments! For Cube List Use Format { f1 f2 .. fn }.");
	    return TCL_ERROR;
	 }

//----------------------------------Check arguments for syntax errors-------------------------------------------------------------------//

	//Extract elements of argument 1 and argument 2

	Tcl_ListObjGetElements ( interp, objv[1], &numOfElementsList1, &List1Addr ); 
	Tcl_ListObjGetElements ( interp, objv[2], &numOfElementsList2, &List2Addr );

	//Check first cube list (dividend) for syntax errors

	for (int i = 0; i < numOfElementsList1; i++)
	  {

	  	//Extract string values of each cube in dividend

		stringCube = Tcl_GetStringFromObj (*( List1Addr + i ), &cubeLength);

		//Save the length of the longest string element of function.

		if ( cubeLength > maxLengthList1 )
		 {
			maxLengthList1 = cubeLength;
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

	//Check second cube list (divisor) for syntax errors

	for (int i = 0; i < numOfElementsList2; i++)
	  {
		stringCube = Tcl_GetStringFromObj (*( List2Addr + i ), &cubeLength);

		//Save the length of the longest string element of function.

		if ( cubeLength > maxLengthList2 )
		 {
			maxLengthList2 = cubeLength;
		 }

		len = strspn ( stringCube, allowed_characters );

		if ( (len != cubeLength) ||  (cubeLength % 2 != 0 ) ) 
		 {
			printf ("Wrong Type Of Argument! Only Positional Cube Notation Is Accepted!");
			return TCL_ERROR;
		 }

		for (int j = 0; j < cubeLength - 1; j = j + 2 )
		  { 
			if ( *( stringCube + j ) == '0' && *( stringCube + j + 1) == '0' )
			 {
				printf("Invalid Cube! Found One Or More '00' Fields In The Encoding.");
				return TCL_ERROR;
			 }
		  }	
	  }	

	//Allocate space for divisor and dividend

	dividend = malloc (sizeof (char *) * numOfElementsList1);
	divisor  = malloc (sizeof (char *) * numOfElementsList2);

	//check malloc for errors.

	if ( dividend == NULL || divisor == NULL )
	 { 
		printf ("Failed To Allocate Memory!");
		return TCL_ERROR;
	 }

	//Calculate max cube length of both lists

	if ( maxLengthList1 > maxLengthList2 ) 
	 {
	 	longestString = maxLengthList1;
	 }
	else 
	 {
	 	longestString = maxLengthList2;
	 }

	//Make size of all cubes in dividend-list equal to longest string

	for (int i = 0; i < numOfElementsList1; i++)
	  {
		dividend[i] = (char *) malloc (longestString + 1);

		//check malloc for errors

		if (dividend[i] == NULL)
		 {
			printf ("Failed To Allocate Memory!");
			return TCL_ERROR;
		 }

		strcpy( dividend[i], Tcl_GetStringFromObj (*(List1Addr + i), &cubeLength));

		diff = longestString - cubeLength;

		//Add "11" to increase size 

		for (int j = 0; j < diff; j++)
		  {
			strcat(dividend[i], "1");
		  }
	  }

	//Make size of all cubes in divisor-list equal to longest string

	for (int i = 0; i < numOfElementsList2; i++)
	  {
		divisor[i] = (char *) malloc (longestString + 1);

		//check malloc for errors

		if (divisor[i] == NULL)
		 {
			printf ("Failed To Allocate Memory!");
			return TCL_ERROR;
		 }

		strcpy( divisor[i], Tcl_GetStringFromObj (*(List2Addr + i), &cubeLength));

		diff = longestString - cubeLength;

		//Add "11" to increase size 

		for (int j = 0; j < diff; j++)
		  {
			strcat(divisor[i], "1");
		  }
	  }
//------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------Calculation of V----------------------------------------------------------------------------------//

	//Allocate memory for V List

	V = malloc (sizeof (Tcl_Obj *) * 1);

	//Allocate memory for string element of V

	stringElementV = (char *) malloc (longestString + 1);

	//Calculate Algebraic-Weak Division Algorithm

	for (int i = 0; i < numOfElementsList2; i++)
	  {

	  	//Initialize list for each Vi

		V[i] = Tcl_NewListObj (0, NULL);

		//For each element in dividend find those that contains all literals of each divisor element

		for (int j = 0; j < numOfElementsList1; j++)
		  {
			missingLiteral = 0;
			k = 0;


		  	while ( ( k < longestString - 1 ) &&  ( missingLiteral == 0 ) )
		      { 

		      	//Ignore elements "11" of divisor 

				if ( *( divisor[i] + k ) =='1' && *( divisor[i] + k + 1) == '1' )
			     {
			     	k  = k + 2;
			   	 }
			   	else 
			   	 {
			   	 	//If divisor element exists in dividend element proceed to next comparison, otherwise raise signal missing to 1

			   	 	if ( ( *( dividend[j] + k ) == *( divisor[i] + k ) ) && ( *( dividend[j] + k + 1 ) == *( divisor[i] + k + 1) ) ) 
					 {
					 	k = k + 2;
					 }
					else 
					 {
					 	missingLiteral = 1;
					 }
			   	 }
		  	  }

		  	//if no missing literal was found, add element of dividend to V list without the divisor literals.

		  	if ( missingLiteral == 0 ) 
		  	 {
		  	 	//make dividend element, element of V

		  	 	strcpy ( stringElementV, dividend[j] );

		  	 	//Delete literals of P by adding "11"

		  	 	for ( int k = 0; k < longestString - 1; k = k + 2 )
		  	 	  {
					if ( ( *( dividend[j] + k ) == *( divisor[i] + k ) ) && ( *( dividend[j] + k + 1 ) == *( divisor[i] + k + 1) ) ) 
					 {
					 	*( stringElementV + k ) = '1';
					 	*( stringElementV + k + 1 ) = '1';
					 }	
		  	 	  }

		  	 	//Add new V element to Vi list

		  	 	elementV = Tcl_NewObj();
				Tcl_AppendStringsToObj ( elementV, stringElementV, NULL );
				Tcl_ListObjAppendElement ( interp, V[i], elementV );
		  	 }	
	      }

	    //Increase size of V by allocating extra space

	    V = realloc (V, sizeof (Tcl_Obj *) * (i + 2));
	  }	

//------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------Calculation of Quotient------------------------------------------------------------------//

    quotientList = Tcl_NewListObj (0, NULL);

    //Allocate memory for 2 string arrays

    stringCompare1 = (char *) malloc (longestString + 1);
    stringCompare2 = (char *) malloc (longestString + 1);

    //Find quotient by calculating intersection of all V list elements 

    for (int i = 0; i < numOfElementsList2; i++)
	  {

	  	//for each V element extract cubes

	  	Tcl_ListObjGetElements ( interp, V[i], &numOfElementsV, &listAddrV ); 

		for (int j = 0; j < numOfElementsV; j++)
		  {
		  	
		  	//add each cube of V to stringCompare1 string

		  	strcpy(stringCompare1, Tcl_GetStringFromObj (*( listAddrV + j ), NULL)); 

		  	Tcl_ListObjGetElements ( interp, quotientList, &numOfQuotientElements, &quotientListAddr );

		  	exitComparison = 0;

		  	//if cube exists in quotient list, no action needed. Ignore (exitComparison = 1) next loops

		  	for (int k = 0; k < numOfQuotientElements; k++)
		  	  {
				strcpy(stringCompare2, Tcl_GetStringFromObj (*( quotientListAddr + k ), NULL));
	      		
	      		if ( strcmp ( stringCompare1, stringCompare2 ) == 0  )
	      		 {
	      			exitComparison = 1;
	      		 }
		  	  }

		  	k = 0;
		  	found = 0;
		  	timesFound = 0;

		  	//Search for cube - stringCompare1, in all others V elements  

			while ( k < numOfElementsList2 && exitComparison == 0 )
	 	      {

	 	      	if ( i != k ) //Ignore searching at V element that current cube belongs to. 
	 	      	 {
		 	      	Tcl_ListObjGetElements ( interp, V[k], &numOfElementsV_tmp, &listAddrV_tmp );
		 	      	found = 0;

		 	      	//search all cubes of the other V elements

		 	      	for (int l = 0; l < numOfElementsV_tmp; l++)
		 	      	  {
		 	      		strcpy(stringCompare2, Tcl_GetStringFromObj (*( listAddrV_tmp + l ), NULL));

		 	      		//if cube exists in other V element mark it as found

		 	      		if ( strcmp ( stringCompare1, stringCompare2 ) == 0  )
		 	      		 {
		 	      			found = 1;
		 	      		 }
		 	      	  }

		 	      	//for each found marks increment timesCounter, otherwise raise exitComparison to 1 
		 	      	//in order to exit searching and proceed to next cube

		 	      	if ( found == 1 ) 
		 	      	 {
		 	      	 	timesFound++;
		 	      	 }
		 	      	else 
		 	      	 {
		 	      	 	exitComparison = 1;
		 	      	 }
		 	     }

		 	    k++;
		 	  } 

		 	//if timesFound is equal to number of V elements (equal to divisor number of elements),
		 	//minus 1 (the V element of current cube), add cube to quotient result

	 	    if ( timesFound == numOfElementsList2 - 1 ) 
	 	     {
	 	     	quotientElement = Tcl_NewStringObj(stringCompare1, strlen(stringCompare1) + 1);
	 	     	Tcl_ListObjAppendElement ( interp, quotientList, quotientElement );
	 	     }
		  }
      }

//------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------Calculation of P*Q-----------------------------------------------------------------------//

    Tcl_ListObjGetElements ( interp, quotientList, &numOfQuotientElements, &quotientListAddr );

	//Initialize list for P*Q product

	listPQ = Tcl_NewListObj (0,NULL);

	//Allocate memory for necessary string arrays, stringElementPQ and stringQuotient

	stringElementPQ = (char *) malloc (longestString + 1);
	stringQuotient  = (char *) malloc (longestString + 1);

	//Multiplty each element of divisor with each element of Quotient

	for (int i = 0; i < numOfElementsList2; i++)
	  {

		for (int j = 0; j < numOfQuotientElements; j++)
	      {

	      	strcpy(stringQuotient, Tcl_GetStringFromObj (*(quotientListAddr + j), NULL)); 

	      	//Calculate P*Q product,via bitwise AND

	      	for ( int l = 0; l < longestString + 1; l++ )
			  {
		 
		  		if ( ( *(divisor[i] + l) == '0' ) || ( *(stringQuotient + l) == '0' ) )
		  		 {
			
					strcpy(stringElementPQ + l , "0"); 

		    	 }
		  		else if (( *(divisor[i] + l)== '1' ) && ( *(stringQuotient + l) == '1' ) )
		         {
		  	
		  			strcpy(stringElementPQ + l, "1"); 
		  		 }
		  	  }

		  	//Add calculated element (stringElementPQ) to PQ list

		  	elementPQ = Tcl_NewObj();
			Tcl_AppendStringsToObj ( elementPQ, stringElementPQ, NULL );
			Tcl_ListObjAppendElement ( interp, listPQ, elementPQ );
	  	  }
	  }	  	
//------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Calculation of Remainder-------------------------------------------------------------//


    Tcl_ListObjGetElements ( interp, listPQ, &numOfElementsPQ, &listAddrPQ );

    //Initialize list of remainder result

    remainderList = Tcl_NewListObj (0,NULL);

    //Calculate R = Dividend Elements - P*Q

    for (int i = 0; i < numOfElementsList1; i++)
      {

      	foundInPQ = 0;

      	//search each element of dividend list

    	for (int j = 0; j < numOfElementsPQ; j++)
	      {

	      	//if element in dividend list exists in P*Q product mark it as found

	      	if ( strcmp (dividend[i], Tcl_GetStringFromObj (*(listAddrPQ + j), NULL) ) == 0 )
	      	 {
	      		foundInPQ = 1;
	      	 }
	      }

	    //if element of dividend was not found in P*Q, belongs to Remainder List

	    if ( foundInPQ == 0 ) 
	     {

	     	//Add element to Remainder List 

	     	elementRemainder = Tcl_NewObj ();
	     	Tcl_AppendStringsToObj ( elementRemainder, dividend[i], NULL );
	     	Tcl_ListObjAppendElement ( interp, remainderList, elementRemainder );

	     } 
      }

//------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------Display Results------------------------------------------------------------------------//
   	
   	//Print Results: Quotient and Remainder

    Tcl_ListObjGetElements ( interp, quotientList, &numOfQuotientElements, &quotientListAddr );

    //Print Quotient List

    printf("\nQuotient  (Q):");

	for (int i = 0; i < numOfQuotientElements; ++i)
	  {
	  	printf( " %s", Tcl_GetStringFromObj ( *(quotientListAddr + i), NULL ) );
	  }
 
	printf("\n");

	//no Result case for quotient

	if ( numOfQuotientElements == 0 )
	 {
		printf(" Νο Result");
	 }

    Tcl_ListObjGetElements ( interp, remainderList, &numOfElementsRemainder, &listAddrRemainder );

    //Print Remainder List

    printf("Remainder (R):");

    for (int i = 0; i < numOfElementsRemainder; i++)
      {
    	printf(" %s", Tcl_GetStringFromObj (*(listAddrRemainder + i), NULL));
      }

    //no Result case for remainder

   	if ( numOfElementsRemainder == 0 )
	 {
		printf(" Νο Result");
	 }

    printf("\n");

//------------------------------------------------------------------------------------------------------------------------------------//

    //free unnecessary dynamically allocated memory

	free(dividend);
	free(divisor);
	free(stringCompare1);
	free(stringCompare2);
	free(V);
	free(stringElementV);
	free(stringElementPQ);
	free(stringQuotient);

	return TCL_OK;
  }
