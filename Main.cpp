#define cpuu
#ifdef  mainn
#include <iostream>
#include "Mainwindow.h"
int main() {
	/*int x=500, y=500;
	std::cout << "x: "; std::cin >> x;
	std::cout << "y: "; std::cin >> y;
	Mainwindow window(sf::Vector2f(x,y));*/
	Mainwindow window;
	window.run();
	return 0;
}
#endif //  mainn
#ifdef test
#include <windows.h>
#include <iostream>
#include "psapi.h"
using namespace std;
int main(){
	char *x = new char[1024 * 1024*512 ];
	//MEMORYSTATUSEX statex;
	//statex.dwLength = sizeof(statex); // I misunderstand that
	//GlobalMemoryStatusEx(&statex);
	//cout << "Physical RAM => " << (float)statex.ullTotalPhys / (1024 * 1024 * 1024) << endl;
	//system("PAUSE");
	//return EXIT_SUCCESS;

	/*MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
	DWORDLONG freePhysMem = memInfo.ullAvailPhys;
	DWORDLONG physMemUsed = totalPhysMem - freePhysMem;
	cout << "Total: "<<totalPhysMem/1024/1024<<endl<<"Free: "<<freePhysMem/1024/1024<<endl<<"Used: "<<physMemUsed/1024/1024;
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	cout <<endl<<"Used by me: "<<physMemUsedByMe/1024/1024;*/


	cin.ignore(1);
	delete[]x;
	return 0;
}
#endif // test
#ifdef cpuu
#include "TCHAR.h"
#include "pdh.h"
#include "windows.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <random>
#include <math.h> 
static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;
using namespace std;
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;
void init2() {
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;
	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;
	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));
	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
}
void init() {
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	// You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
	PdhAddEnglishCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);
}
double getCurrentValue() {
	PDH_FMT_COUNTERVALUE counterVal;
	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	return counterVal.doubleValue;
}
double getCurrentValue2() {
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;
	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lastSysCPU.QuadPart) +(user.QuadPart - lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;
	return percent * 100;
}
void fun(std::future<void> future) {
	while (future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
		size_t size = 1024*1024;
		double * ptr = new double[size];
		/*for (size_t i = 0; i < size; i++) {
			if (future.wait_for(std::chrono::milliseconds(1)) != std::future_status::timeout) {
				cout << "terminate" << endl;
				break;
			}
			double lower_bound = 0;
			double upper_bound = 10000;
			std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
			std::default_random_engine re;
			for (size_t i = 0; i < size; i++)
				ptr[i] = unif(re);
			sin(ptr[i])*cos(ptr[i]);
		}*/
		delete[]ptr;
		//cout << "loop" << endl;
	}
}
int main() {
	std::promise<void> signal_exit; //create promise object
	std::future<void> future = signal_exit.get_future();//create future objects
	init();
	init2();
	thread thr(fun,std::move(future));
	for (int i = 1; i < 1000;i++) {
		if (i % 20 == 0)system("cls");
		cout <<"My: "<< getCurrentValue2()<<endl<<"All: "<<getCurrentValue()<<endl<<endl;
		std::this_thread::sleep_for(200ms);
	}
	signal_exit.set_value();
	thr.join();
	cin.ignore(1);
	return 0;
}
#endif // cpuu


