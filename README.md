fast_search
===========

fast search across files, directory and content

Allow a quick query on different criteria, using basic wildcard :

	xsearch [-rcnd] [-t root_path] [-f[nd] filter_value] search_value
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
