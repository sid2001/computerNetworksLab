#include <stdio.h>
#include <math.h>
#include <string.h>

int calculateParityBit(int position, int encoded[], int length) {
    int parity = 0;
    for (int i = position - 1; i < length; i += 2 * position) {
        for (int j = i; j < i + position && j < length; j++) {
            parity ^= encoded[j];
        }
    }
    return parity;
}

void generateHammingCode(int data[], int dataSize, int encoded[], int totalBits) {
    int j = 0, k = 0;

    for (int i = 0; i < totalBits; i++) {
        encoded[i] = 0;
    }

    for (int i = 0; i < totalBits; i++) {
        if ((i & (i + 1)) == 0) { 
            continue;
        }
        encoded[i] = data[k++];
    }

    for (int i = 0; i < log2(totalBits + 1); i++) {
        int position = pow(2, i);
        encoded[position - 1] = calculateParityBit(position, encoded, totalBits);
    }
}

void detectAndCorrectError(int encoded[], int totalBits) {
    int errorPosition = 0;

    for (int i = 0; i < log2(totalBits + 1); i++) {
        int position = pow(2, i);
        int parity = calculateParityBit(position, encoded, totalBits);
        if (parity != 0) {
            errorPosition += position; 
        }
    }

    if (errorPosition == 0) {
        printf("No error detected. The Hamming code is correct.\n");
    } else {
        printf("Error detected at position: %d\n", errorPosition);
        encoded[errorPosition - 1] ^= 1;
        printf("Error corrected. The corrected Hamming code is: ");
        for (int i = 0; i < totalBits; i++) {
            printf("%d", encoded[i]);
        }
        printf("\n");
    }
}

void extractDataBits(int encoded[], int data[], int totalBits, int dataSize) {
    int dataIndex = 0;
    for (int i = 0; i < totalBits; i++) {
        if ((i & (i + 1)) != 0) { 
            data[dataIndex++] = encoded[i];
        }
    }
}

void printHammingCode(int encoded[], int totalBits) {
    printf("The Hamming code is: ");
    for (int i = 0; i < totalBits; i++) {
        printf("%d", encoded[i]);
    }
    printf("\n");
}

void printDataBits(int data[], int dataSize) {
    printf("The data bits are: ");
    for (int i = 0; i < dataSize; i++) {
        printf("%d", data[i]);
    }
    printf("\n");
}

int main() {
    char inputString[100];
    int data[100], encoded[100];
    int dataSize, totalBits = 0, parityBits = 0, choice;

    printf("Choose an option:\n");
    printf("1. Generate Hamming code\n");
    printf("2. Check if the given Hamming code is correct\n");
    printf("3. Extract data bits from Hamming code\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); 

    switch (choice) {
        case 1: {
            printf("Enter the data string: ");
            fgets(inputString, sizeof(inputString), stdin);
            inputString[strcspn(inputString, "\n")] = '\0';

            dataSize = strlen(inputString);

            for (int i = 0; i < dataSize; i++) {
                data[i] = inputString[i] - '0';
            }

            while (pow(2, parityBits) < dataSize + parityBits + 1) {
                parityBits++;
            }
            totalBits = dataSize + parityBits;

            generateHammingCode(data, dataSize, encoded, totalBits);
            printHammingCode(encoded, totalBits);
            break;
        }

        case 2: {
            printf("Enter the number of bits in the Hamming code: ");
            scanf("%d", &totalBits);
            getchar(); 

            printf("Enter the Hamming code as a string: ");
            fgets(inputString, sizeof(inputString), stdin);
            inputString[strcspn(inputString, "\n")] = '\0';

            for (int i = 0; i < totalBits; i++) {
                encoded[i] = inputString[i] - '0';
            }

            detectAndCorrectError(encoded, totalBits);
            break;
        }

        case 3: {
            printf("Enter the number of bits in the Hamming code: ");
            scanf("%d", &totalBits);
            getchar(); 

            printf("Enter the Hamming code as a string: ");
            fgets(inputString, sizeof(inputString), stdin);
            inputString[strcspn(inputString, "\n")] = '\0';

            for (int i = 0; i < totalBits; i++) {
                encoded[i] = inputString[i] - '0';
            }

            int parityBits = 0;
            while (pow(2, parityBits) < totalBits + 1) {
                parityBits++;
            }
            
            int extractedDataSize = totalBits - parityBits;
            int data[extractedDataSize];
            extractDataBits(encoded, data, totalBits, extractedDataSize);
            printDataBits(data, extractedDataSize);
            break;
        }

        default:
            printf("Invalid choice! Please select 1, 2, or 3.\n");
            break;
    }

    return 0;
}
