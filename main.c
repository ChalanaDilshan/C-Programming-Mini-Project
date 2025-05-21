#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "user_management.h"
#include "menu_system.h"
#include "common.h"

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
