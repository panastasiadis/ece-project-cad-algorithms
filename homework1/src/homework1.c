/* Inclusion of the necessary libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl8.6/tcl.h> 
#include <readline/readline.h>
#include <readline/history.h>

#define LINE_MAX 250


/* Array including the strings of all Tcl built-in commands from Tcl8.6, plus the custom commands of ls and less */ 
char *tcl_commands[] = {
"after", "error", "lappend", "platform", "tcl_findLibrary",
"append", "eval", "lassign", "platform::shell",	"tcl_startOfNextWord",
"apply", "exec", "lindex"	, "proc"	,"tcl_startOfPreviousWord", 
"array", "exit",	"linsert"	,"puts", "tcl_wordBreakAfter"
"auto_execok" ,	"expr",	"list",	"pwd" , "tcl_wordBreakBefore",
"auto_import"	,"fblocked",	"llength"	,"re_syntax"	,"tcltest"
"auto_load", "fconfigure",	"load",	"read"	,"tclvars",
"auto_mkindex" , "fcopy", "lrange", "refchan", "tell",
"auto_mkindex_old", "file", "lrepeat", "regexp", "time",
"auto_qualify" , "fileevent", "lreplace", "registry", "tm",
"auto_reset", "filename", "lreverse", "regsub", "trace",
"bgerror", "flush", "lsearch", "rename", "unknown",
"binary", "for", "lset", "return", "unload",
"break", "foreach", "lsort", "unset",
"catch", "format", "mathfunc", "scan", "update",
"cd", "gets", "mathop", "seek", "uplevel",
"chan", "glob", "memory", "set", "upvar",
"clock", "global", "msgcat", "socket", "variable",
"close", "history", "namespace", "source", "vwait",
"concat", "http", "open", "split", "while",
"continue", "if", "package", "string",
"dde", "incr",	"parray", "subst",
"dict",	"info",	"pid",	"switch",
"encoding",	"interp", "pkg::create", "Tcl",
"eof",	"join",	"pkg_mkIndex",	"tcl_endOfWord", "ls","less", NULL };

Tcl_Interp *interp; /* Global declaration of the interp variable which is necessary for the Tcl Interpeter and the Tcl C API */  

/* Function responsible for searching user's typed word 
   inside the array and comparing it with the Tcl Commands 
   in order to find the best match and return it */
char *text_generator(const char *text, int state)
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

/* Custom completer's function */
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

/*  Structure of the custom tcl command "ls" */
static int custom_command_ls (ClientData dummy,                /* Not used. */
    						  Tcl_Interp *interp,              /* Current interpreter. */
    						  int objc,                        /* Number of arguments. */
                              Tcl_Obj *const objv[]) 
{

	int i;
	int check;
	char ls_plus_arguments[LINE_MAX];

	strcpy (ls_plus_arguments,"ls "); /*initialize array ls_plus_arguments with command  "ls " */ 

	if (objc == 1) 	
	{

	/* if no argument is specified next to "ls" 
	   command just call the system function using string "ls" */	

       system("ls");
       return TCL_OK;
    }
    else 
    {

	/* if there are arguments next to command "ls" (for example -a or -l) do the following for all arguments:
	   1.Set current object of Tcl, argument obj[i]
	   2.Put argument inside the the string ls_plus_arguments next to "ls " 
	     or next to last argument from right side  (like "ls -a ")
	   3.Put " " next to last argument
	   4.Pass the string to system function */

		for(i = 1;i < objc;i++) 	
		{
			Tcl_SetObjResult(interp, objv[i]);
			strcat (ls_plus_arguments, Tcl_GetStringResult(interp) );      
			strcat(ls_plus_arguments," ");
	    }

		Tcl_ResetResult (interp);
		check = system(ls_plus_arguments);
		
		if (check == - 1) 	/*Check if system function failed */
		{
			return TCL_ERROR;
		}

	}
		return TCL_OK;
}


/* Structure of custom Tcl command "less". Same as custom "ls" */
static int custom_command_less (ClientData dummy,                /* Not used. */
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

/* Function including the creations of the custom tcl commands ls,less */	
int ExtendTcl (Tcl_Interp *interp, char * command) 
{

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
	rl_completion_entry_function = NULL; //use rl filename_completion_function(), the default filename completer
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

	/* While loop with functionality to maintain the Tcl Shell with the readline capabilities
	  and accept commands from user */
	while(1)
	{

		text = readline("PR> ");

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
			Tcl_Finalize();	 /*When command is "quit" Tcl has to be finalized with Tcl_Finalize Function */
			return EXIT_SUCCESS;
		}
		else if (strcmp(command, "history") == 0)
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

			/*Print the result of Tcl in my shell */
			strcpy (result,Tcl_GetStringResult (interp));
			printf("%s\n", result );
			
			
		}
	}
	return 0;
}




