// Work_scheduler.h

#ifndef _WORK_SCHEDULER_h
#define _WORK_SCHEDULER_h

#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Work_scheduler
{
private: 
	unsigned long thoi_gian_troi_qua;
	unsigned long thoi_gian_lam_viec;

	int _pin_Work;
	
	// con trỏ hàm , kiểu void không trả về giá trị , nhận  tham số int 
	void(*con_tro_ham)(int);

public : 
	// khởi tạo hàm dựng có tham số 
	// int Pin : là chân Pin sẽ làm việc
	// unsigned long time : thơi gian làm việc 
	// void(*con_tro_ham)(int) : nó trỏ tới cái việc cần làm 
	// đây là 1 kiểu gọi hàm trong hàm , trong hàm class to có thằng hàm con 
	// thằng hàm con viết sau , để nó dưới dạng con trỏ .
	Work_scheduler( int Pin , unsigned long time , void (*con_tro_ham)(int));

	// hàm hủy , nhiệm vụ giải phóng bộ nhớ 
	// những cái gì đã đưa vào hàm tạo bên trên thì hủy nó đi để giải phóng bộ nhớ 
	// hàm này không cần gọi , nó cũng tự chạy 
	~Work_scheduler();
	
	// hàm này tính toán để làm việc , 
	// ta sẽ tính thời gian trôi qua >= thời gian làm việc 
	// thì Pin sẽ làm việc , rồi chúng ta reset lại thời gian trôi qua 
	void tinh_thoi_gian_de_lam_viec();
};
#endif

