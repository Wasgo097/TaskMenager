#pragma once
#include <string>
typedef std::string string;
enum Flag{
<<<<<<< HEAD
	cpu,mem,proc,wea,term,null
=======
	cpu,mem,proc,wea,null
>>>>>>> df995e29a225298c097c0d5dc921483d00163261
};
struct Weather {
	string weather_type;
	string min_temp;
	string max_temp;
	string wind_dir;
	string wind_str;
	string water;
};