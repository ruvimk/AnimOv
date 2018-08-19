#include <stdio.h> 

int main (){ 
	int i; 
	int j; 
	for (i= 0; i < 5; i++){ 
		for (j= 0; j < 5; j++){ 
			if (j == 2) break; 
			printf ("%d * %d = %d \r\n", i, j, i * j); 
		} 
	} 
} 