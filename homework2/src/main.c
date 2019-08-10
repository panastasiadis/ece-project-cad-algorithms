//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

//Headers Declaration

#include "header_files/libraries.h"
#include "header_files/tcl_commands.h"

/* Global declaration of the interp variable which is necessary for the Tcl Interpeter and the Tcl C API */ 

Tcl_Interp *interp;  

/* Functions custom completer and text generator are responsible for searching user's typed word 
   inside the array and comparing it with the Tcl Commands 
   in order to find the best match and return it */

/* Custom completer's function */

char *text_generator( const char *text, int state )
{
    static int list_index, len;
    char *name;
    if (!state) 
    {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = tcl_commands[list_index++])) 
    {
        if (strncmp(name, text, len) == 0) 
        {
            return strdup(name);
        }
    }

    return NULL;
}


char ** custom_completer (const char *text, int start, int end) 
{
    char **matches;
	matches = (char **)NULL;

	/* If this word is at the start of the line, then it is a command
	     to complete.  Otherwise it is the name of a file in the current
	     directory. */

	if (start == 0) 
	{
	    matches = rl_completion_matches (text, text_generator);
	}

  	return (matches);
}

/* Function including the creations of the custom tcl commands ls,less and the cube operations */	

int ExtendTcl (Tcl_Interp *interp, char * command) 
{	

	if (Tcl_CreateObjCommand( interp, "OFF_f", custom_OFF_f, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }

	if (Tcl_CreateObjCommand( interp, "sharp", custom_sharp, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }

	if (Tcl_CreateObjCommand( interp, "sharp_2", custom_sharp_2, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }

	if (Tcl_CreateObjCommand( interp, "cube_cover_2", custom_cube_cover_2, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }

	if (Tcl_CreateObjCommand( interp, "distance_2", custom_distance_2, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }

	if (Tcl_CreateObjCommand( interp, "supercube_2", custom_supercube_2, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }

	if (Tcl_CreateObjCommand( interp, "cube_intersect_2", custom_cube_intersect_2, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }

    if (Tcl_CreateObjCommand( interp, "ls", custom_command_ls, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }

    if (Tcl_CreateObjCommand( interp, "less", custom_command_less, NULL, NULL) == NULL) 
    {
        return TCL_ERROR;
    }  

    return TCL_OK;
}


int main (int argc, char *argv[]) 
{

	char *text = NULL;   		    //readline result
	char *textexpansion; 		    //readline result history expanded
	int expansionresult;
	HIST_ENTRY **the_history_list;  //readline commands history list - NULL terminated
	char command[LINE_MAX];         //current command
	unsigned long i;

	char result[LINE_MAX];

	//Readline Initialisation

	//use rl filename_completion_function(), the default filename completer

	rl_completion_entry_function = NULL; 
	rl_attempted_completion_function = custom_completer;
	rl_completion_append_character = '\0';

	using_history(); //initialise history functions

	//Tcl Initialisation

	Tcl_FindExecutable(argv[0]);
    interp = Tcl_CreateInterp();

    //Check for errors in Tcl_CreateInterp function

    if (interp == NULL) 
    {
		fprintf(stderr,
		"Error in Tcl_CreateInterp, aborting\n");
		exit(1);
	}

	//Run Tcl_Init function and check for errors 

    if (Tcl_Init(interp) != TCL_OK) 
    {
	        return EXIT_FAILURE;
	}

	//Run ExtendTcl function to initialise the creation of the custom commands ls,less
	//Also Check for errors 

	if (ExtendTcl(interp, command) != TCL_OK) 
	{
	        fprintf(stderr ,"Tcl_Init error: %s\n" ,Tcl_GetStringResult(interp));
	        exit(EXIT_FAILURE);
	}

	// Display following text when app starts.

	printf("////////////////////////////////////////////////////////////\n");
	printf("University      : Electrical & Computer Engineering\n");
	printf("Course          : CE437 - Computer-Aided Design Algorithms\n" );
	printf("Full Name       : Panagiotis Anastasiadis\n");
	printf("Registry Number : 2134\n");
	printf("////////////////////////////////////////////////////////////\n\n\n");
	printf("		             ***\n");
	printf("                           PROJECT\n");
	printf("       {CAD Algorithms Implementation On A TCL Shell}\n");  
	printf("		             ***\n\n\n");
	printf("////////////////////////////////////////////////////////////\n\n");
	printf("Starting...\n\n");

	/* While loop with functionality to maintain the Tcl Shell with the readline capabilities
	  and accept commands from user */

	while(1)
	{

		text = readline("Enter command> ");

		/* Readline's history mode for every command that has been previously typed from user */

		if (text != NULL)
		{

			expansionresult = history_expand(text, &textexpansion);
			if( (expansionresult == 0) || (expansionresult ==2)) //no expansion or do not execute
			{
				add_history(text);
				strcpy(command, text); //store command
			}
			else
			{
				add_history(textexpansion);
				strcpy(command, textexpansion); //store command
			}

			free(textexpansion);
			free(text);
		}

		//Handle commands: history and quit

		if(strcmp(command, "quit") == 0)
		{
			printf("\nQuiting...\n\n");
			Tcl_Finalize();	 /*When command is "quit" Tcl has to be finalized with Tcl_Finalize Function */
			return EXIT_SUCCESS;
		}
		else if ( strncmp(command, "history", 7 ) == 0 )
		{
			the_history_list = history_list(); //get history list
			if(the_history_list != NULL)
			{
				i = 0;
				while ( *(the_history_list + i) != NULL ) //history list - NULL terminated
				{
					printf("%ld: %s\n", (i + history_base), ( *(the_history_list + i))->line );
					i++;
				}
			}

		}
		else 
		{
			/* User's command passes as argument in Tcl_Eval function in order to be executed as tcl command */

				Tcl_Eval(interp, command);

				/*Print the result of Tcl in this shell */

				strcpy (result,Tcl_GetStringResult (interp));
				printf("%s\n", result );
		}
	}
	return 0;
}




