#include <stdio.h> 
#include <math.h> 

int main (){ 
	double a; 
	double b; 
	double c; 
	printf ("Enter a: "); 
	scanf ("%lf", &a); 
	printf ("Enter b: "); 
	scanf ("%lf", &b); 
	c= pow (a, 1 / b); 
	printf ("Result: %f\r\n", c); 
} 