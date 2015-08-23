#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifdef _WIN32
#define OS_WINDOWS 1
#define PLATFORM_DESKTOP 1
#elif __MACH__
#define OS_X 1
#define PLATFORM_DESKTOP 1
#elif __linux__
#define OS_LINUX 1
#define PLATFORM_DESKTOP 1
#else
#define MSP430 1
#endif

#define LOG

#define BENCHMARK_BLUETOOTH_LATENCY_WARMUPS     1000
#define BENCHMARK_BLUETOOTH_LATENCY_ITERATIONS 10000

// If READOUT_COMPRESS is defined, choose size of LSBs to consider for compression
//#define READOUT_COMPRESS
#define READOUT_COMPRESS_LSB 3

//#define READOUT_100HZ_SNIFF

#endif /* PLATFORM_H_ */
