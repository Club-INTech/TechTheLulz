#define CAL_SIZE 1001

//g
#define MIN_X1 73
#define MAX_X1 1802
#define MIN_Y1 101
#define MAX_Y1 1746

//d
#define MIN_X2 29
#define MAX_X2 1000
#define MIN_Y2 70
#define MAX_Y2 1051

const FLOAT calibr_1[CAL_SIZE]={
		#include "calibr_1"
};

const FLOAT calibr_2[CAL_SIZE]={
		#include "calibr_2"
};
