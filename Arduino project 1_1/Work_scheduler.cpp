// 
// 
// 

#include "Work_scheduler.h"
#include "Timer.h"
#include <Arduino.h>

Work_scheduler::Work_scheduler(int pin_work, unsigned long time, void(*con_tro_ham)(int) )
{
	_pin_Work = pin_work;
	thoi_gian_lam_viec = time;
	thoi_gian_troi_qua = 0;
	this->con_tro_ham = con_tro_ham ;
}

Work_scheduler::~Work_scheduler()
{
	_pin_Work = 0;
	thoi_gian_lam_viec = 0;
	thoi_gian_troi_qua = 0;
	con_tro_ham = NULL;
}

void Work_scheduler::tinh_thoi_gian_de_lam_viec()
{
	thoi_gian_troi_qua = thoi_gian_troi_qua + Timer::getInstance()->tinh_delta();
	
	if (thoi_gian_troi_qua > thoi_gian_lam_viec)
	{
		thoi_gian_troi_qua = thoi_gian_troi_qua - thoi_gian_lam_viec;
		
		if (con_tro_ham != NULL)
		{
			con_tro_ham(_pin_Work );
		}
	}
}

// không tự tin vào code của mình à ??
// thật ra gì nó đơn giản trẳng qua cho thêm mấy cái kỹ thuật vào nhìn code cho nó đẹp thì nó tự nhiên khó lên thôi
