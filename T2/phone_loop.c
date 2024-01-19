#include <stdio.h>

int main() {
    char phone[11];
    int index;
    int result;

    // Read the 10-character string
    scanf("%10s", phone);

    // Continuously read integers until the input stream is closed
    while ((result = scanf("%d", &index)) != EOF) {
        if (result == 1) {
            if (index == -1) {
                printf("%s\n", phone);
            } else if (index >= 0 && index <= 9) {
                printf("%c\n", phone[index]);
            } else {
                printf("ERROR\n");
                return 1;  // Return 1 immediately if the input is out of the specified range
            }
        } else {
            // Clear the input buffer if scanf fails to match an integer
            while (getchar() != '\n');
        }
    }

    return 0;  // Return 0 on normal program termination
}