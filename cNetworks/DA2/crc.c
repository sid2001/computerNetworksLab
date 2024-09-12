#include <stdio.h>
#include <string.h>

void xorOperation(char *dividend, char *divisor, int divisorLen) {
    for (int i = 0; i < divisorLen; i++) {
        dividend[i] = (dividend[i] == divisor[i]) ? '0' : '1';
    }
}

void crcDivision(char *data, char *divisor, char *remainder, int dataLen, int divisorLen) {
    char temp[100];

    strcpy(temp, data);

    for (int i = 0; i <= dataLen - divisorLen; i++) {
        if (temp[i] == '1') {
            xorOperation(temp + i, divisor, divisorLen);
        }
    }

    strncpy(remainder, temp + dataLen - divisorLen + 1, divisorLen - 1);
    remainder[divisorLen - 1] = '\0';
}

void crcEncode(char *dataword, char *divisor, char *codeword) {
    int dataLen = strlen(dataword);
    int divisorLen = strlen(divisor);
    char remainder[100], paddedData[100];

    strcpy(paddedData, dataword);
    for (int i = 0; i < divisorLen - 1; i++) {
        strcat(paddedData, "0");
    }

    crcDivision(paddedData, divisor, remainder, strlen(paddedData), divisorLen);

    strcpy(codeword, dataword);
    strcat(codeword, remainder);
}

int crcCheck(char *codeword, char *divisor) {
    int codewordLen = strlen(codeword);
    int divisorLen = strlen(divisor);
    char remainder[100];

    crcDivision(codeword, divisor, remainder, codewordLen, divisorLen);

    for (int i = 0; i < divisorLen - 1; i++) {
        if (remainder[i] != '0') {
            return 0;
        }
    }
    return 1; 
}

int main() {
    char dataword[100], divisor[100], codeword[100];
    int choice;

    printf("Choose an option:\n");
    printf("1. Encode dataword using CRC\n");
    printf("2. Check if the received codeword is valid\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("Enter the divisor (generator polynomial): ");
    scanf("%s", divisor);

    switch (choice) {
        case 1:
            printf("Enter the dataword: ");
            scanf("%s", dataword);

            crcEncode(dataword, divisor, codeword);
            printf("The encoded codeword is: %s\n", codeword);
            break;

        case 2:
            printf("Enter the received codeword: ");
            scanf("%s", codeword);

            if (crcCheck(codeword, divisor)) {
                printf("The received codeword is valid.\n");
            } else {
                printf("The received codeword is invalid.\n");
            }
            break;

        default:
            printf("Invalid choice! Please select 1 or 2.\n");
            break;
    }

    return 0;
}
