// cipher.c

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// static const char pass[] = "witch";		// 10 chars

int char2hex(char c){
	switch(c){
		case '0':	return 0;
		case '1':	return 1;
		case '2':	return 2;
		case '3':	return 3;
		case '4':	return 4;
		case '5':	return 5;
		case '6':	return 6;
		case '7':	return 7;
		case '8':	return 8;
		case '9':	return 9;
		case 'a':	return 10;
		case 'b':	return 11;
		case 'c':	return 12;
		case 'd':	return 13;
		case 'e':	return 14;
		case 'f':	return 15;
		default:	return -1;
	}
		// Doesn't work for some reason
	// char* cstar = malloc(sizeof(char));
	// strncpy(cstar,&c,1);
	// return atoi(cstar);
}

char hex2char(int h){
	switch(h){
		case 0  :	return '0';
		case 1  :	return '1';
		case 2  :	return '2';
		case 3  :	return '3';
		case 4  :	return '4';
		case 5  :	return '5';
		case 6  :	return '6';
		case 7  :	return '7';
		case 8  :	return '8';
		case 9  :	return '9';
		case 10 :	return 'a';
		case 11 :	return 'b';
		case 12 :	return 'c';
		case 13 :	return 'd';
		case 14 :	return 'e';
		case 15 :	return 'f';
		default:	return -1;
	}
}


/*
Converts key to writable format
*/
char* convertKey(char* key, int len){
	int i;
	for(i=0;i<len;i++){
		// printf("%x %x\n", key[i],hex2char(((int) key[i])));
		key[i] = hex2char(key[i]);
	}
	return key;
}

int find(char* buf, int bufsize, char c){
	int i;
	for(i=0;i<bufsize;i++) {
		// printf("[%d]", i);
		// printf("[%d,%x]",i,buf[i]&0xff);
		// if((buf[i]&0xff)==(c&0xff)) return i;
		if((buf[i])==(c)) return i;
	}
		// if(buf[i]==c) return i;
	return -1;
}

// int find(int* buf, int bufsize, int c){
// 	int i;
// 	for(i=0;i<bufsize;i++)
// 		if((buf[i]&0xff)==(c&0xff)) return i;
// 	return -1;
// }


/*
Expects a password less than 16 chars long
returns a 4x4 nibble matrix
The nibbles should not repeat. Assert 16 unique values
*/
void keygen(char* key, char* pw, int pwlen){
	// assert(strlen(pw)==pwlen);
	assert(pwlen<16);
	int i,j;
	for(i=0;i<16;i++)
		key[i] = (-1&0xff);
	char xor_, xor_prev;
	for(i=0,j=0;i<pwlen;i++){
		char xor_ = (pw[i] & 0xf)^(pw[i]>>4);
			// search if we have seen this char before
		// printf("xor_: %x\n", xor_);
		if(find(key,16,xor_)!=-1) {
			// printf("xor in while loop\n");
			while(find(key,16,xor_)!=-1) {
				xor_++;
				xor_%=0xf;
				xor_&=0xf;
				// printf("\t%x\n", xor_);
			}
			// key[i] = ((xor_&0xf)+(j++))%15;	// increment by one if so
			key[i] = ((xor_%15)&0xf);
		}
		// if(find(key,16,xor_)==-1) key[i] = (xor_&0xf);		// add as usual
		else key[i] = (xor_&0xf);		// add as usual
		// printf("key[i]: %x\n\n", key[i]);
	}
	for(j=0,i=pwlen;i<16;i++){
		while(find(key,16,j)!=-1) j++;
		// printf("%x\n", j);
		key[i] = j&0xf;
	}
}


/*
prints the key in 4x4 format
*/
void printkey(char* key){
	int i;
	for(i=0;i<16;i++)
		(i % 4 == 3) ? printf("%x\n", (key[i]&0xff)) : printf("%x", (key[i]&0xff));
	printf("\n");
}


/*
encryption without chaining
expects
4x4 key
Then substitutes bytes in the plaintext using the key
*/
char* encrypt(char* key, char* plaintext, int textlen){
	char *ciphertext = (char*) malloc(sizeof(char) * textlen);
	int i;

	char a,b, a_,b_;	// a: MS nibble, b: LS nibble of char
	int ai,aj,ak;		// ai, aj: 2D coords, ak: 1D index
	int bi,bj,bk;		// bi, bj: 2D coords, bk: 1D index
	int coldiff;		// abs(bj-aj) difference of columns

	for(i=0;i<textlen;i++){
		a = (plaintext[i]>>4)&0xf; b = (plaintext[i]&0xf);
		ak = find(key,16,a);
		bk = find(key,16,b);
		ai = ak/4; bi = bk/4;
		aj = ak%4; bj = bk%4;
		coldiff = abs(bj-aj);

		// printf("plaintext[1]: %x\na:  %x, b: %x\n",plaintext[i], a,b);
		// printf("ak: %d, bk %d\n", ak,bk);
		// printf("ai: %d, bi %d\n", ai,bi);
		// printf("aj: %d, bj %d\n", aj,bj);
		// printf("a_: %x, b_: %x\n\n", a_,b_);


		if(ai==bi){							//same row
			a_ = key[ai*4 + ((aj+1 > 3) ? 0 : aj+1)];
			b_ = key[bi*4 + ((bj+1 > 3) ? 0 : bj+1)];
		}
		else if(aj==bj){					//same column
			a_=key[(((ai+1)%4)*4+aj)];
			b_=key[(((bi+1)%4)*4+bj)];
		}
		else if(aj>bj){						//first char to the right of the second char
			a_ = key[ak - coldiff];
			b_ = key[bk + coldiff];
		}
		else if(bj>aj){						//second char to the right of the first char
			a_ = key[ak + coldiff];
			b_ = key[bk - coldiff];
		}
		ciphertext[i] = ((a_<<4)|b_)&0xff;
	}
	return ciphertext;
}


/*
decryption without chaining
expects
4x4 key
Then substitutes bytes in the cipher using the key
NOTE: length of cipher should equal length of plaintext
*/
char* decrypt(char* key, char* ciphertext, int textlen){
	char *plaintext = (char*) malloc(sizeof(char) * textlen);
	int i,j;


	char a,b, a_,b_;	// a: MS nibble, b: LS nibble of char
	int ai,aj,ak;		// ai, aj: 2D coords, ak: 1D index
	int bi,bj,bk;		// bi, bj: 2D coords, bk: 1D index
	int coldiff;		// abs(bj-aj) difference of columns

	for(i=0;i<textlen;i++){
		a = (ciphertext[i]>>4)&0xf; b = (ciphertext[i]&0xf);
		ak = find(key,16,a);
		bk = find(key,16,b);
		ai = ak/4; bi = bk/4;
		aj = ak%4; bj = bk%4;
		coldiff = abs(bj-aj);
		// printf("ciphertext[%d]: %x\na:  %x, b: %x\n",i,ciphertext[i], a,b);
		// printf("ak: %d, bk %d\n", ak,bk);
		// printf("ai: %d, bi %d\n", ai,bi);
		// printf("aj: %d, bj %d\n", aj,bj);

		if(ai==bi){							//same row
			a_ = key[ai*4 + ((aj-1 < 0) ? 3 : aj-1)];
			b_ = key[bi*4 + ((bj-1 < 0) ? 3 : bj-1)];
		}
		else if(aj==bj){					//same column
			a_=key[( ((ai-1) < 0) ? 3:ai-1 )*4+aj];
			b_=key[( ((bi-1) < 0) ? 3:bi-1 )*4+bj];
		}
		else if(aj>bj){						//first char to the right of the second char
			a_ = key[ak - coldiff];
			b_ = key[bk + coldiff];
		}
		else if(bj>aj){						//second char to the right of the first char
			a_ = key[ak + coldiff];
			b_ = key[bk - coldiff];
		}
		// printf("a_: %x, b_: %x\n\n", a_,b_);
		// printf("%d, %d, %x\n", i,((a_<<4)|b_)&0xff,(int)(((a_<<4)|b_)&0xff) < 0x7f);
		// assert((int)(((a_<<4)|b_)&0xff) < 128);
		plaintext[i] = ((a_<<4)|b_)&0xff;
	}
	return plaintext;
}


/*
Adds additional characters to the end of the plaintext to meet the nearest blocksize

Can specify padding character
*/
char* pad(char* text, int length, int blocksize, char padchar){
	int i;
	// printf("%d\n", length % blocksize);
	if(length%blocksize==0){
		char* padded = (char*) malloc(sizeof(char) * length);
		strcpy(padded,text);
		return padded;
	}  
	int padding = blocksize - length % blocksize;
	char* padded = (char*) malloc(sizeof(char) * (length + padding));
	strcpy(padded,text);

	for(i=length; i<length + padding; i++ )
		padded[i] = padchar;
	// printf("%s\n", padded);
	return padded;
}


/*
Divides the plaintext into blocks of size blocksize for use in 
chaining encryption
*/
char** blocky(char *text, int length, int blocksize,char padchar){
	int i,j;
	int numblocks = ceil(((double) length)/((double) blocksize));
	// printf("%d\n", numblocks);
	char* padded = pad(text,length,blocksize,padchar);
	char** blocks = (char**) malloc(sizeof(char*)*numblocks);
	for(i=0; i<numblocks; i++ ){
		blocks[i] = (char*) malloc(sizeof(char)*blocksize);
		for(j=0;j<blocksize;j++)
			blocks[i][j] = padded[i*blocksize+j]&0xff;
	}

	return blocks;
}


/*
	Converts 4 char arrays to integers for ease of printing or XORing
*/
int func_4char2int(char* block,int blocksize){
	assert(blocksize%2==0);
	assert(blocksize<=4);

	int i,val=0;
	for(i=0;i<sizeof(int);i++){
		val <<= 8;
		val |= block[i]&0xff;
		// printf("%x\n", block[i], block[i]);
	}
	// printf("%x\n", val);
	return val;
}


/*
	Converts integers to 4 char arrays to store back
*/
char* func_int_2_4char(int val){
	int i;
	char* block = (char*) malloc(sizeof(char)*sizeof(int));
	for(i=sizeof(int)-1;i>=0;i--){
		block[i] = val & 0xff;
		val >>= 8;
		// printf("%x,%c\n", block[i], block[i]);
	}
	// printf("%s\n", block);
	return block;
}

/*
	Quick function to evaluate the XOR of two chars
*/
char* xor4char(char* t1, char* t2){
	return func_int_2_4char(func_4char2int(t1,4) ^ func_4char2int(t2,4));
}


/*
	Encrypt using chaining mode
	Uses 4x4 key
		plaintext of length x char
		blocksize: restricted to 4 for NIOS
	Resulting cipher is length + 4 to accommodate initialization vector
*/
char* encryptCBC(char* key, char* plaintext, int length, int blocksize){
	int i;
	int numblocks = ceil(((double) length)/((double) blocksize));

	// Open a random byte
	FILE *fp = fopen("/dev/random", "r");

	int IV = 0;
	for(i=0;i<sizeof(int);i++){  // fill initialization vector with values from CSPRNG
		IV <<= 8;
		IV |= fgetc(fp);
		// printf("IV: %x\n", IV);
	}
	// IV = 0x6aa29ebe; // fixed IV; for testing

	char** blocks = blocky(plaintext,length,blocksize,'~');
	// int numblocks = (length%blocksize==0) ? length/blocksize : length/blocksize+1;
	// for(i=0;i<numblocks;i++)
	// 	printf("[%s]", blocks[i]);
	// printf("\n\n");
	char *cipher = malloc(sizeof(char)*(numblocks+1)*blocksize);
	char *cprev = malloc(sizeof(char)*blocksize);
	char *p = malloc(sizeof(char)*blocksize);
	char *c = malloc(sizeof(char)*blocksize);

	strncpy(cipher,func_int_2_4char(IV),4); // initialize: C_0 = IV

	for(i=1;i<numblocks+1;i++){ // start at i = 1 with P_1, C_1
		strncpy(cprev,cipher+(i-1)*blocksize,blocksize); // C_{i-1}
		// printf("%x\n", func_4char2int(cprev,blocksize));
		strcpy(p,blocks[i-1]); 							// P_i
		c = encrypt(key,xor4char(p,cprev),blocksize);	// C_i = P_i xor C_{i-1}
		strncpy(cipher+i*blocksize,c,blocksize);	// append cipher block to ciphertext
		strcpy(cprev,c);							// update C_{i-1}
		// printf("%x\n", func_4char2int(c,blocksize));
	}

	return cipher;
}


/*
	Decrypt a chaining mode cipher
	Uses 4x4 key
		ciphertext -- IV should be the first 4 bytes of the array
		length argument is the plaintext length -- that's: ciphertext.length - 4
		blocksize = 4
	Outputs a printable char array (may contain padding characters)
*/
char* decryptCBC(char* key, char* ciphertext, int plaintxt_len, int blocksize){
	int i;
	int numblocks = ceil(((double) plaintxt_len)/((double) blocksize));

	char* plaintext = malloc(sizeof(char)*plaintxt_len);
	char *cprev = malloc(sizeof(char)*blocksize);
	char *p = malloc(sizeof(char)*blocksize);
	char *d = malloc(sizeof(char)*blocksize);

	// int IV = 0x6aa29ebe; // fixed IV; for testing
	// strncpy(ciphertext,func_int_2_4char(IV),4); // initialize: C_0 = IV


	for(i=1;i<numblocks+1;i++){		// start at i=1 with C_0 = IV, P_1, C_1
		strncpy(cprev,ciphertext+(i-1)*blocksize,blocksize);	// C_{i-1}
		d = decrypt(key,ciphertext+i*blocksize,blocksize);		// D_K(C_i)
		// printf("%x\n", func_4char2int(d,blocksize));		
		p = xor4char(d,cprev);								// P_i = C_{i-1} xor D_K(C_i)
		strncpy(cprev,ciphertext+i*blocksize,blocksize);	// update C_{i-1}
		strncpy(plaintext+(i-1)*blocksize,p,blocksize);		// append P_i
	}
	return plaintext;
}


/*
	Prints byte stream in terminal viewable format

	ie converts each byte to two hex digits
	eg [0xef] --> [0xe][0xf]
*/
void printCipher(char* cipher, int length){ 	// print for now
	int i;
	char* new_cipher = malloc(sizeof(char)*length);
	// new_cipher = convertString(cipher,length);
	// for(i=0;i<length;i++)
	// 	printf("[%x]", new_cipher[i]&0xff);
	for(i=0;i<length;i++)		// add zeros to single digit chars
		((int)(cipher[i]&0xff) < 16) ? printf("0%x", cipher[i]&0xff) : printf("%x", cipher[i]&0xff);	
		// ((int)(cipher[i]&0xff) < 16) ? printf("[0%x]", cipher[i]&0xff) : printf("[%x]", cipher[i]&0xff);	
}


/*
	Outputs byte stream in saveable format -- to be later parsed

	ie converts each byte to two hex digits
	eg [0xef] --> [0xe][0xf]

	output length should be twice the cipher length
*/
char* writeCipher(char* cipher, int length){ 	// print for now
	int i;
	// char* new_cipher = malloc(sizeof(char)*length);
	// new_cipher = convertString(cipher,length);
	// for(i=0;i<length;i++)
	// 	printf("[%x]", new_cipher[i]&0xff);
	// printf("%c\n", hex2char(0xe));
	char* doublecipher = malloc(sizeof(char)*length*2);
	for(i=0;i<length;i++) {		// add zeros to single digit chars
		printf("[%x]", cipher[i]&0xff);
		doublecipher[2*i] = hex2char((cipher[i]>>4)&0xf);
		doublecipher[2*i+1] = hex2char(cipher[i]&0xf);
		// ((int)(cipher[i]&0xff) < 16) ? printf("0%x", cipher[i]&0xff) : printf("%x", cipher[i]&0xff);
		// ((int)(cipher[i]&0xff) < 16) ? printf("[0%x]", hex2char(cipher[i]&0xf)) : printf("[%x%x]", hex2char(cipher[i]>>4),hex2char(cipher[i]&0xf));	
		// ((int)(new_cipher[i]&0xff) < 16) ? printf("0%x", new_cipher[i]&0xff) : printf("%x", new_cipher[i]&0xff);	
	}
	return doublecipher;
}


/*
	Converts byte stream back to original hex values (max 0xff ie in [0,255]) 
	for decryption. Decrypter will not understand cipher if this is not used!

	ie converts each byte to two hex digits
	eg [0xef] --> [0xe][0xf]

	cipher length should be half the input length
*/
char* parseCipher(char* input, int inputlength){
	assert(inputlength%2==0);
	int i; 
	char *cipher = malloc(sizeof(char) * (inputlength/2));
	for(i=0; i<inputlength; i+=2)
		cipher[i/2] = (char2hex(input[i]) << 4) | char2hex(input[i+1]);
	return cipher;
}

int main(){
	// Sample input
	char pass[] = "witch";		//5
	char pass2[] = "IT(cwe)";	//7
	char pass3[] = "kkkkkkkkkkkkkkk";  //15
	char pass4[] = "hihihihihihihi";  //14
	char plaintext[] = "hello world"; //11
	char plaintext2[] = "The quick brown fox jumps over the lazy dog. hello world."; //44
	int textlength = 11;
	int textlength2 = 57;
	// block size should be 4 for 32bit machines
	int blocksize = 4;
	char key[16];
	int i,j;


	// Test key generation scheme
	printf("show pretty key:\n");
	keygen(key,pass4,14);
	printkey(key);			// prints as square matrix
	// Print inline
	for(i=0;i<16;i++)
		printf("%x", key[i]);
	printf("\n\n");


	printf("Simple encryption and decryption (without chaining)\n");
	char *cipher = encrypt(key,plaintext2,textlength2);
	char *plaintest = decrypt(key,cipher,textlength2);
	// Check cipher bytes (not printable on most terminals)
	for(i=0;i<textlength2;i++)
		printf("[%x]",cipher[i]&0xff);
	printf("\n");
	// Check decrypted plaintext (expected to be printable)
	printf("%s\n", plaintest);

	// check plaintext bytes
	// for(i=0;i<textlength2;i++)
	// 	printf("%x",plaintest[i]);
	printf("\n\n");



	char** blocks = blocky(plaintext2,textlength2,blocksize,'~');
	int numblocks = (textlength2%blocksize==0) ? textlength2/blocksize : textlength2/blocksize+1;

	printf("test block generator: \n"); // for use in chaining
	for(i=0;i<numblocks;i++)		// show partitions
		printf("[%s]", blocks[i]); 
	printf("\n\n");


	printf("Start chaining tests:\n");
	cipher = encryptCBC(key,plaintext2,textlength2,blocksize);

	printf("Print CBC mode cipher bytes:\n");
	for(i=0;i<(numblocks+1)*blocksize;i++)
		printf("[%x]", cipher[i]&0xff);
	printf("\n\n");
	
	plaintest = decryptCBC(key, cipher, textlength2, blocksize);
	// Check plaintext output
	printf("plain: %s\n\n", plaintest);


	printf("test write: filling zeros where needed\n");
	// for(i=0;i<(numblocks+1)*blocksize;i++){
	// 	if((int)(cipher[i]&0xff) < 16)			// add zeros to single digit chars
	// 		// printf("[0%x]", cipher[i]&0xff);	// highlight single digit problematic output
	// 		printf("0%x", cipher[i]&0xff);
	// 	else printf("%x", cipher[i]&0xff);	
	// }
	printCipher(cipher,(numblocks + 1)*blocksize); // the above tests distilled into a function

	// Test file IO wrapper functions
	char *cipher_write_test = malloc(sizeof(char)*2*(numblocks + 1)*blocksize);

		// tests for routines later built into function: hex2char 
	// printf("[%c,%c]", hex2char((cipher[i]>>4)&0xf),hex2char(cipher[i]&0xf));
	// doublecipher[i] = hex2char((cipher[i]>>4)&0xf);
	// doublecipher[i+1] = hex2char(cipher[i]&0xf);
	// for(i=0;i<2*(numblocks + 1)*blocksize;i++);
	printf("\nwrite test 2:\n");
		// tests to determine indexing relations
	// for(i=0;i<(numblocks + 1)*blocksize;i++){
	// 	printf("[%x]", cipher[i]&0xff);
	// 	cipher_write_test[2*i] = hex2char((cipher[i]>>4)&0xf);
	// 	cipher_write_test[2*i+1] = hex2char(cipher[i]&0xf);
	// }

	// Using final function to do writing
	// since chars comprised of 2 chars to represent bytes
	// it was necessary to convert this prior to file writes
	cipher_write_test = writeCipher(cipher,(numblocks + 1)*blocksize);
	// printf("%d\n", strlen(cipher_write_test));
	printf("\n\n");
	// Check plaintext
	printf("%s\n", cipher_write_test);
	// printf("\n\n");
	// for(i=0;i<2*(numblocks + 1)*blocksize;i+=2)
	// 	printf("[%c%c]", cipher_write_test[i],cipher_write_test[i+1]);
	printf("\n\n");


	// printf("test parsing (remember to fix IV for this test)\n");
	// // char samplecipher[] = "f9f52902bad52d44dc0eee18fb41c3a0d908ba63f17838257d5a0000e66b7087c2470f35bb44134dab02a09ab39ab8d1";
	// char samplecipher[] = "6aa29ebe02db1f648f0687f61ee84092b77541c1b4eb5812a464508eb9ee5337ad8d8f25d326b09dc13dd8cad034f11e";
	// int cipherlen = strlen(samplecipher);

	// // printf("%x\n", (samplecipher[0] << 4) | samplecipher[1]);
	// printf("cipher:\t\t\t");


	// cipher = parseCipher(samplecipher, cipherlen);
	// printf("\n");
	// printf("samplecipher:\t%s\n", samplecipher);

	// printf("read test:\t\t");
	// for(i=0;i<textlength2;i++)
	// 	// printf("%x\n", func_4char2int(cipher+i,blocksize));
	// 	((int)(cipher[i]&0xff) < 16) ? printf("0%x", cipher[i]&0xff) : printf("%x", cipher[i]&0xff);
	// printf("\n");

	// plaintest = decryptCBC(key,cipher,textlength2,blocksize);
	printf("plaintext:\t\t%s\n", plaintest);

	// printf("\n");

} ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GRAVEYARD: various tests for intermediary functions 
	// char a,b, a_,b_;		// a: MS nibble, b: LS nibble of char
	// int ai,aj,ak;	// ai, aj: 2D coords, ak: 1D index
	// int bi,bj,bk;	// bi, bj: 2D coords, bk: 1D index
	// int coldiff;	// abs(bj-aj) difference of columns

	// i=0;
	// a = (plaintext[i]>>4); b = (plaintext[i]&0xf);
	// ak = find(key,16,(plaintext[i]>>4));
	// bk = find(key,16,(plaintext[i]&0xf));
	// ai = ak/4; bi = bk/4;
	// aj = ak%4; bj = bk%4;
	// coldiff = abs(bj-aj);

	// // printf("a:  %x, b: %x\n", a,b);
	// printf("plaintext[1]: %c, %x\na:  %x, b: %x\n",plaintext[i],plaintext[i], a,b);
	// printf("ak: %d, bk %d\n", ak,bk);
	// printf("ai: %d, bi %d\n", ai,bi);
	// printf("aj: %d, bj %d\n", aj,bj);

	// a_ = key[ai*4 + ((aj-1 > 3) ? 0 : aj+1)];
	// b_ = key[bi*4 + ((bj-1 > 3) ? 0 : bj+1)];

	// printf("a_: %x, b_: %x\n\n", a_,b_);
	
	// i=10;
	// a = (cipher[i]>>4)&0xf; b = (cipher[i]&0xf);
	// ak = find(key,16,a);
	// bk = find(key,16,b);
	// ai = ak/4; bi = bk/4;
	// aj = ak%4; bj = bk%4;
	// coldiff = abs(bj-aj);

	// printf("cipher[i]: %x\na:  %x, b: %x\n",cipher[i], a,b);
	// printf("ak: %d, bk %d\n", ak,bk);
	// printf("ai: %d, bi %d\n", ai,bi);
	// printf("aj: %d, bj %d\n", aj,bj);

	// if(ai==bi){
	// 	a_ = key[ai*4 + ((aj-1 < 0) ? 3 : aj-1)];
	// 	b_ = key[bi*4 + ((bj-1 < 0) ? 3 : bj-1)];
	// }
	// else if(aj==bj){
	// 	a=key[(((ai-1)%4)*4+aj)];
	// 	b=key[(((bi-1)%4)*4+bj)];
	// }
	// else if(aj>bj){
	// 	a_ = key[ak - coldiff];
	// 	b_ = key[bk + coldiff];
	// }
	// else if(bj>aj){
	// 	a_ = key[ak + coldiff];
	// 	b_ = key[bk - coldiff];
	// }

	// printf("a_: %x, b_: %x, plain: %c.\n\n", a_,b_,((a_<<4)|b_)&0xff);

	// 	printf("a_: %x, b_: %x\n\n", a_,b_);
	// 	printf("plain char: %c, cipher char: %x\n",(a<<4)| b, (a_<<4)|b_);
	// }
	// // i=5;
	// // a = (plaintext[i]>>4); b = (plaintext[i]&0xf);
	// a = 3; b = 8;
	// ak = find(key,16,a);
	// bk = find(key,16,b);
	// ai = ak/4; bi = bk/4;
	// aj = ak%4; bj = bk%4;
	// coldiff = abs(bj-aj);

	// // printf("plaintext[1]: %c, %x\na:  %x, b: %x\n",plaintext[i],plaintext[i], a,b);
	// printf("%x,%x\n", a,b);
	// printf("ak: %d, bk %d\n", ak,bk);
	// printf("ai: %d, bi %d\n", ai,bi);
	// printf("aj: %d, bj %d\n", aj,bj);

	// printf("%d,%d\n", ak + coldiff,bk - coldiff);
	// printf("%x,%x\n", a,b);
	// printf("%x,%x\n", key[(((ai+1)%4)*4+aj)],key[(((bi+1)%4)*4+bj)]);

		// if(ai==bi){					//same row
		// 	a_ = key[ai*4 + ((aj-1 > 3) ? 0 : aj+1)];
		// 	b_ = key[bi*4 + ((bj-1 > 3) ? 0 : bj+1)];
		// }
		// else if(aj==bj){			//same column
		// 	a=key[(((ai+1)%4)*4+aj)];
		// 	b=key[(((bi+1)%4)*4+bj)];
		// }
		// else if(aj>bj){				//second char to the right of the first char
		// 	a=keystr[ak-coldiff];
		// 	b=keystr[bk+coldiff];
		// }
		// else if(bj>aj){				//first char to the right of the second char
		// 	a=key[ak + coldiff];
		// 	b=key[bk - coldiff];
		// }




	// printf("%d\n", strlen(pass));
	// printf("find: %s\n",pass);
	// printf("char: %x\n",(pass2[2] & 0xf)^(pass2[2]>>4));

	// assert(strlen(temp_pass)<16);
	// printf("char\thex\t\txor'd\tkey[i]\n");
	// for(i=0;i<strlen(temp_pass);i++){
	// 	char xor_ = (temp_pass[i] & 0xf)^(temp_pass[i]>>4);
	// 	if(find(key,16,xor_)==-1) key[i] = (xor_&0xff);
	// 	printf("%c\t\t%x\t\t%x\t\t%x\n",temp_pass[i],temp_pass[i],xor_,key[i]);
	// }
	// printf("\n");
	// for(j=0,i=strlen(temp_pass);i<16;i++){
	// 	while(find(key,16,j)!=-1) j++;
	// 	// printf("j: %d, find: %d\n",j,find(key,16,j));
	// 	key[i] = j;
	// }

	// j=0;
	// printf("%d\n", find(key,16,0));
	// while(find(key,16,j)>=0) printf("%x\n", j++);;


	// *(cipher+(numblocks+1)*4) = 0x0;
	// for(i=1;i<numblocks+1;i++){
	// 	strncpy(cprev,cipher+(i-1)*blocksize,blocksize);
	// 	// printf("%x\n", func_4char2int(cprev,blocksize));
	// 	strcpy(p,blocks[i-1]);
	// 	c = encrypt(key,xor4char(p,cprev),blocksize);
	// 	strncpy(cipher+i*blocksize,c,blocksize);
	// 	strcpy(cprev,c);
	// 	printf("%x\n", func_4char2int(c,blocksize));
	// }

	// plaintest = malloc(sizeof(char)*textlength2);
	// for(i=1;i<numblocks+1;i++){
	// 	strncpy(cprev,cipher+(i-1)*blocksize,blocksize);
	// 	d = decrypt(key,cipher+i*blocksize,blocksize);
	// 	printf("%x\n", func_4char2int(d,blocksize));
	// 	p = xor4char(d,cprev);
	// 	strncpy(cprev,cipher+i*blocksize,blocksize);
	// 	strncpy(plaintest+(i-1)*blocksize,p,blocksize);
	// }




