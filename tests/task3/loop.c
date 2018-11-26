// This is our minimal startup code (usually in _start)
asm("jal main");        // call main
asm("mv a1, x10");      // save return value in a1
asm("li a0, 10");       // prepare ecall exit
asm("ecall");           // now your simlator should stop

int sum(int arr[], int len)
{
	int _sum = 0;
	for(int i = 0; i < len; i++)
		_sum += arr[i];
	return _sum;
}

void main(void) 
{
	int len = 5;
	int arr[len];
	for(int i = 0; i < len; i++)
		arr[i] = i;
	int _sum = sum(arr, len);
}
