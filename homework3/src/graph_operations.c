//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

#include "header_files/libraries.h"

//Global variables needed to be accessed by read,write,draw graph functions and function that finds critical path 

Tcl_Obj *NodesList;
Tcl_Obj **ConnectionsList;
Tcl_Obj **WeightsList;

//When a graph is stored successfully to Shell, graph_ok should be 1 to indicate that other functions can use safely the above lists. Default value is set to 0. 

int graph_ok = 0;

//Function extract_GraphLine_Elements attempts to extract values of node, node successor and weight.

int extract_GraphLine_Elements ( char *new_line , int *node, int *nodeConnectedTo, int *weight  )
  {

 	char nodeChar = 'n';
 	char weightchar = 'w';
 	char arrowSymbol[] = "->";

 	char *first_n;
 	char *second_n;
 	char *weight_w;
 	char *checkForArrow;

    //Check if graph file has correct format by attempted to find declaration of first node, specifically first char "n"

	first_n = strchr (new_line, nodeChar);

	if (first_n == NULL)
	 {
		printf("Found One Or More Errors In Graph File!");
		return 1;
	 }

	//Check after a character "n" is found, if there is a valid number next to it  

	*node = atoi (first_n + 1);

	if ( (( *node == 0 ) && ( *(first_n + 1) != '0' )) || *node < 0 )
	 {
		printf("Found One Or More Errors In Graph File!");
		return 2;
	 }

	//Check the existance of symbol '->'

	checkForArrow = strstr (first_n + 1, arrowSymbol);

	if (checkForArrow == NULL)
	 {
		printf("Found One Or More Errors In Graph File!");
		return 3;
	 }

    //Check if graph file has correct format by attempted to find declaration of second node, specifically second char "n" in line.

	second_n = strchr (first_n + 1, nodeChar);

	if (second_n == NULL)
	 {
		printf("Found One Or More Errors In Graph File!");
		return 4;
	 }

	//Check for valic number next to character n.

	*nodeConnectedTo = atoi (second_n + 1);

	if ( ( ( *nodeConnectedTo == 0 ) && ( *( second_n + 1 ) != '0' ) ) || ( *nodeConnectedTo < 0 ) || ( *node == *nodeConnectedTo ) )
	 {
		printf("Found One Or More Errors In Graph File!");
		return 5;
	 }

	//search for symbol "w" in file line, to find weight value.

	weight_w  = strchr (second_n, weightchar);

	if (weight_w == NULL)
	 {
		printf("Found One Or More Errors In Graph File!");
		return 6;
	 }

	//check for a valid value next to character w.

	*weight = atoi (weight_w + 1);

	if ( (( *weight == 0 ) && ( *(weight_w + 1) != '0' )) || *weight < 0 ) 
	 {
		printf("Found One Or More Errors In Graph File!");
		return 7;
	 }

	return 0;
  }


//Function read_graph loads to Shell a new graph, if process ends with success or returns Tcl_error  

int custom_read_graph ( ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[] )         
  {

  	ClientData graph_channel;

  	Tcl_Obj *objptr;
 	Tcl_Obj *nodeObj;
 	Tcl_Obj *connectionObj;
 	Tcl_Obj *weightObj;

 	Tcl_Obj **NodesListAddr;

 	char *new_line;
 	int line_length;
 	
 	int weight;
 	int node;
 	int nodeConnectedTo;
 	int errorInGraph;
 	int numberOfNodes = 0;
 	int existingNode;
 	int foundNodeFlag;
 	int foundNodeFlag2;
 	int connection_counter = 0;
 	int node_index = 0;
	int counter = 0;

	char *file_name;

	//Check number of arguments. Specify exactly ONE graph.

	if (objc != 2)
	 {
		printf("Wrong Number of Arguments!");
		return TCL_ERROR;
	 }

	//Attempt to open file in order to be inserted into a TCL channel.

	file_name = Tcl_GetStringFromObj(objv[1], NULL );
  	graph_channel = Tcl_OpenFileChannel (interp, file_name, "RDONLY", 777); 

  	//If this process fails exit with TCL_ERROR

  	if ( graph_channel == NULL )
	 {
		return TCL_ERROR;
	 }

	//Allocate memory for List of Nodes 

 	NodesList =  Tcl_NewListObj (0,NULL);

 	//Allocate memory for list of connections for all nodes

 	ConnectionsList = malloc (sizeof (Tcl_Obj *) * 1);

 	//Check for errors in malloc

 	if ( ConnectionsList == NULL ) 
 	 {
 	 	printf ("Failed To Allocate Memory!\n");
		return TCL_ERROR;
 	 }

 	//Initialize list of connections - successors for first node

 	ConnectionsList[connection_counter] = Tcl_NewListObj (0, NULL);

 	//Allocate memory for list of weights 

 	WeightsList = malloc (sizeof (Tcl_Obj *) * 1);

 	//Check for errors in malloc

 	if ( WeightsList == NULL ) 
 	 {
 	 	printf ("Failed To Allocate Memory!\n");
		return TCL_ERROR;
 	 }

 	//Initialize list of weights of connections for first node

 	WeightsList[connection_counter] = Tcl_NewListObj (0, NULL);
 	
 	//Set access point of given file to starting point

	Tcl_Seek(graph_channel, 0, SEEK_SET);

	//Read first line of file and save to objptr

	objptr = Tcl_NewObj();
	Tcl_GetsObj (graph_channel, objptr);

	//While Loop: Keep reading new lines before reaching EOF

	while ( Tcl_Eof(graph_channel)== 0 ) 
	   {

	   		//Convert line stored in objptr to string

	    	new_line = Tcl_GetStringFromObj(objptr, NULL);
	    	line_length = strlen (new_line);

	    	//Call function extract_GraphLine_Elements and check if line of graph line is valid

	   		errorInGraph = extract_GraphLine_Elements (new_line, &node , &nodeConnectedTo, &weight);

	   		//If there is an error in line, return TCL_ERROR and print error message 

	   		if (errorInGraph != 0 ) 
	   		 {
	   		 	graph_ok = 0;
	   		 	printf("Cannot Load to Shell The Given Graph File!" );
	   		 	return TCL_ERROR;
	   		 }

			////////////////////////////////////////////////////////////////////////////////////
	   		//From this point, manipulate valid line of file and save to memory nodes and weight

   		 	foundNodeFlag = 0;
   		 	foundNodeFlag2 = 0;

  
   		 	Tcl_ListObjGetElements ( interp, NodesList, &numberOfNodes, &NodesListAddr );
   		 	node_index = numberOfNodes;

   			//1.Search node list if new node or connection of this node already exists in list
   			//2.node_index indicates the node which we want to store its sucessor and weight of the edge

   		 	for (int i = 0; i < numberOfNodes; i++)
   		 	  {

				Tcl_GetIntFromObj(interp, *(NodesListAddr + i), &existingNode);

   		 	  	if ( existingNode == node ) 
   		 	  	 {
   		 	  	 	foundNodeFlag = 1; 	//If node before "->" exists in list, raise foundNodeFlag to 1
   		 	  	 	node_index = i;	
   		 	  	 }
   		 	  	if ( existingNode == nodeConnectedTo ) 
   		 	  	 {
   		 	  	 	foundNodeFlag2 = 1;  //If node after "->" exists in list, raise foundNodeFlag2 to 1
   		 	  	 }
   		 	  }
   		 	

   		 	if ( foundNodeFlag == 0 ) 
   		 	 {

   		 	 	//node - successor in current line is a new element,so we should store it to node list

   		 	 	nodeObj = Tcl_NewIntObj (node);
   		 	 	Tcl_ListObjAppendElement (interp, NodesList, nodeObj);

   		 	 	//node sucessor in current line is a new element,so we should reallocate list of connections and weights with one more element

   		 	 	connection_counter++;

   		 	 	ConnectionsList = realloc (ConnectionsList, sizeof (Tcl_Obj *)*(connection_counter + 1));

   		 	 	//Check for errors in realloc

   		 	 	if ( ConnectionsList == NULL ) 
 	 		     {
 	 				printf ("Failed To Allocate Memory!\n");
					return TCL_ERROR;
 	 			 }

 	 			//Initialize list of connections for node in position connection_counter

				ConnectionsList[connection_counter] = Tcl_NewListObj (0, NULL);

				WeightsList = realloc (WeightsList, sizeof (Tcl_Obj *)*(connection_counter + 1));

				//Check for errors in realloc

				if ( WeightsList == NULL ) 
 	 		     {
 	 				printf ("Failed To Allocate Memory!\n");
					return TCL_ERROR;
 	 			 }

 	 			//Initialize list of of weight of connection for node in position connection_counter

				WeightsList [connection_counter] = Tcl_NewListObj (0, NULL);
					
   		 	 }


   		 	if ( foundNodeFlag2 == 0 )
   		 	 {

   		 	 	//node - predecessor in current line is a new element,so we should store it to node list

   		 	 	nodeObj = Tcl_NewIntObj (nodeConnectedTo);
   		 	 	Tcl_ListObjAppendElement (interp, NodesList, nodeObj);

   		 	 	//node predecessor in current line is a new element,so we should reallocate list of connections and weights with one more element

   		 	 	connection_counter++;

   		 	 	ConnectionsList = realloc (ConnectionsList, sizeof (Tcl_Obj *) * (connection_counter+1));

   		 	 	if ( ConnectionsList == NULL ) 
 	 		     {
 	 				printf ("Failed To Allocate Memory!\n");
					return TCL_ERROR;
 	 			 }

 	 			//Initialize list of connections for node in position connection_counter 

				ConnectionsList[connection_counter] = Tcl_NewListObj (0, NULL);

				WeightsList = realloc (WeightsList, sizeof (Tcl_Obj *)*(connection_counter + 1));

				if ( WeightsList == NULL ) 
 	 		     {
 	 				printf ("Failed To Allocate Memory!\n");
					return TCL_ERROR;
 	 			 }

 	 			//Initialize list of of weight of connection for node in position connection_counter 

				WeightsList [connection_counter] = Tcl_NewListObj (0, NULL);
   		 	 }
	   		 
	   		//After correct memory allocation for both lists of connections and weights,
	   		//..store node-predecessor and weight in appropriate position of list ( node index )

	   		connectionObj = Tcl_NewIntObj (nodeConnectedTo);
			Tcl_ListObjAppendElement (interp, ConnectionsList[node_index], connectionObj);

			weightObj = Tcl_NewIntObj (weight);
			Tcl_ListObjAppendElement (interp, WeightsList[node_index], weightObj );


			//End of part that is tasked with allocation of nodes in memory
	    	///////////////////////////////////////////////////////////////////////////////////////

			//Move access point to next line

	    	Tcl_Seek(graph_channel, counter + line_length + 1, SEEK_SET);
			counter = counter + line_length + 1;

			objptr = Tcl_NewObj();
			Tcl_GetsObj (graph_channel, objptr);
   		 	 
		}

	//If graph was loaded to memory with no errors, print successful message and change global variable of graph ok to 1

	printf("Graph Was Loaded Successfully!");
	graph_ok = 1;
  	return TCL_OK;
  }

//Draw graph function draws loaded graph. It uses application graphiz for this operation by exexuting command dotty.

int custom_draw_graph ( ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[] )         
  {
	
	int check;
	int node;
	int numberOfConnections;
	int connection;

	int numberOfWeights;
	int weight;
	int numberOfNodes;

	char arrowSymbol[] = " -> ";
	char dotty_starting_string [] = "digraph {\nnode [fillcolor=\"lightgray\",\nstyle=\"filled,solid\"\nfontsize=10,\nfontcolor=\"red\"]\n"; //format readable for graphiz tool
	char nodeString[1000];
	char connectionString[1000];
	char weightString[1000];
	char StringForOutput[10000];

	Tcl_Obj **NodesListAddr;
	Tcl_Obj **WeightListAddr;
	Tcl_Obj **connectionsListAddr;

	ClientData dotty_file; 

	//First of all a graph should be loaded in the Shell. Otherwise print specific message. 

	if ( graph_ok == 0 )
	 {
	 	printf("Please Load a Graph To Shell First! Use Command read_graph <filename>." );
	 	return TCL_ERROR;
	 }

	//Create a file in order to store the loaded graph, so graphiz can have access to it.

	dotty_file = Tcl_OpenFileChannel(interp, "graph_files/dotty_file.txt", "RDWR CREAT TRUNC", 0777); 

	//Gain access to node list by extracting number of nodes and node list address. 

	Tcl_ListObjGetElements ( interp, NodesList, &numberOfNodes, &NodesListAddr );

	//Start construction of the string that is supposed to be saved at new dotty file.

	strcpy(StringForOutput, dotty_starting_string);

	//Start the process of storing nodes, connections and weights to under construction dotty file 

	for (int i = 0; i < numberOfNodes; ++i)
	  {
		Tcl_GetIntFromObj(interp, *(NodesListAddr + i), &node);

		Tcl_ListObjGetElements ( interp, ConnectionsList[i], &numberOfConnections, &connectionsListAddr );
		Tcl_ListObjGetElements ( interp, WeightsList[i], &numberOfWeights, &WeightListAddr );

		//For each node, write to output_string, node number and arrow symbol " -> "

		if (numberOfConnections != 0 )
		 {
			sprintf (nodeString, "n%d", node);
			strcat (StringForOutput, nodeString);
			strcat (StringForOutput, arrowSymbol);
		 }

		//for each node, write to output string, the successors of node

		for (int j = 0; j < numberOfConnections; j++)
		  {
			Tcl_GetIntFromObj(interp, *(connectionsListAddr + j), &connection);
			sprintf ( connectionString,"n%d", connection );
			strcat (StringForOutput, connectionString);

			//write weight with relative label to dotty file

			Tcl_GetIntFromObj(interp, *(WeightListAddr + j), &weight);
			sprintf (weightString, "[label=\"%d\",weight=\"%d\"];\n", weight, weight);
			strcat (StringForOutput, weightString);

			//write node [i] -> for all successors of i node
			
			if ( j != numberOfConnections - 1 )
		     {
			 	sprintf (nodeString, "n%d", node);
				strcat (StringForOutput, nodeString);
				strcat (StringForOutput, arrowSymbol);
		     }
		   }
	  }

	//Complete graphiz format with character symbol "}" 

	strcat (StringForOutput, "}");

	//Write completed string output to dotty file and print success message

	Tcl_WriteChars(dotty_file, StringForOutput, strlen(StringForOutput));		
	Tcl_Close (interp, dotty_file);

	printf("\nGraph Was Succesfully Written To Output File!\nGraph Stored To {graph_files/dotty_file.txt}\n");
	printf("\n...Loading graphviz Tool\n");

	//call dotty command to draw given graph from new created dotty_file 

	check = system("dotty graph_files/dotty_file.txt");

	if (check == - 1) 
	 { 
		return TCL_ERROR;
	 }

	printf("\n...Quiting graphviz Tool\n");

	return TCL_OK;

  }

//Write graph function is the same as draw graph without calling dotty command. Function create only the dotty file. 

int custom_write_graph ( ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[] )         
  {

	int node;
	int numberOfConnections;
	int connection;

	int numberOfWeights;
	int weight;
	int numberOfNodes;

	char arrowSymbol[] = " -> ";
	char dotty_starting_string [] = "digraph {\nnode [fillcolor=\"lightgray\",\nstyle=\"filled,solid\"\nfontsize=10,\nfontcolor=\"red\"]\n";
	char nodeString[1000];
	char connectionString[1000];
	char weightString[1000];
	char StringForOutput[10000];

	Tcl_Obj **NodesListAddr;
	Tcl_Obj **WeightListAddr;
	Tcl_Obj **connectionsListAddr;

	ClientData dotty_file; 

	//First of all a graph should be loaded in the Shell. Otherwise print specific message. 

	if ( graph_ok == 0 )
	 {
	 	printf("Please Load a Graph To Shell First! Use Command read_graph <filename>." );
	 	return TCL_ERROR;
	 }

	//Create a file in order to store the loaded graph. 

	dotty_file = Tcl_OpenFileChannel(interp, "graph_files/dotty_file.txt", "RDWR CREAT TRUNC", 0777); 

	//Gain access to node list by extracting number of nodes and node list address. 

	Tcl_ListObjGetElements ( interp, NodesList, &numberOfNodes, &NodesListAddr );

	//Start construction of the string that is supposed to be saved at new dotty file.

	strcpy(StringForOutput, dotty_starting_string);

	for (int i = 0; i < numberOfNodes; ++i)
	  {
		Tcl_GetIntFromObj(interp, *(NodesListAddr + i), &node);

		Tcl_ListObjGetElements ( interp, ConnectionsList[i], &numberOfConnections, &connectionsListAddr );
		Tcl_ListObjGetElements ( interp, WeightsList[i], &numberOfWeights, &WeightListAddr );

		//For each node, write to output_string, node number and arrow symbol " -> "

		if (numberOfConnections != 0 )
		 {
			sprintf (nodeString, "n%d", node);
			strcat (StringForOutput, nodeString);
			strcat (StringForOutput, arrowSymbol);
		 }

		//for each node, write to output string, the successors of node

		for (int j = 0; j < numberOfConnections; j++)
		{
			Tcl_GetIntFromObj(interp, *(connectionsListAddr + j), &connection);
			sprintf ( connectionString,"n%d", connection );
			strcat (StringForOutput, connectionString);

			//write weight with relative label to dotty file

			Tcl_GetIntFromObj(interp, *(WeightListAddr + j), &weight);
			sprintf (weightString, "[label=\"%d\",weight=\"%d\"];\n", weight, weight);
			strcat (StringForOutput, weightString);
			
			//write node [i] -> for all successors of i node
			
			if (j != numberOfConnections - 1)
		     {
			 	sprintf (nodeString, "n%d", node);
				strcat (StringForOutput, nodeString);
				strcat (StringForOutput, arrowSymbol);
			 }
		}
	  }

	//Complete graphiz format with character symbol "}" 

	strcat (StringForOutput, "}");

	//Write completed string output to dotty file and print success message

	Tcl_WriteChars(dotty_file, StringForOutput, strlen(StringForOutput));
	Tcl_Close (interp, dotty_file);

	printf("\nGraph Was Succesfully Written To Output File!\nGraph Stored To {graph_files/dotty_file.txt}\n");

	return TCL_OK;
  }


//Function that finds critical path of loaded graph and print length of the path along with the nodes included

int custom_graph_critical_path ( ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[] )         
  {

  	char rat_parameter[] ="-RAT";
  	char validParameters[] = "0123456789"; 

 	int length;
 	int third_argument_length;
 	char *third_argument;

  	Tcl_Obj **NodesListAddr;
  	int numberOfNodes;

  	Tcl_Obj **connectionsListAddr;
	int numberOfConnections;

	Tcl_Obj **WeightListAddr;
	int numberOfWeights;

	Tcl_Obj **queueListAddr;
	int numOfQueueElements;

	Tcl_Obj **indexesQueueListAddr;
	int numOfQueueIndexes;

	Tcl_Obj *queueList;
	Tcl_Obj *indexesQueueList;

	Tcl_Obj *nodeIndexObj;
	Tcl_Obj *queueObj;

	Tcl_Obj **fanInList;
	Tcl_Obj *fanInObj;

	Tcl_Obj *critical_path;

	int node;
	int temp_node;
	int weight;
	int nodeConnectedTo;
	int queueItem;
	int queueIndex;

	int *distance;
	int max_distance = -1;
	int *nodeUncheckedPredecessors;

	int slackEnableFlag = 0;

	int j;

	//Arguments must be either the name of command only or the name of command along the option -RAT and a number relevant to RAT ( Required Arrival Time ) 

	if (objc != 1 && objc != 3)
	 {
		printf("Wrong Number of Arguments!");
		return TCL_ERROR;
	 }
	else if ( objc == 3 )
	 {
	 	if ( strcmp ( Tcl_GetStringFromObj( objv[1] , NULL ), rat_parameter  ) == 0 ) 
	     {

	     	//if arguments are three and the second is RAT check if there is a valid number for third argument

	     	third_argument = Tcl_GetStringFromObj( objv[2] , NULL );
			third_argument_length = strlen(third_argument);

			length = strspn ( third_argument, validParameters );

			if (third_argument_length != length) 
			 {
			 	printf("Unvalid Value Next to \"-RAT\" Paramater!\n");
			 	return TCL_ERROR;
			 }

			//1.slackEnableFlag indicates whether a RAT parameter was given along with the command or not.
			//2.If there is no -RAT option, the program will calculate critical path without finding slack. 

			 slackEnableFlag = 1;  		 	
	     }
	    else 
	     {
	     	printf("Unknown Argument \"%s\"!\nAvailable options are {-RAT}\n", Tcl_GetStringFromObj(  objv[1] , NULL ) );
			return TCL_ERROR;
	     }
	 }

	//Check global variable to note if a graph is loaded to memory. If there is no valid graph on memory, exit.

  	if ( graph_ok == 0 )
	 {
		printf("Please Load a Graph To Shell First! Use Command read_graph <filename>." );
		return TCL_ERROR;
	 }

	//Extract elements of node list

	Tcl_ListObjGetElements ( interp, NodesList, &numberOfNodes, &NodesListAddr );

	//Allocate memory for distance (max distance will be critical path)

	distance = (int *) malloc (numberOfNodes * sizeof(int));

	//malloc: check for errors

	if ( distance == NULL )
	 {
		printf ("Failed To Allocate Memory!\n");
		return TCL_ERROR;
	 }

	//Allocate memory for each node's number of predecessors

	nodeUncheckedPredecessors = (int *) malloc (numberOfNodes *sizeof(int));

	//malloc: check for errors
	
	if ( nodeUncheckedPredecessors == NULL )
	 {
		printf ("Failed To Allocate Memory!\n");
		return TCL_ERROR;
	 }

	//Allocate memory for each node's fan in list of nodes

	fanInList = malloc (sizeof (Tcl_Obj *) * numberOfNodes);

	//check malloc for errors

	if ( fanInList == NULL )
	 {
		printf ("Failed To Allocate Memory!\n");
		return TCL_ERROR;
	 }


	for (int i = 0; i < numberOfNodes; i++)
	  {
	  	//initialize with zero each node's number of unchecked predecessors

		*( nodeUncheckedPredecessors + i ) = 0;

		//for each node in graph, initialize list of fan-in nodes

		 fanInList[i] = Tcl_NewListObj (0, NULL);
	  }

	//In this part of the 3 for-loops,graph located in memory is being searched,
	//..in order to fill fan-in list and predecessor list with the correct values
	
	for (int i = 0; i < numberOfNodes; i++)
	  {

		Tcl_GetIntFromObj(interp, *(NodesListAddr + i), &node);
		Tcl_ListObjGetElements ( interp, ConnectionsList[i], &numberOfConnections, &connectionsListAddr );
		

		//first step is to initialize distance of each node to zero. Distance of a node will increase until max value for each of its connections 

		*( distance + i ) = 0;
		
		for (int j = 0; j < numberOfConnections; j++)
		  {

			Tcl_GetIntFromObj(interp, *(connectionsListAddr + j), &nodeConnectedTo);

			//for every node in connection list of node i, we must find its index in node list

			for (int k = 0; k < numberOfNodes; k++ )
			  {

			  	Tcl_GetIntFromObj(interp, *(NodesListAddr + k), &temp_node);

				if ( temp_node == nodeConnectedTo )
				 {

				 	//when index of node connected to node i is found, we increase predecessors number list with 1 

				 	*( nodeUncheckedPredecessors + k ) = *( nodeUncheckedPredecessors + k ) + 1;

				 	//node i is the predecessor ( fan-in element  ) of node k, so node i is stored in fan-in list of node k.

	 		 		fanInObj = Tcl_NewIntObj(node);
	 		 		Tcl_ListObjAppendElement (interp, fanInList[k], fanInObj );
				 }
			  }
		  }
	  }

	//Create list for queue of nodes

	queueList = Tcl_NewListObj (0,NULL);

	//Create list fir indexes of nodes parallel to queue list

	indexesQueueList = Tcl_NewListObj(0, NULL);

	for (int i = 0; i < numberOfNodes; ++i)
	  {
		Tcl_GetIntFromObj(interp, *(NodesListAddr + i), &node);

		//queue must be initialized with root nodes. If a node has zero predecessors is root node.

		if ( *(nodeUncheckedPredecessors + i ) == 0 )
		 {
			Tcl_GetIntFromObj(interp, *(NodesListAddr + i), &node);

			queueObj = Tcl_NewIntObj(node);
			Tcl_ListObjAppendElement (interp, queueList, queueObj);
			
			nodeIndexObj = Tcl_NewIntObj(i);
			Tcl_ListObjAppendElement (interp, indexesQueueList, nodeIndexObj);		
		 }
	  }

	Tcl_ListObjGetElements ( interp, queueList, &numOfQueueElements, &queueListAddr );
	Tcl_ListObjGetElements ( interp, indexesQueueList, &numOfQueueIndexes, &indexesQueueListAddr );

	//Main part of the algorithm. Increment distance for queue nodes until queue becomes empty.

	while ( numOfQueueElements != 0 )
	  {

	 	//Extract first element in queue ( both node value and index)

	 	Tcl_GetIntFromObj(interp, *(queueListAddr), &queueItem);
	 	Tcl_GetIntFromObj(interp, *(indexesQueueListAddr), &queueIndex);

	 	//Remove extracting element from queue list (both node and index)

	 	Tcl_ListObjReplace(interp, queueList, 0, 1, 0, NULL);
	 	Tcl_ListObjReplace(interp, indexesQueueList, 0, 1, 0, NULL);

	 	//Extract connections and weight of connections for the current node in queue

	 	Tcl_ListObjGetElements ( interp, ConnectionsList[queueIndex], &numberOfConnections, &connectionsListAddr );
	 	Tcl_ListObjGetElements ( interp, WeightsList[queueIndex], &numberOfWeights, &WeightListAddr );

	 	for (int i = 0; i < numberOfConnections; i++)
	 	  {

	 		Tcl_GetIntFromObj(interp, *(connectionsListAddr + i), &nodeConnectedTo);
	 		Tcl_GetIntFromObj(interp, *(WeightListAddr + i), &weight);

	 		node = -1;
	 		j = 0;

	 		//for each successor of current node in queue, find its index in node list

	 		while ( j < numberOfNodes && nodeConnectedTo != node )
	 		  {
	 		  	Tcl_GetIntFromObj(interp, *(NodesListAddr + j), &node);

	 		  	if ( node != nodeConnectedTo ) 
	 		  	 {
	 		  	 	j++;
	 		  	 }
	 		  }

	 		//increment index according to formula that can be found in course's lectures

	 		if ( ( *(distance + queueIndex) + weight ) >  *( distance + j ) ) 
	 		 {
	 		 	*( distance + j ) = ( *(distance + queueIndex) + weight );

	 			//1.find and store max distance among the distances of all nodes.
	 			//2.Final value of max distance is the length of the critical path. 	

	 		 	if ( *( distance + j ) > max_distance ) 
	 		 	 {
	 		 	 	max_distance = *( distance + j );
	 		 	 }
	 		 }

	 		//After calculating above formula, mark predecessor of node j as checked.

	 		*(nodeUncheckedPredecessors + j) = *(nodeUncheckedPredecessors + j) - 1;

	 		//If a node has all predecessors checked must be inserted to queue.

	 		if ( *(nodeUncheckedPredecessors + j) == 0 )
	 		 {
	 			queueObj = Tcl_NewIntObj(nodeConnectedTo);
				Tcl_ListObjAppendElement (interp, queueList, queueObj);

				nodeIndexObj = Tcl_NewIntObj(j);
				Tcl_ListObjAppendElement (interp, indexesQueueList, nodeIndexObj);
	 		 }
	 	  }

	 	//Refresh queue and queue of indexes list, because of possible new elements that might were inserted

	 	Tcl_ListObjGetElements ( interp, queueList, &numOfQueueElements, &queueListAddr );
	    Tcl_ListObjGetElements ( interp, indexesQueueList, &numOfQueueIndexes, &indexesQueueListAddr );

	  }
	
	//Critical path is the final value of max distance
	//Back trace function is called to find nodes that are included in the path and calculate slack if it is requested.

	critical_path = back_trace (interp, slackEnableFlag, fanInList, distance, max_distance, objv[2] );

	//if back_trace ended with failure return error.

	if ( critical_path == NULL ) 
	 {
	 	printf("Error!\n");
	 	return TCL_ERROR;
	 } 

	//Back trace returns list of the nodes included in critical path.  

	printf("->Critical Path Consists Of Nodes { %s }.\n", Tcl_GetStringFromObj (critical_path, NULL) );
	printf("\n------------------\n");

	//Exit graph critical path with success.(TCL_OK)
	free(distance);
	free(fanInList);
  	return TCL_OK;
  }


//back_trace -function calculates slack if it is requested and finds nodes included in critical path

Tcl_Obj *back_trace (Tcl_Interp *interp,  int slackEnableFlag, Tcl_Obj **fanInList, int *distance ,int max_distance, Tcl_Obj *rat_argument ) 
  {

  	Tcl_Obj **NodesListAddr;
  	int numberOfNodes;

  	Tcl_Obj **connectionsListAddr;
	int numberOfConnections;

	Tcl_Obj **WeightListAddr;
	int numberOfWeights;

	Tcl_Obj **queueListAddr;
	int numOfQueueElements;

	Tcl_Obj **indexesQueueListAddr;
	int numOfQueueIndexes;

	int node;
	int weight;
	int nodeConnectedTo;
	int queueItem;
	int queueIndex;

  	int *slack;
  	int rat_number;
	int fanInElement;

	Tcl_Obj *critical_path_nodeList;

	Tcl_Obj *queueList;
	Tcl_Obj *indexesQueueList;

	Tcl_Obj *nodeIndexObj;
	Tcl_Obj *queueObj;

	Tcl_Obj **FanInListAddr;
	int numFanInElements;

	int j;
	int k;

	//Gain access to node list

	Tcl_ListObjGetElements ( interp, NodesList, &numberOfNodes, &NodesListAddr );

	//Create a list for critical path nodes that goind to be inserted into.

	critical_path_nodeList =  Tcl_NewListObj (0, NULL);

	//Create list for a queue of nodes and for a list of node indexes

	queueList = Tcl_NewListObj (0,NULL);
	indexesQueueList = Tcl_NewListObj(0, NULL);

	//Allocate memory for slack dynamic array. Slack of each node is going to be inserted into the array.

	slack = (int *) malloc (sizeof(int) *numberOfNodes);

	//check for errors in malloc

	if ( slack == NULL ) 
 	 {
		printf ("Failed To Allocate Memory!\n");
		return NULL;
 	 }


	for (int i = 0; i < numberOfNodes; ++i)
	 {

	 	//Initialize slack of each node with longest-critical path value  

	 	Tcl_GetIntFromObj(interp, *(NodesListAddr + i), &node);
	 	*(slack + i) = max_distance;

	 	if ( *(distance + i) == max_distance )
	 	 {

	 	 	//node with max distance is the first node in critical path to be inserted,
	 	 	//..in critical path node list (from last to first because of back tracing) 

	 	 	Tcl_ListObjAppendElement (interp, critical_path_nodeList,  *(NodesListAddr + i));
	 	 	
	 	 	//node with max distance will be the first node in queue, during back trace process

	 	 	queueObj = Tcl_NewIntObj(node);
			Tcl_ListObjAppendElement (interp, queueList, queueObj);

			nodeIndexObj = Tcl_NewIntObj(i);
			Tcl_ListObjAppendElement (interp, indexesQueueList, nodeIndexObj);


	 	 	if ( slackEnableFlag == 1 )
	 	 	 {

	 	 	 	//slackEnableFlag indicates whether or not a Required arrival time is given as argument,
	 	 	 	//..along with the command.Slack value of node with max distance will be RAT - max_distance. 

	 	 		Tcl_GetIntFromObj(interp, rat_argument, &rat_number);
		 	 	*(slack + i) = rat_number - max_distance;	
	 	 	 }
	 	 	else 
	 	 	 {
	 	 	 	//set default value of slack for node with max distance to zero, if no RAT was given.
	 	 		*(slack + i) = 0;
	 	 	 }
	 	 }	 	
	 }

	Tcl_ListObjGetElements ( interp, queueList, &numOfQueueElements, &queueListAddr );
	Tcl_ListObjGetElements ( interp, indexesQueueList, &numOfQueueIndexes, &indexesQueueListAddr );

	//Keep doing back trace process until queue is empty.

	while ( numOfQueueElements != 0 )
	 {

	 	//Extract first node in queue along with its index.

	 	Tcl_GetIntFromObj(interp, *(queueListAddr), &queueItem);
	 	Tcl_GetIntFromObj(interp, *(indexesQueueListAddr), &queueIndex);

	 	//Remove element from queue.

	 	Tcl_ListObjReplace(interp, queueList, 0, 1, 0, NULL);
	 	Tcl_ListObjReplace(interp, indexesQueueList, 0, 1, 0, NULL);

	 	//Gain access to fan-in nodes of extracted from queue node.

	 	Tcl_ListObjGetElements ( interp, fanInList[queueIndex], &numFanInElements, &FanInListAddr );

	 	for (int i = 0; i < numFanInElements; i++)
	 	 {
	 		Tcl_GetIntFromObj (interp, *(FanInListAddr + i), &fanInElement);

	 		//for node i of fanin elements of node in queue, first step is to find its index in node list. 

	 		node = -1;
	 		j = 0;

	 		while ( j < numberOfNodes && fanInElement != node )
	 		  {

	 		  	Tcl_GetIntFromObj(interp, *(NodesListAddr + j), &node);
	 		  	
	 		  	if ( node != fanInElement ) 
	 		  	 {
	 		  	 	j++;
	 		  	 }
			  }

			//for node i in fan-in elements gain access to its connections along with weights.  

	 		Tcl_ListObjGetElements ( interp, WeightsList[j], &numberOfWeights, &WeightListAddr );
	 		Tcl_ListObjGetElements ( interp, ConnectionsList[j], &numberOfConnections, &connectionsListAddr );

			nodeConnectedTo = -1;
		 	k = 0;

		 	//to change slack value we are interested only in the connection with successor-queue element 
		 	//..of node i of fanin elements. For this reason we are searching for index k of this connection.

		 	while ( k < numberOfConnections && nodeConnectedTo != queueItem )
		 	   {
		 		  
		 		  	Tcl_GetIntFromObj(interp, *(connectionsListAddr + k), &nodeConnectedTo);
		 	 	  
		 	 	  	if ( queueItem != nodeConnectedTo ) 
		 		  	 {
		 		  	 	k++;
		 		  	 }
				}

			//we are interested in weight of connection of fan-in element k with its successor-current node in queue.

			Tcl_GetIntFromObj(interp, *(WeightListAddr + k), &weight);

			//calculate slack according to formula given in course's lectures.

			*(slack + j) = *( slack + queueIndex ) + ( *(distance + queueIndex) -  ( *(distance + j) + weight)  );
		 		
			//if slackenableflag is raised and slack value of node j reached rat-max distance value,
			//it means that node j belongs to critical path.

		 	if ( slackEnableFlag == 1 )
		 	 {	
		 		if ( *(slack + j) == rat_number - max_distance )
	 			 {

	 			 	//put node j and its index to queue.

		 			queueObj = Tcl_NewIntObj(fanInElement);
					Tcl_ListObjAppendElement (interp, queueList, queueObj);

					nodeIndexObj = Tcl_NewIntObj(j);
					Tcl_ListObjAppendElement (interp, indexesQueueList, nodeIndexObj);

					//put node j to critical path node list.
					
					Tcl_ListObjAppendElement (interp, critical_path_nodeList, queueObj);

					//break;
	 		 	 }
	 		 }
	 		else 
	 		 {

	 		 	//1.this else is being accessed, if no specific arrival time is requested.

	 		 	//2.For this reason default value of slack of the longest path is set to zero.

	 		 	//3.Although slack will not be printed as output, process must continue the same way as above,
	 		 	//..because of the need to print the critical path node list.  

	 		  	if ( *( slack + j ) == 0 )
	 			 {

		 			queueObj = Tcl_NewIntObj(fanInElement);
					Tcl_ListObjAppendElement (interp, queueList, queueObj);

					nodeIndexObj = Tcl_NewIntObj(j);
					Tcl_ListObjAppendElement (interp, indexesQueueList, nodeIndexObj);

					Tcl_ListObjAppendElement (interp, critical_path_nodeList, queueObj);

					//break;
	 		 	 }
	 		 }
	 	 }

	 	//Refresh queue lists.

	 	Tcl_ListObjGetElements ( interp, queueList, &numOfQueueElements, &queueListAddr );
	    Tcl_ListObjGetElements ( interp, indexesQueueList, &numOfQueueIndexes, &indexesQueueListAddr );
	 }


	//Back_trace process is done. Printing-result process is the next step.

	printf("\n--SUMMARY REPORT--\n\n");
	printf("->Length Of Critical Path Is %d\n\n", max_distance);

	//if slackEnableFlag is 1, print slack results.

	if ( slackEnableFlag == 1 )
	{
		printf("->Calculate Slack For Each Node In Graph...\n");

		//print slack for each node.

		for (int i = 0; i < numberOfNodes; ++i)
	      {
	 		Tcl_GetIntFromObj(interp, *(NodesListAddr + i), &node);
	 		printf("   Slack Of Node %d Is %d.\n",node, *(slack + i) );
	 	  }

	 	printf("\n->Calculate Slack For The Critical Path...\n");

	 	//print slack of the critical path.

	 	if ( rat_number - max_distance < 0 ) 
	 	 {

	 	 	//if slack of critical path is violated, print slack along with (VIOLATED) message.

	 	 	printf("   Slack Of Critical Path Is %d. (VIOLATED)\n\n", rat_number - max_distance );
	 	 }
	 	else 
	 	 {
	 	 	printf("   Slack Of Critical Path Is %d.\n\n", rat_number - max_distance );
	 	 }
	}
			
	//return to graph_critical_path function the completed list of nodes included in the path.

	free(slack);
	return critical_path_nodeList;		
}

