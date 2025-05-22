#include <stdio.h>
#include <stdlib.h>
#include "transaction.h"
#include "user_management.h"
#include "common.h"

void handleBalanceInquiry() {
    printHeader("BALANCE INQUIRY");
    printf("Current Balance: LKR %.2f\n", currentUser->balance);
}

void handleDeposit() {
    float amount;
    printHeader("DEPOSIT FUNDS");
    printf("Current Balance: LKR %.2f\n", currentUser->balance);
    printf("Enter amount to deposit: LKR ");

    if (scanf("%f", &amount) != 1 || amount <= 0) {
        clearInputBuffer();
        printf("\nInvalid amount. Please enter a positive number.\n");
        return;
    }
    clearInputBuffer();

    currentUser->balance += amount;
    printf("\nDeposit successful. New balance: LKR %.2f\n", currentUser->balance);
    saveUserData();
}

void handleWithdrawal() {
    float amount;
    printHeader("WITHDRAW FUNDS");
    printf("Current Balance: LKR %.2f\n", currentUser->balance);
    printf("Enter amount to withdraw: LKR ");

    if (scanf("%f", &amount) != 1 || amount <= 0) {
        clearInputBuffer();
        printf("\nInvalid amount. Please enter a positive number.\n");
        return;
    }
    clearInputBuffer();

    if (amount > currentUser->balance) {
        printf("\nInsufficient funds. Current balance: LKR %.2f\n", currentUser->balance);
        return;
    }

    currentUser->balance -= amount;
    printf("\nWithdrawal successful. New balance: LKR %.2f\n", currentUser->balance);
    saveUserData();
}

