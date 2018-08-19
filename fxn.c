double stack[512]; 
int sp= 512; 

void push (double number){ 
	sp--; 
	stack[sp]= number; 
} 
void pop (double * number){ 
	*number= stack[sp]; 
	sp++; 
} 

void smul (){ 
	double a; 
	double b; 
	pop (&b); 
	pop (&a); 
	push (a * b); 
} 
void sdiv (){ 
	double a; 
	double b; 
	pop (&b); 
	pop (&a); 
	if (!b) printf ("Warning:  Division by 0. \r\n"); 
	push (a / b); 
} 
void sadd (){ 
	double a; 
	double b; 
	pop (&b); 
	pop (&a); 
	push (a + b); 
} 
void ssub (){ 
	double a; 
	double b; 
	pop (&b); 
	pop (&a); 
	push (a - b); 
} 
void spow (){ 
	double a; 
	double b; 
	pop (&b); 
	pop (&a); 
	push (pow (a, b)); 
} 
void ssqrt (){ 
	double a; 
	pop (&a); 
	push (sqrt (a)); 
} 

int NewtonsMethodCubicZerosInteger (double a, double b, double c, double d, double start){ 
	int r1; 
	int r2= (int)(start); 
	double x0; 
	double x1= start; 
	// r1 = 46 
	// r2 = 30 
	// x0 = 46.4393 
	// x1 = 30.729 
	do { 
		x0= x1; 
		x1= x0 - (a * pow (x0, 3) + b * pow (x0, 2) + c * x0 + d) / (3 * a * pow (x0, 2) + 2 * b * x0 + c); 
		r1= r2; 
		r2= (int)(x1); 
	} while ((int)(abs (x0 - x1) * 2)); 
	printf ("Exact Answer: %f\r\n", x1); 
	return r2; 
} 

double CubicRoots (double a, double b, double c, double d, double * r0, double * r1, double * r2){ 
	double temp; 
	push (27); 
	push (a); 
	push (a); 
	smul (); 
	smul (); 
	push (d); 
	push (d); 
	smul (); 
	smul (); 
	push (4 * pow (b, 3) - 18 * a * b * c); 
	push (d); 
	smul (); 
	sadd (); 
	push (4 * a * c * c * c); 
	sadd (); 
	push (b * b * c * c); 
	ssub (); 
	ssqrt (); 
	push (2 * pow (3, 3 / 2) * a * a); 
	sdiv (); 
	push (27 * a * a * d); 
	push (9 * a * b * c); 
	ssub (); 
	push (2 * b * b * b); 
	sadd (); 
	push (54 * a * a * a); 
	sdiv (); 
	ssub (); 
	push (1 / 3); 
	spow (); 
	pop (&temp); 
	push (temp); 
	push (3 * a * c); 
	push (b * b); 
	ssub (); 
	push (9 * a * a); 
	push (temp); 
	smul (); 
	sdiv (); 
	ssub (); 
	pop (r0); 
	printf ("End of Operation; Stack Pointer: %d\r\n", sp); 
	*r1= -65536; 
	*r2= -65536; 
	return *r0; 
} 