#ifndef SEARCH_TASK_HPP
#define SEARCH_TASK_HPP

#include "boost\filesystem.hpp"
#include "InputOutput.hpp"


struct search_task
{
private:
	const Input& _input;
	Output _output;

public:

	search_task(const Input& input);

	void do_search();

	const Output& getOutput();

	string_pool output_path(const boost::filesystem::path& input);

	void search_file(const boost::filesystem::path& path);

	void search_directory(const boost::filesystem::path& path);

	void search(boost::filesystem::path path, bool recurcive);

	template<typename T, typename Y>
	bool find(const T& in, const Y& value)
	{
		return in.find(value.c_str()) != T::npos;
	}
};



#endif //!SEARCH_TASK_HPP