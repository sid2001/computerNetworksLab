#include <stdio.h>
#include <string.h>

char* even_parity(char* data) {
    static char result[100];
    int count_1 = 0;

    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1') {
            count_1++;
        }
    }

    strcpy(result, data);
    if (count_1 % 2 == 0) {
        strcat(result, "0");
    } else {
        strcat(result, "1");
    }

    return result;
}

char* odd_parity(char* data) {
    static char result[100];
    int count_1 = 0;
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1') {
            count_1++;
        }
    }

    strcpy(result, data);
    if (count_1 % 2 == 0) {
        strcat(result, "1");
    } else {
        strcat(result, "0");
    }

    return result;
}

int even_error_detection(char* data) {
    int count_1 = 0;

    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1') {
            count_1++;
        }
    }

    return count_1 % 2 != 0;
}

int odd_error_detection(char* data) {
    int count_1 = 0;

    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1') {
            count_1++;
        }
    }

    return count_1 % 2 == 0;
}

int main() {
    char data[] = "10011";
    printf("The transferred data with even parity check is: %s\n", even_parity(data));
    printf("The transferred data with odd parity check is: %s\n", odd_parity(data));

    char received_data[] = "110111";
    printf("The received data is: %s\n", received_data);

    if (even_error_detection(received_data)) {
        printf("Error detected with Even Parity checker\n");
    }

    if (odd_error_detection(received_data)) {
        printf("Error detected with Odd Parity checker\n");
    }

    return 0;
}
