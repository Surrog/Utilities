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
