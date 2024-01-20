#include <stdio.h>

int main() {
    char phone[11];
    int index;
    int result;

   
    scanf("%10s", phone);

    
    while ((result = scanf("%d", &index)) != EOF) {
        if (result == 1) {
            if (index == -1) {
                printf("%s\n", phone);
            } else if (index >= 0 && index <= 9) {
                printf("%c\n", phone[index]);
            } else {
                printf("ERROR\n");
                return 1;  
            }
        } else {
           
            while (getchar() != '\n');
        }
    }

    return 0; 