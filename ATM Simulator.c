#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_USERS 100
#define USER_DATA_FILE "user_data.dat"
#define ACCOUNT_NUM_LENGTH 16
#define PIN_LENGTH 4
#define INITIAL_USERS 5

#define GREEN "\033[0;32m"
#define RESET "\033[0m"

typedef struct {
    char accountNumber[ACCOUNT_NUM_LENGTH + 1];
    char name[50];
    char pin[PIN_LENGTH + 1];
    float balance;
} User;

User users[MAX_USERS];
int userCount = 0;
User *currentUser = NULL;

void initializeSystem();
void loadUserData();
void saveUserData();
void createInitialUsers();
bool loginUser();
void logoutUser();
void displayMainMenu();
void handleBalanceInquiry();
void handleDeposit();
void handleWithdrawal();
void displayAccountInfo(const User *user);
void clearInputBuffer();
bool validateAccountNumber(const char *accountNumber);
bool validatePIN(const char *pin);
void printHeader(const char *title);

int main() {
    initializeSystem();
    loadUserData();

    int choice;
    bool running = true;

    while (running) {
        printHeader(GREEN "WELCOME TO BANK OF CEYLON ATM SYSTEM" RESET);
        printf("1. Login to Your Account\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\nInvalid input. Please enter a number (1-2).\n");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                if (loginUser()) {
                    displayMainMenu();
                }
                break;
            case 2:
                running = false;
                break;
            default:
                printf("\nInvalid choice. Please enter 1 or 2.\n");
        }
    }

    saveUserData();
    printf("\nThank you for using the ATM. Goodbye!\n");
    return 0;
}
void initializeSystem() {
    userCount = 0;
    currentUser = NULL;

    FILE *file = fopen(USER_DATA_FILE, "rb");
    if (file == NULL) {
        createInitialUsers();
        saveUserData();
    } else {
        fclose(file);
    }
}

void createInitialUsers() {
    User initialUsers[INITIAL_USERS] = {
        {"1234567890123456", "Ravindu Chamikara", "1234", 15721.00f},
        {"2345678901234567", "Chalana Dilshan", "2345", 1287.50f},
        {"3456789012345678", "Anusha Avishki", "3456", 1126.75f},
        {"4567890123456789", "Sanka Dulanjana", "5678", 45000.21f},
        {"4567890123456781", "Usam Hafrath", "4321", 41000.21f}
    };

    for (int i = 0; i < INITIAL_USERS; i++) {
        users[userCount++] = initialUsers[i];
    }
}

void loadUserData() {
    FILE *file = fopen(USER_DATA_FILE, "rb");
    if (file != NULL) {
        fread(&userCount, sizeof(int), 1, file);
        if (userCount > MAX_USERS) userCount = MAX_USERS;
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
}
