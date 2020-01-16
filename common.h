#pragma once
#include <string>
typedef std::string string;
enum Flag{
	cpu,mem,proc,wea,term,null
};
struct Weather {
	string weather_type;
	string min_temp;
	string max_temp;
	string wind_dir;
	string wind_str;
	string water;
};