// Timer.h

#ifndef _TIMER_h
#define _TIMER_h
#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class Timer
{
private: // rieng , để các biến trong này đến 
	unsigned long diem_cuoi; // điểm cuối
	unsigned long diem_hien_tai; // điểm hiện tại
	
	Timer(); // hàm dựng
	~Timer(); // hàm hủy 
	
public : // chung 
//----------- sử dụng kỹ thuật Singletons --------
	static Timer *getInstance() 
		// khai báo thằng getInstance
		// static cố định cái biến hay con trỏ
	{
		static Timer *instance = new Timer(); 
		// đoạn này khai báo con trỏ instance 
		// rồi cấp phát bộ nhớ thuộc kiểm Tiemr()
		return instance; // trả về kết quả instance 
	}
	// như vậy bất kỳ hàm nào trong lớp Timer() này từ dưới hàm này trở xuống 
	// muốn sử dụng hoặc gọi ra đều phải đi qua thằng *getInstance 	
	//--------------- end Singletons ----------

	void khoi_tao(); // hàm khởi tạo gọi đầu tiên trong hàm setup , diem_cuoi = millis() 
	void cap_nhat(); // hàm cập nhật gọi đầu tiên trong hàm Loop , diem_hien_tai = millis()
	unsigned long tinh_delta(); // tính delta giữa 2 lần loop , diem_hien_tai - diem_cuoi
	void reset_diem(); // reset lại diem_cuoi = diem_hien_tai .
};

#endif

