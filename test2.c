#include <stdio.h> 
#include <math.h> 

#include "fxn.c" 

int main (){ 
	double a, b, c, d; 
	double number; 
	double infl; 
	int ans1, ans2, ans3; 
	printf ("Enter coefficients for a^x^3 + b*x^2 + c*x + d = 0 \r\n"); 
	printf ("in the order a b c d, with spaces or tabs as delimiters. \r\n"); 
	printf ("\r\n"); 
	scanf ("%lf %lf %lf %lf", &a, &b, &c, &d); 
	printf ("\r\nWorking... "); 
	number= sqrt (4 * b * b - 12 * a * c) / (3 * a); 
	infl= -b / (3 * a); 
	ans1= (NewtonsMethodCubicZerosInteger (a, b, c, d, infl - number)); 
	ans2= (NewtonsMethodCubicZerosInteger (a, b, c, d, infl)); 
	ans3= (NewtonsMethodCubicZerosInteger (a, b, c, d, infl + number)); 
	printf ("\r\n\r\nResults (Rounded Toward 0 to the Nearest Integer): \r\n"); 
	printf ("Root 1:  %d \r\n", ans1); 
	printf ("Root 2:  %d \r\n", ans2); 
	printf ("Root 3:  %d \r\n", ans3); 
} 