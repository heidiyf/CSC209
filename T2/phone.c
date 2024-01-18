#include <stdio.h>

int main() {
    char phone[11];
    int index;
    scanf("%10s %d", phone, &index); 

    if (index == -1) {
        printf("%s\n", phone);
        return 0;
    } else if (index >= 0 && index <= 9) { 
        printf("%c\n", phone[index]); 
        return 0;
    } else {
        printf("ERROR\n"); 
        return 1;
    }

    return 0; 
}