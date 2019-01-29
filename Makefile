cshell:	run.c pinfo.c  launch.c execute.c readSplit.c builtinFuncs.c ls.c piping.c redirect.c signal.c print.c
	gcc	-o	cshell run.c  pinfo.c  launch.c execute.c readSplit.c builtinFuncs.c ls.c  piping.c redirect.c  signal.c print.c -I.

