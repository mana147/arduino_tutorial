#include <Wire.h> 
#include <LiquidCrystal.h>

#define DHT_ER  0
#define DHT_OK  1
#define DHT_DATA  13

byte HNhietDo, LNhietDo, HDoAm, LDoAm;
byte DHT_GetTemHumi(byte &tem1, byte &tem2, byte &humi1, byte &humi2);
//------------------------------------------------------------------
const byte add_DS1307 = 0x68;
const byte byte_data = 7;
int second, minute, hour, day, wday, month, year;
int giay, phut, gio, thu, ngay, thang, nam;
//----------------------------------------------------------------------
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int relay_dao_trung = 12;
int relay_den_nhiet_do = 11;
int relay_den_do_am = 10;
int relay_quat_hut = 9;


void setup()
{
	Serial.begin(9600);
	Wire.begin();
	setTime(5, 59, 55, 1, 28, 5, 17);// 00h:00p:00s // thu-00n-00t-0000N
	lcd.begin(16, 4);
	pinMode(relay_dao_trung, OUTPUT);
	pinMode(relay_den_nhiet_do, OUTPUT);
	pinMode(relay_den_do_am, OUTPUT);
	pinMode(relay_quat_hut, OUTPUT);
}

void loop()
{
	DHT_GetTemHumi(HNhietDo, LNhietDo, HDoAm, LDoAm); // doc nhien do / do am 
	readDS1307(); // doc thoi gian tu ic 1307
	in_lcd();     // in thoi gian / nhiet do ra man lcd
	dao_trung();  // dao trung 
	temperature_control();
	humidity_control();
	digitalClockDisplay(); // in thoi gian ra serial
	delay(100);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void temperature_control() // điều khiển nhiệt độ 
{
	if (HNhietDo < 37) // nhỏ hơn 37 độ thì thắp sáng bóng đèn tạo nhiệt 
	{
		digitalWrite(relay_den_nhiet_do, HIGH);
	}
	else // neu = 37 độ thì tắt bóng đèn
	{
		digitalWrite(relay_den_nhiet_do, LOW);
	}

	if (HNhietDo > 37) // lớn hơn 37 độ thì chạy quạt hút 
	{
		digitalWrite(relay_quat_hut, HIGH);
	}
	else // nếu  = 37 độ thì bắt quạt hút 
	{
		digitalWrite(relay_quat_hut, LOW);
	}
}

void humidity_control() // điều khiển độ ẩm 
{
	if (HDoAm <= 55)
	{
		digitalWrite(relay_den_do_am, HIGH);
	}
	else
	{
		digitalWrite(relay_den_do_am, LOW);
	}
}

void in_lcd()
{
	lcd.setCursor(0, 0);
	lcd.print("TIME:");
	lcd.println(gio);
	lcd.print(":");
	lcd.println(phut);
	lcd.print(":");
	lcd.println(giay);

	/*
	lcd.setCursor(0,1);
	switch (thu)
	{
	case 1:
	lcd.print("Monday");
	break;
	case 2:
	lcd.print("Tuesday");
	break;
	case 3:
	lcd.print("Wednesday");
	break;
	case 4:
	lcd.print("Thursday");
	break;
	case 5:
	lcd.print("Friday");
	break;
	case 6:
	lcd.print("Saturday");
	break;
	case 7:
	lcd.print("Sunday");
	break;
	}
	lcd.print("/");
	lcd.print(ngay);
	lcd.print("/");
	lcd.print(thang);
	lcd.print("/");
	lcd.print(nam);

	*/

	lcd.setCursor(0, 1);
	lcd.print("NHIET DO:");
	lcd.print(HNhietDo);
	lcd.print(",");
	lcd.print(LNhietDo);

	lcd.setCursor(0, 2);
	lcd.print("DO AM:");
	lcd.print(HDoAm);
	lcd.print(",");
	lcd.print(LDoAm);

}
//-------------------------------------------------
void dao_trung()
{
	lcd.setCursor(0, 3);
	switch (phut)
	{
	case 0:
		if ((giay > 0) && (giay < 15))
		{
			lcd.print("dao trung lan 1");
			digitalWrite(relay_dao_trung, HIGH);
		}
		else
		{
			digitalWrite(relay_dao_trung, LOW);
		}
		break;
	case 1:

		if ((giay > 0) && (giay < 15))
		{
			lcd.print("dao trung lan 2");
			digitalWrite(relay_dao_trung, HIGH);
		}
		else
		{
			digitalWrite(relay_dao_trung, LOW);
		}
		break;
	case 2:

		if ((giay > 0) && (giay < 15))
		{
			lcd.print("dao trung lan 3");
			digitalWrite(relay_dao_trung, HIGH);
		}
		else
		{
			digitalWrite(relay_dao_trung, LOW);
		}
		break;
	case 3:

		if ((giay > 0) && (giay < 15))
		{
			lcd.print("dao trung lan 4");
			digitalWrite(relay_dao_trung, HIGH);
		}
		else
		{
			digitalWrite(relay_dao_trung, LOW);
		}
		break;
	}
}

//-------------------------------------------
void readDS1307()
{
	Wire.beginTransmission(add_DS1307);
	Wire.write((byte)0x00);
	Wire.endTransmission();
	Wire.requestFrom(add_DS1307, byte_data);

	giay = bcd2dec(Wire.read() & 0x7f);
	phut = bcd2dec(Wire.read());
	gio = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
	thu = bcd2dec(Wire.read());
	ngay = bcd2dec(Wire.read());
	thang = bcd2dec(Wire.read());
	nam = bcd2dec(Wire.read());
	nam = nam + 2000;

}

int bcd2dec(byte num)
{
	return ((num / 16 * 10) + (num % 16));
}

int dec2bcd(byte num)
{
	return ((num / 10 * 16) + (num % 10));
}

void digitalClockDisplay()
{
	Serial.print(gio);
	Serial.print(":");
	Serial.print(phut);
	Serial.print(":");
	Serial.print(giay);
	Serial.print("---");
	//----------------------
	Serial.print(thu);
	Serial.print("/");
	Serial.print(ngay);
	Serial.print("/");
	Serial.print(thang);
	Serial.print("/");
	Serial.print(nam);
	Serial.println();
}

void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
	Wire.beginTransmission(add_DS1307);
	Wire.write(byte(0x00));
	Wire.write(dec2bcd(sec));
	Wire.write(dec2bcd(min));
	Wire.write(dec2bcd(hr));
	Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
	Wire.write(dec2bcd(d));
	Wire.write(dec2bcd(mth));
	Wire.write(dec2bcd(yr));
	Wire.endTransmission();
}

//-------------------------------------------------------------------------

byte DHT_GetTemHumi(byte &tem1, byte &tem2, byte &humi1, byte &humi2)
{
	byte buffer[5] = { 0,0,0,0,0 };
	byte ii, i, checksum;

	pinMode(DHT_DATA, OUTPUT);//set la cong ra
	digitalWrite(DHT_DATA, HIGH);//binh thuong khi chua giao tiep voi cam bien dht11 thi chan tin hieu lun o muc cao
	delayMicroseconds(60);//cho doi tam 60 micro giay
	digitalWrite(DHT_DATA, LOW);//gui tin hieu bat dau cho cam bien, va mat tam it nhat la 18ms
	delay(25);// it nhat 18ms, de bao dam rang DHT phat hien duoc tin hieu bat dau nay
	digitalWrite(DHT_DATA, HIGH);//sau do MCU se keo tin hieu nay len cao va cho tu 20us-40us de DHT phan hoi lai    
	pinMode(DHT_DATA, INPUT);//set la cong vao
	delayMicroseconds(60);//cho doi tam 60 micro giay
	if (digitalRead(DHT_DATA) == 1) return DHT_ER;//neu tin hieu nay van o muc cao nghia la DHT khong phan hoi thi bao loi
	else
	{
		while (digitalRead(DHT_DATA) == 0);//sau khi DHT11 phan hoi thi no se keo tin hieu nay ve muc thap tam 80us
	}
	delayMicroseconds(60);//sau do DHT11 lai chuyen tin hieu nay tu muc thap len muc cao tam 80us de chuan bi bat dau gui du lieu cho MCU
	if (digitalRead(DHT_DATA) == 0) return DHT_ER;//neu duong tin hieu van o muc cao sau 80us thi bao loi
	else
	{
		while (digitalRead(DHT_DATA) == 1);//cho duong tin hieu nay ve muc thap de bat dau gui du lieu cho MCU
	}

	for (i = 0;i<5;i++)
	{
		for (ii = 0;ii<8;ii++)
		{
			while (digitalRead(DHT_DATA) == 0);//doi data len 1
			delayMicroseconds(50);//kiem tra do dai bit 0 hay la 1
			if (digitalRead(DHT_DATA) == 1)
			{
				buffer[i] |= (1 << (7 - ii));
				while (digitalRead(DHT_DATA) == 1);//doi data xuong 0
			}
		}
	}
	checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
	if ((checksum) != buffer[4])return DHT_ER;
	tem1 = buffer[2];//gia tri phan nguyen
	tem2 = buffer[3];//gia tri phan thap phan
	humi1 = buffer[0];//gia tri phan nguyen
	humi2 = buffer[1];//gia tri phan thap phan
	return DHT_OK;
}
