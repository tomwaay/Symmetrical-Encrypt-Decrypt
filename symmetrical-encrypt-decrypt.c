#include <stdio.h>
#include <string.h>

#define MAX_BUF  256

unsigned char xor(unsigned char, unsigned char);
unsigned char getBit(unsigned char, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);
unsigned char processCounter(unsigned char, unsigned char);
unsigned char processByte(unsigned char, unsigned char);
unsigned char userText[MAX_BUF];
int result[MAX_BUF];

int main()
{
	char userText[MAX_BUF];
	unsigned int result[MAX_BUF], temp = 0;
	unsigned char counter = 0b00101000;
	unsigned char key = 0b10110010; 

	char str[8];
	int  choice, size, num;

	printf("\nYou may:\n");
	printf("  (1) Encrypt a message \n");
	printf("  (2) Decrypt a message \n");
	printf("\nWhat is your selection: ");
  	fgets(str, sizeof(str), stdin);
  	sscanf(str, "%d", &choice);

	switch (choice) {

	case 1:
	    	printf("\nEnter text: \n");
	     
	    	fgets(userText, sizeof(userText), stdin);
	    	printf("\n");
	    	size = strlen(userText);

	    	for(int i = 0; i < size; i++){
	    		counter = processCounter(counter,key);
	    		counter++;
	    		key++;
	    		result[i]=processByte(userText[i], counter);	
	    	}
	    
	    	printf("\nYour encoded text is: \n");	
	    	for(int x = 0; x < size-1; x++){
	    		printf("%d ", result[x]);	
	    	}
	    	printf("\n");		
	      	break;

    	case 2:
	    	printf("\nEnterb  ciphertext: \n");
	    
	    	while(scanf("%d", &num) == 1){
	    		if(num == -1){
	    			break;
	    		}
	    		result[temp++] = num;
	    	}
	    	result[temp] = '\0';
	   
	    	for(int i = 0; i <= temp; i++){
	    		counter = processCounter(counter,key);
	    		counter++;
	    		key++;
	    		result[i]=processByte(result[i], counter);
	    	}
	    
	    	printf("\nYour decrypted text reads: \n");
	    
	    	for(int x=0; x <= temp-1; x++){
	    		printf("%c", result[x]);
	    	}
	    	printf("\n");

	      	break;

    	default:
    	
    		printf("Please enter a 1 or 2 to access the respective functions of the program");
      	break;
  }

  return 0;
}


/*
  Function:  processCounter
  Purpose:   Processes the counter to ensure that every input byte is returned as a different value
  		ex. ABBA = 35 79 28 59
       in:   counter value in which the bits will be edited
       in:   key value which determines what bits in the counter will be used to compute the new bits
   return:   Counter after the bits have been changed to ensure a secure encryption
*/
unsigned char processCounter(unsigned char counter, unsigned char key){

	unsigned char tempCounter = counter;
	int i = 7;
	int position = 7;
	
	while(i>-1){
		
		unsigned char pos1 = getBit(tempCounter, position);
		unsigned char pos2;
		if(getBit(key, position) == 1){
			if(i==7){
				pos2 = getBit(tempCounter, 0);
			}
			else{
				pos2 = getBit(tempCounter, position+1);
			}	
		}
		
		
		if(getBit(key, position) == 0){
			if(i==7){
				pos2 = getBit(tempCounter, 1);
			}
			if(i==6){
				pos2 = getBit(tempCounter, 0);
			}
			if(i<6){
				pos2 = getBit(tempCounter, position+2);
			}
		
		}
		
		int xorResult = xor(pos1, pos2);
		
		if(xorResult == 1){
			tempCounter = setBit(tempCounter, position);
		}
		else{
			tempCounter = clearBit(tempCounter, position);
		}
		
		position--;
		i--;
	}
	return tempCounter;
}

/*
  Function:  processByte
  Purpose:   Decrypts cyphertext into plaintext and vice versa
       in:   Value to be encrypted/decrypted
       in:   Counter that is used to determine what bits in the value will change.
   return:   The processed byte after encrpyting or decrypting  
*/
unsigned char processByte(unsigned char c, unsigned char counter){

	unsigned char tempByte = c;
	int modResult = counter % 9;
	
	if(modResult < 3){
		for(int i = 0; i < 7; i+=2){
			unsigned char pos1 = getBit(c,i);
			unsigned char pos2 = getBit(counter, i);
			unsigned char result = xor(pos1,pos2);
			
			if(result == 1){
				tempByte = setBit(tempByte, i);
			}
			else{
				tempByte = clearBit(tempByte, i);
			}
		}
	}
	
	if(modResult >= 3 & modResult <= 5){	
		for(int i = 1; i <= 7; i+=2){
			unsigned char pos1 = getBit(c,i);
			unsigned char pos2 = getBit(counter, i);
			unsigned char result = xor(pos1,pos2);
			
			if(result == 1){
				tempByte = setBit(tempByte, i);
			}
			else{
				tempByte = clearBit(tempByte, i);
			}
		}
	}
	
	if(modResult > 5){
		for(int i = 0; i <= 7; i++){
			unsigned char pos1 = getBit(c,i);
			unsigned char pos2 = getBit(counter, i);
			unsigned char result = xor(pos1,pos2);
			
			if(result == 1){
				tempByte = setBit(tempByte, i);
			}
			else{
				tempByte = clearBit(tempByte, i);
			}
		}
	}
	return tempByte;
}
/*
  Function:  xor
  Purpose:   preforms xor operation between two bytes
       in:   first character in the operation
       in:   second character in the operation
   return:   Result of the xor operation between the two bits
*/
unsigned char xor(unsigned char pos1, unsigned char pos2){
	int result = pos1 ^ pos2;
	return result;
	}

/*
  Function:  getBit
  Purpose:   retrieve value of bit at specified position
       in:   character from which a bit will be returned
       in:   position of bit to be returned
   return:   value of bit n in character c (0 or 1)
*/
unsigned char getBit(unsigned char c, int n)   
{ 
	return ((c & (1 << n)) >> n);
}

/*
  Function:  setBit
  Purpose:   set specified bit to 1
       in:   character in which a bit will be set to 1
       in:   position of bit to be set to 1
   return:   new value of character c with bit n set to 1
*/
unsigned char setBit(unsigned char c, int n)   
{ 
	return c | (1 << n);
}

/*
  Function:  clearBit
  Purpose:   set specified bit to 0
       in:   character in which a bit will be set to 0
       in:   position of bit to be set to 0
   return:   new value of character c with bit n set to 0
*/
unsigned char clearBit(unsigned char c, int n) 
{ 
	return c & (~(1<<n));
}
