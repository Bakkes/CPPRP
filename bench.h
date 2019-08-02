#pragma once
//https://stackoverflow.com/questions/2349776/how-can-i-benchmark-c-code-easily
#ifdef _WIN32

#include <windows.h>
double get_time()
{
	LARGE_INTEGER t, f;
	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&f);
	return (double)t.QuadPart / (double)f.QuadPart;
}

#else

#include <sys/time.h>
#include <sys/resource.h>

double get_time()
{
	struct timeval t;
	struct timezone tzp;
	gettimeofday(&t, &tzp);
	return t.tv_sec + t.tv_usec*1e-6;
}

#endif

#define BENCHMARK(name, func, iterations) \
{\
double start_time = get_time();\
for(int aaaa = 0; aaaa < iterations; aaaa++) {\
func;\
}\
double end_time = get_time();\
double elapsed = (end_time-start_time) * 1000.f;\
printf("[%s] Ran %i iterations in %.5f ms (avg: %.5f ms)\n", name, iterations, elapsed, (elapsed/(double)iterations));\
}
