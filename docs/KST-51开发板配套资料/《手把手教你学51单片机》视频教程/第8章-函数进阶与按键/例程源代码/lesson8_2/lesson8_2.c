unsigned char add(unsigned char x, unsigned char y);

void main()
{
	unsigned char a = 1;
	unsigned char b = 2;
	unsinged char c = 0;

	c = add(a, b);

	while(1);
}

unsigned char add(unsigned char x, unsigned char y)
{
	unsigned char z = 0;

	z = x + y;
	return z;
}
