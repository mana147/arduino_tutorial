unsigned int Gia_tri_moi;
void setup()
{
	TCCR1A = 0;
	TCCR1B = 0;
	// set lại 2 thanh ghi
	DDRB |= (1 << PB1);
	// đầu ra pin 9 / PB1 / OC1A / PCINT1
	TCCR1A |= (1<< WGM11)| (1 << COM0A1);
	TCCR1B |= (1<< WGM12) | (1 << WGM13);
	TCCR1B |= (1 << CS10) | (1 << CS11);
	
	ICR1 = 500;
	OCR1A = 0;
}

void set(unsigned int x)
{
	if (Gia_tri_moi != x)
	{
		OCR1A = x;
		Gia_tri_moi = OCR1A;
	}
	else
	{
		return;
	}
}

void loop()
{
	int y = 100;
	set(y);	
}
