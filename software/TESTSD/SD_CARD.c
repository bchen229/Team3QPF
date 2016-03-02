#include <stdio.h>
#include <string.h>
#include "altera_up_sd_card_avalon_interface.h"
#define FILENAME "CI32.txt"

/**
 * Writes contents of buffer into SD card with file name, fileName.
 * If no file exists, a new file will be created. Returns 0 on success,
 * -1 on failure.
 */
int writeToSD(char* buffer, char* fileName) {
	alt_up_sd_card_dev *device_reference = NULL;
	short int myFileHandle;
	int i;

	printf("Opening SDCard\n");
	if ((device_reference = alt_up_sd_card_open_dev(
			"/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL) {
		printf("SDCard Open FAILED\n");
		return -1;
	} else {
		printf("SDCard Open PASSED\n");
	}
	if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
		if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
			myFileHandle = alt_up_sd_card_fopen(fileName, false);
			if (myFileHandle == -1) {
				myFileHandle = alt_up_sd_card_fopen(fileName, true);
			}
			if (myFileHandle != -1) {
				printf("File Opened\n");
				for (i = 0; i < strlen(buffer); i++) {
					if (alt_up_sd_card_write(myFileHandle, buffer[i])
							== false) {
						printf("Error writing to file...\n");
						return -1;
					}
				}
				printf("Done!!!\n");
				alt_up_sd_card_fclose(myFileHandle);
			} else {
				printf("File NOT Opened\n");
			}
		}
	}
	return 0;
}

/**
 * Read from file fileName into buffer. Returns 0 on success, -1
 * on failure.
 */
int readFromSD(char* buffer, char* fileName, int bufferSize) {
	alt_up_sd_card_dev *device_reference = NULL;
	short int myFileHandle;
	int i;
	char tempChar[2];
	tempChar[1] = '\0';

	printf("Opening SDCard\n");
	if ((device_reference = alt_up_sd_card_open_dev(
			"/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL) {
		printf("SDCard Open FAILED\n");
		return -1;
	} else {
		printf("SDCard Open PASSED\n");
	}
	if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
		myFileHandle = alt_up_sd_card_fopen(fileName, false);
		if (myFileHandle == -1) {
			myFileHandle = alt_up_sd_card_fopen(fileName, true);
		}
		if (myFileHandle != -1) {
			printf("File Opened\n");
			for (i = 0; i < bufferSize; i++) {
				tempChar[0] = alt_up_sd_card_read(myFileHandle);
				if (tempChar[0] < 0) {
					return -1;
				}
				strcat(buffer, tempChar);
			}
			printf("Done!!!\n");
			alt_up_sd_card_fclose(myFileHandle);
		} else {
			printf("File NOT Opened\n");
		}
	}
	return 0;
}

/**
 * Format and save information to SD card in file contactInformation.txt.
 * Returns 0 on success, -1 on failure. Max character length of 512 chars.
 * Format: Name, Address, Phone Number, Emergency Contact.
 */
int formatAndSaveSD(char* name, char* address, char* phone, char* emergency) {

	char data[512] = "";
	int ret = 0;

	strcat(data, name);
	strcat(data, " ,");
	strcat(data, address);
	strcat(data, " ,");
	strcat(data, phone);
	strcat(data, " ,");
	strcat(data, emergency);
	strcat(data, " ,");

	ret += writeToSD(data, FILENAME);
	printf("saved: %s\n", data);

	ret += writeToSD(data, FILENAME);
	if (ret == 0) {
		return 0;
	}
	return -1;
}

/**
 * Takes in name, address, phone, and emergency buffers. Read holds the
 * "," delimited string to be separated.
 */
int loadInfo(char* name, char* address, char* phone, char* emergency, char* read) {

	const char delim[2] = ",";

	strcpy(name, strtok(read, delim));
	strcpy(address, strtok(NULL, delim));
	strcpy(phone, strtok(NULL, delim));
	strcpy(emergency, strtok(NULL, delim));

	return 0;
}

int main() {
	char* buffer = "HELLO WORLD";
	char read[512] = "";
	char name[100] = "";
	char address[100] = "";
	char phone[100] = "";
	char emergency[100] = "";
//	writeToSD(buffer, "test1.txt");
//	printf("done write\n");
//	readFromSD(read, "test1.txt", 256);
//	printf("done read\n");

	formatAndSaveSD("BOB", "", "(604)-123-4567", "BOB 555-5555");
	readFromSD(read, FILENAME, 512);
	printf("read: %s\n", read);
	loadInfo(name,address,phone,emergency,read);

	printf("name: %s\n", name);
	printf("address: %s\n", address);
	printf("phone: %s\n", phone);
	printf("emergency: %s\n", emergency);
	printf("done");
	return 0;
}
