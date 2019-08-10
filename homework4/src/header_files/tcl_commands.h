//////////////////////////////////////////////////////////////////
//University      : Electrical & Computer Engineering
//Course          : CE437 - Computer-Aided Design Algorithms
//Full Name       : Panagiotis Anastasiadis
//Registry Number : 2134
//Project         :	"CAD Algorithms Implementation On A TCL Shell"
//////////////////////////////////////////////////////////////////

// All TCL Built-in commands plus the custom commands ls, less and the positional cubic operations.
// This array with strings is requested by the tab completer to check possible matches between the commands and the typed word in command line.

#ifndef TCL_COMMANDS_H_INCLUDED
#define TCL_COMMANDS_H_INCLUDED

char *tcl_commands[] = {
"after", "error", "lappend", "platform", "tcl_findLibrary",
"append", "eval", "lassign", "platform::shell",	"tcl_startOfNextWord",
"apply", "exec", "lindex"	, "proc"	,"tcl_startOfPreviousWord", 
"array", "exit", "linsert"	,"puts", "tcl_wordBreakAfter"
"auto_execok" ,	 "expr",	"list",	"pwd" , "tcl_wordBreakBefore",
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
"close", "namespace", "source", "vwait",
"concat", "http", "history" , "open", "split", "while",
"continue", "if", "package", "string",
"dde", "incr",	"parray", "subst",
"dict",	"info",	"pid",	"switch",
"encoding",	"interp", "pkg::create", "Tcl",
"eof",	"join",	"pkg_mkIndex",	"tcl_endOfWord", 
"ls","less",
"cube_intersect_2", "supercube_2", "distance_2", "cube_cover_2", "sharp_2", "sharp", "OFF_f", 
"read_graph", "draw_graph", "write_graph", "graph_critical_path",
"alg_division", "r_kernels", NULL };

#endif