#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "user_management.h"
#include "common.h"

// Global variables defined in common.h/user_management.h
User users[MAX_USERS];
int userCount = 0;
User *currentUser = NULL;

void loadUserData() {
    FILE *file = fopen(USER_DATA_FILE, "rb");
    if (file != NULL) {
        fread(&userCount, sizeof(int), 1, file);
        if (userCount > MAX_USERS) userCount = MAX_USERS;
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void saveUserData() {
    FILE *file = fopen(USER_DATA_FILE, "wb");
    if (file != NULL) {
        fwrite(&userCount, sizeof(int), 1, file);
        fwrite(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

bool loginUser() {
    char accountNumber[ACCOUNT_NUM_LENGTH + 2];
    char pin[PIN_LENGTH + 2];
    int attempts = 3;

    printHeader("ACCOUNT LOGIN");

    bool validAccount = false;
    while (!validAccount && attempts > 0) {
        printf("Enter your %d-digit account number: ", ACCOUNT_NUM_LENGTH);
        fgets(accountNumber, sizeof(accountNumber), stdin);
        accountNumber[strcspn(accountNumber, "\n")] = '\0';
        validAccount = validateAccountNumber(accountNumber);

        if (!validAccount) {
            attempts--;
            printf("Invalid account number format. Attempts left: %d\n", attempts);
        }
    }

    if (!validAccount) {
        printf("Login failed. Returning to main menu.\n");
        return false;
    }

    User *foundUser = NULL;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].accountNumber, accountNumber) == 0) {
            foundUser = &users[i];
            break;
        }
    }

    if (foundUser == NULL) {
        printf("Account not found.\n");
        return false;
    }

    attempts = 3;
    while (attempts > 0) {
        printf("Enter your PIN: ");
        fgets(pin, sizeof(pin), stdin);
        pin[strcspn(pin, "\n")] = '\0';

        if (strcmp(foundUser->pin, pin) == 0) {
            currentUser = foundUser;
            printf("\nWelcome, %s!\n", currentUser->name);
            return true;
        }

        attempts--;
        if (attempts > 0) {
            printf("Incorrect PIN. %d attempts remaining.\n", attempts);
        }
    }

    printf("Too many incorrect attempts. Access denied.\n");
    return false;
}

void logoutUser() {
    if (currentUser != NULL) {
        printf("\nGoodbye, %s. Thank you for banking with us.\n", currentUser->name);
        currentUser = NULL;
    }
}

bool validateAccountNumber(const char *accountNumber) {
    if (strlen(accountNumber) != ACCOUNT_NUM_LENGTH) return false;

    for (int i = 0; i < ACCOUNT_NUM_LENGTH; i++) {
        if (!isdigit(accountNumber[i])) return false;
    }

    return true;
}

bool validatePIN(const char *pin) {
    if (strlen(pin) != PIN_LENGTH) return false;

    for (int i = 0; i < PIN_LENGTH; i++) {
        if (!isdigit(pin[i])) return false;
    }

    return true;
}

void displayAccountInfo(const User *user) {
    printHeader("ACCOUNT INFORMATION");
    printf("Account Number: %s\n", user->accountNumber);
    printf("Account Holder: %s\n", user->name);
    printf("Current Balance: LKR %.2f\n", user->balance);
    printf("--------------------------------------------------\n");
}
