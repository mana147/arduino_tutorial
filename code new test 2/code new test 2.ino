#include "WorkScheduler.h"
#include "Timer.h"

const int pinRed = 13;
byte inUsedByAnalog = 0;


//khởi tạo các job
WorkScheduler *redPinWorkScheduler;
WorkScheduler *printToSerialScheduler;
WorkScheduler *printToSerial100msScheduler;
WorkScheduler *ramCheckScheduler;
WorkScheduler *analogReadScheduler;



void OnRedPin_Work(int pin)
{
	if (!inUsedByAnalog) {
		static boolean wasLightedUp = false; 
		digitalWrite(pin, !wasLightedUp ? HIGH : LOW);
		wasLightedUp = !wasLightedUp;
	}
}

//chúng ta không dùng pin gì hết vì mình đã viết thêm một cập nhập nhỏ
void printToSerial() {
	static unsigned long counter = 0;
	counter++;
	Serial.print("kLaserCutter - ksp. Lan thu: ");
	Serial.println(counter);
}

void printToSerial100ms() {
	static unsigned long counter = 0;
	counter++;
	Serial.print("ksp's handsome!!! Lan thu: ");
	Serial.println(counter);
}

//lấy ram còn trong arduino :3 
int getMemoryFree() {
	// Trong trường hợp này, ta có thể hiểu extern sẽ khai báo một biến toàn cục trong chương trình (nếu chưa có) hoặc include một biến toàn cục đã được extern trước đó
	extern int __heap_start;
	extern int *__brkval;

	//Dấu & phía trước tên biến / tên con trỏ sẽ cho ta biết vị trí ô nhớ mà nó đang đứng
	//Lưu ý: bài viết này không dành cho beginner và bạn cần tưởng tượng một chút để có thể mườn tượng vấn đề
	return (int)SP - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

//hàm getRam
void getRam() {
	static unsigned long counter = 0;
	static byte *arr = NULL; // Thử bỏ chữ static thử xem bạn hiền ơi
	counter++;
	if (counter % 3 == 0) {
		if (arr != NULL) {
			delete[]arr;
			arr = NULL;
		}
		else {
			arr = new byte[rand() % getMemoryFree()];
		}
	}
	Serial.print("Ram con lai trong Arduino: ");
	Serial.print(getMemoryFree());
	Serial.println(" bytes!");
}

//analogRead từ chân A0 - cập nhập liên tục
void analogReadTest(int pin) {
	int value = analogRead(pin);
	if (value == 0) {
		digitalWrite(pinRed, LOW);
		inUsedByAnalog = true;
	}
	else {
		inUsedByAnalog = false;
	}
}


void setup()
{
	//Khởi tạo các pin nếu cần
	pinMode(pinRed, OUTPUT);

	//Khởi tạo serial ở mức baudrate 115200
	Serial.begin(9600);

	//Khởi gạo class timer (design pattern singleton)
	Timer::getInstance()->initialize();

	//Khởi tạo một công việc (job), cứ mỗi một giây bật / tắt đèn 1 lần (nôm na code blink :3)
	redPinWorkScheduler = new WorkScheduler(pinRed, 40UL, OnRedPin_Work); //sẽ gọi hàm onReadPin_work

																		  //Khởi tạo một công việc (job) - không đùng đến một pin cụ thể nào đó mà chỉ thực hiện các tác vụ như in serial monitor hoăc đọc các cảm biến có nhiều chân ^_^
	printToSerialScheduler = new WorkScheduler(1000UL, printToSerial);

	//print ra nhanh hơn
	printToSerial100msScheduler = new WorkScheduler(100UL, printToSerial100ms);

	//in ra ram còn dư giống Task Manager / Performance tron Windows
	ramCheckScheduler = new WorkScheduler(1000UL, getRam);

	//hàm kiểm tra cảm biến chạm
	analogReadScheduler = new WorkScheduler(A0, 0, analogReadTest);
}

void loop()
{
	//đầu hàm loop phải có để cập nhập thời điểm diễn ra việc kiểm tra lại các tiến trình
	Timer::getInstance()->update();

	//Không quan trọng thứ tự các job, các job này là các job thực hiện các công việc độc lập với nhau
	// tuy nhiên job analogReadScheduler và job 
	redPinWorkScheduler->update();
	printToSerialScheduler->update();
	ramCheckScheduler->update();
	printToSerial100msScheduler->update();
	analogReadScheduler->update();


	//cuối hàm loop phải có để cập nhập lại THỜI ĐIỂM (thời điểm chứ ko phải thời gian nha, tuy tiếng Anh chúng đều là time) để cho lần xử lý sau
	Timer::getInstance()->resetTick();

}