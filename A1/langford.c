#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void print_array(int numbers[], int size){
    printf("[");
    for (int i = 0; i < size; ++i) {
        printf("%d", numbers[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

bool is_langford_pairing(int size, const int arr[]) {
    
    if (size % 2 != 0) {
        return false; // Langford pairings must have an even number of elements.
    }

    int *arr2 = malloc(sizeof(int) * size);
    if (!arr2) {
        // Handle memory allocation failure
        return false;
    }
    memcpy(arr2, arr, sizeof(int) * size);

    for (int i = 0; i < size; i++){
        if (arr2[i] == 0) { // set to zero if already checked, skip
            continue;
        }
        int num = arr2[i];
        int targetIndex = i + num + 1;
        if (num != arr2[targetIndex] || targetIndex >= size){
            free(arr2);
            return false;
        }
        arr2[i] = 0;
        arr2[targetIndex] = 0;
    }
    // at this point, finished checking all the pairs. if all zero -> true, else false

    for (int i = 0; i < size; i++) {
        if (arr2[i] != 0) {
            free(arr2);
            return false;
        }
    }
    free(arr2);
    return true;
}

int ceiling(double n){
    int int_part = (int)n; // Get the integer part of n
    if (n > 0 && n - (double)int_part > 0) {
        return int_part + 1; // If n is positive and has a fractional part, add 1 to the integer part
    }
    return int_part; // If n is an integer or negative, return the integer part
}

void reverse_array(const int *original, int *reversed, int size) {
    for (int i = 0; i < size; ++i) {
        reversed[i] = original[size - 1 - i];
    }
}

void create_langford_pairing(int n) { // from the youtube
    int x = ceiling(n/4);
    int a = 2 * x - 1;
    int b = 4 * x - 2;
    int c = 4 * x - 1;
    int d = 4 * x;

    int p[n], q[n], r[n], s[n];
    int p_reverse[n], q_reverse[n], r_reverse[n], s_reverse[n];
    int p_index = 0, q_index = 0, r_index = 0, s_index = 0;

    // Fill the p sequence with odd numbers up to a-1
    for (int i = 1; i <= a - 1; i += 2) {
        p[p_index++] = i;
    }
    reverse_array(p, p_reverse, p_index);

    // Fill the q sequence with even numbers up to a-1
    for (int i = 2; i <= a - 1; i += 2) {
        q[q_index++] = i;
    }
    reverse_array(q, q_reverse, q_index);

    // Determine the starting point for the r and s sequences based on the parity of a
    int start_r = a % 2 == 0 ? a + 1 : a + 2; // Start with the next odd number after a
    int start_s = a % 2 == 0 ? a + 2 : a + 1; // Start with the next even number after a

    // Fill the r sequence with odd numbers from start_r up to b-1
    for (int i = start_r; i <= b - 1; i += 2) {
        r[r_index++] = i;
    }
    reverse_array(r, r_reverse, r_index);

    // Fill the s sequence with even numbers from start_s up to b-1
    for (int i = start_s; i <= b - 1; i += 2) {
        s[s_index++] = i;
    }
    reverse_array(s, s_reverse, s_index);

    // Now create the final Langford sequence using the sequences and their reversals
    int final_array[2 * n], final_index = 0;

    // reverse s
    for (int i = 0; i < s_index; i++) {
        final_array[final_index++] = s_reverse[i];
    }

    // Add reversed p to the final sequence
    for (int i = 0; i < p_index; i++) {
        final_array[final_index++] = p_reverse[i];
    }

    // Add b, p sequence, c, s sequence, d
    final_array[final_index++] = b;
    for (int i = 0; i < p_index; i++) {
        final_array[final_index++] = p[i];
    }
    final_array[final_index++] = c;
    for (int i = 0; i < s_index; i++) {
        final_array[final_index++] = s[i];
    }
    final_array[final_index++] = d;

    // Add reversed r to the final sequence
    for (int i = 0; i < r_index; i++) {
        final_array[final_index++] = r_reverse[i];
    }

    // Add reversed q to the final sequence
    for (int i = 0; i < q_index; i++) {
        final_array[final_index++] = q_reverse[i];
    }

    // Add b, a, q sequence, c, r sequence, a, d
    final_array[final_index++] = b;
    final_array[final_index++] = a;
    for (int i = 0; i < q_index; i++) {
        final_array[final_index++] = q[i];
    }
    final_array[final_index++] = c;
    for (int i = 0; i < r_index; i++) {
        final_array[final_index++] = r[i];
    }
    final_array[final_index++] = a;
    final_array[final_index++] = d;

    // Print the final Langford sequence
    print_array(final_array, final_index);

}

int main(int argc, char *argv[]) {

    if ((argc == 1) || (argc > 1 && strcmp(argv[1], "-h") == 0)) { // argc == 1: no argument; strcmp is for comparing 2 strings.
        fprintf(stderr, "usage: %s [-h] -c n | num...\n", argv[0]);
        return 0; // Exit with code 0 in both cases. first word of the command varies, as it can be ./langford or ././langford.
    }  else if (argc == 2 && strcmp(argv[1], "-c") == 0) { // check -c is specified AND no arguments(argc only 2)
        fprintf(stderr, "%s: -c option requires an argument.\n", argv[0]);
        return 1;
    } else if (argc > 3 && strcmp(argv[1], "-c") == 0) { // two many
        fprintf(stderr, "%s: -c option received too many arguments.\n", argv[0]);
        return 1;
    } else if (argc == 3 && strcmp(argv[1], "-c") == 0) {
        char *endptr;
        int value = strtol(argv[2], &endptr, 10);

        if (endptr && endptr[0] != '\0') {
            fprintf(stderr, "error: %s is not an integer.\n", argv[2]);
            exit(1);
        } else { // n is properly specified
            printf("Creating a langford pairing with n=%d\n", value);

            if ((value % 4 == 1) || (value % 4 == 2)) {
                printf("No results found.\n");
                return 0;
            }  else { 
                create_langford_pairing(value);
            }

        } 
    } else if (argc > 1 && (strcmp(argv[1], "-h") != 0) && (strcmp(argv[1], "-c") != 1)){ // Check mode: no -h nor -c 
        int numbers[argc - 1]; // prepare integer array

        for (int i = 1; i < argc; i++){ // check arguments are integers
            char *endptr;
            int value = strtol(argv[i], &endptr, 10);

            if (endptr && endptr[0] != '\0') {
                fprintf(stderr, "error: %s is not an integer.\n", argv[i]);
                exit(1);
            }
            numbers[i - 1] = (int) value; // Store the converted integer
        }
        printf("Your sequence:");
        print_array(numbers, argc - 1); // Print the sequence
        ;
     
        if (is_langford_pairing(argc - 1, numbers)){
            printf("It is a langford pairing!\n");
        } else{
            printf("It is NOT a langford pairing.\n");
        }
    } 

    return 0;
}
