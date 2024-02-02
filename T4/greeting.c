#include <stdio.h>
#include <string.h>


/*
    This program has two arguments: the first is a greeting message, and the
    second is a name.

    The message is an impersonal greeting, such as "Hi" or "Good morning".
    name is set to refer to a string holding a friend's name, such as
    "Emmanuel" or "Xiao".

    First copy the first argument to the array greeting. (Make sure it is
    properly null-terminated.)

    Write code to personalize the greeting string by appending a space and
    then the string pointed to by name.
    So, in the first example, greeting should be set to "Hi Emmanuel", and
    in the second it should be "Good morning Xiao".

    If there is not enough space in greeting, the resulting greeting should be
    truncated, but still needs to hold a proper string with a null terminator.
    For example, "Good morning" and "Emmanuel" should result in greeting
    having the value "Good morning Emmanu" and "Top of the morning to you" and
    "Patrick" should result in greeting having the value "Top of the morning ".

    Do not make changes to the code we have provided other than to add your
    code where indicated.
*/

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: greeting message name\n");
        return 1;
    }
    char greeting[20];
    char *name = argv[2];

    
    strncpy(greeting, argv[1], sizeof(greeting) - 1);
    greeting[sizeof(greeting) - 1] = '\0';  // Ensure null termination.

    // Calculate the available space in the greeting array.
    // -1 to ensure there's space for the null terminator after concatenation.
    int size_used = strlen(greeting);
    int size_available = sizeof(greeting) - size_used - 1;

    // Append a space before the name if there's enough space.
    if (size_available > 0) {
        strncat(greeting, " ", 1);
        size_available -= 1;  
        size_used += 1;  
    }
    // Append the name, ensuring the result is null-terminated.
    if (size_available > 0) {
        strncat(greeting, name, size_available);
    }

    printf("%s\n", greeting);
    return 0;
}


