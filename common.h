#pragma once
#include <string>
typedef std::string string;
enum Flag{
	cpu,mem,proc,wea,term,help,null
};
enum Sort {
	///asc=rosnaco,desc-malejaco
	asc, desc
};
enum Help {
	unknow,show,kill,clear,helpp,init
};
struct Weather {
	string weather_type;
	string min_temp;
	string max_temp;
	string wind_dir;
	string wind_str;
	string water;
};
struct Processes_info {
	Processes_info(Sort ord=Sort::asc, int indx=0) :sort{ ord }, index{ indx }{}
	Sort sort;
	int index;
};