Utilities
===========

fast_search
==========

fast search across files, directory and content

Allow a quick query on different criteria, using basic wildcard :

	xsearch [--clean] [-rcnd] [-t root_path] [-f[nd] filter_value] search_value
	--clean remove search detail and performance time
	-n search in filename
	-c search in file content
	-d search in directory name
	-r recurse on sub directory
	-t followed by a path, set the search location
	-f [nd] followed by a pattern: will filter by directory or filter the name where the content is searched.

output: full path of each files on single line in the standard output.

example:

	D:\Projet\Github\fast_search\search>xsearch -rn *obj -fd *Debug*
	target: D:\Projet\Github\fast_search\search
	search "*obj"
	target D:\Projet\Github\fast_search\search
	recursive 1
	filename 1
	directory name 0
	file content 0
	filter "*Debug*"
	filter on filename 0
	filter on directory name 1
	filter on file content 0

	D:\Projet\Github\fast_search\search\search\Debug\main.obj
	D:\Projet\Github\fast_search\search\search\Debug\search_task.obj
	D:\Projet\Github\fast_search\search\search\Debug Large Test\main.obj
	D:\Projet\Github\fast_search\search\search\Debug Large Test\search_task.obj
	execution time: 8 ms
	
	

basic_rm
==========

Allowed options:

	--help	produce help message
	--f		ignore nonexistent files, never prompt
	--i		prompt before every removal
	--I		prompt only once. Less intrusive than - i, while still
			giving protection against most mistakes
	--r		remove directories and their contents recursively
  	--v		explain what is being done
  	--path <arg>	path to element being remove

xcat
==========
Display files or directories content.

Allowed options:

	--help	produce help message
	--t     keep reading the file to display new write
	--i     will recursively display the content of all directory
	--n    	display the line number while reading the file
                giving protection against most mistakes
	--path  path to display (implicit)

netcat
==========
Allowed options:

	--help	produce help message
	-l		set the program to listen
	<destination><port>	set the program to connect to this address

clean_twin
==========

Allowed options:

	<path>		directory path to clean
