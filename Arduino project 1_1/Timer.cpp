// 
// 
// 

#include "Timer.h"
#include <Arduino.h>

Timer::Timer() { }

Timer::~Timer() { }


void Timer::khoi_tao()
{
	diem_cuoi = millis();
}

void Timer::cap_nhat()
{
	diem_hien_tai = millis();
}

unsigned long Timer::tinh_delta()
{
	return diem_hien_tai - diem_cuoi;
}

void Timer::reset_diem()
{
	diem_cuoi = diem_hien_tai;
}
