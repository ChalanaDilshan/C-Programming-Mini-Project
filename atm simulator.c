#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>


#define MAX_USERS 100
#define MAX_TRANSACTIONS 1000
#define FILENAME "users.dat"
#define TRANSACTION_FILE "transactions.dat"
#define ACCOUNT_NUM_LENGTH 16
#define PIN_LENGTH 4

// User data structure
typedef struct {
    char accountNumber[ACCOUNT_NUM_LENGTH + 1];
    char name[50];
    char pin[PIN_LENGTH + 1];
    float balance;
} User;

// Transaction data structure
typedef struct {
    char accountNumber[ACCOUNT_NUM_LENGTH + 1];
    time_t timestamp;
    char type[20];
    float amount;
    float newBalance;
} Transaction;

// Global variables
User users[MAX_USERS];
Transaction transactions[MAX_TRANSACTIONS];
int userCount = 0;
int transactionCount = 0;
User *currentUser = NULL;

// Function prototypes - grouped by functionality
// File I/O operations
void loadAllData();
void saveAllData();
void loadUsers();
void saveUsers();
void loadTransactions();
void saveTransactions();

// User account operations
bool createNewAccount();
bool loginUser();
void logoutUser();
void displayAccountInfo();

// Transaction operations
void recordTransaction(const char *type, float amount);
void displayTransactionHistory();

// ATM operations
void displayMainMenu();
void handleBalanceInquiry();
void handleDeposit();
void handleWithdrawal();

// Utility functions
void clearInputBuffer();
bool validateAccountNumber(const char *accountNumber);
bool validatePIN(const char *pin);
void generateAccountNumber(char *accountNumber);
void displayWelcomeMessage();
void displayGoodbyeMessage();

int main() {
    // Initialize system
    loadAllData();
    displayWelcomeMessage();

    int choice;
    bool running = true;

    // Main application loop
    while (running) {
        printf("\nATM Simulator Main Screen\n");
        printf("1. Login to Existing Account\n");
        printf("2. Create New Account\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
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
                createNewAccount();
                break;
            case 3:
                running = false;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    saveAllData();
    displayGoodbyeMessage();
    return 0;
}

// File I/O implementations
void loadAllData() {
    loadUsers();
    loadTransactions();
}

void saveAllData() {
    saveUsers();
    saveTransactions();
}

void loadUsers() {
    FILE *file = fopen(FILENAME, "rb");
    if (file != NULL) {
        fread(&userCount, sizeof(int), 1, file);
        if (userCount > MAX_USERS) userCount = MAX_USERS;
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void saveUsers() {
    FILE *file = fopen(FILENAME, "wb");
    if (file != NULL) {
        fwrite(&userCount, sizeof(int), 1, file);
        fwrite(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void loadTransactions() {
    FILE *file = fopen(TRANSACTION_FILE, "rb");
    if (file != NULL) {
        fread(&transactionCount, sizeof(int), 1, file);
        if (transactionCount > MAX_TRANSACTIONS) transactionCount = MAX_TRANSACTIONS;
        fread(transactions, sizeof(Transaction), transactionCount, file);
        fclose(file);
    }
}

void saveTransactions() {
    FILE *file = fopen(TRANSACTION_FILE, "wb");
    if (file != NULL) {
        fwrite(&transactionCount, sizeof(int), 1, file);
        fwrite(transactions, sizeof(Transaction), transactionCount, file);
        fclose(file);
    }
}

// User account implementations
bool createNewAccount() {
    if (userCount >= MAX_USERS) {
        printf("System cannot accept more users. Please try again later.\n");
        return false;
    }

    User newUser;
    generateAccountNumber(newUser.accountNumber);

    printf("\nNew Account Creation\n");
    printf("Your new account number: %s\n", newUser.accountNumber);

    // Get user name
    printf("Enter your full name: ");
    fgets(newUser.name, sizeof(newUser.name), stdin);
    newUser.name[strcspn(newUser.name, "\n")] = '\0';

    // Get and validate PIN
    char pin[PIN_LENGTH + 2]; // Extra space for newline
    bool validPin = false;
    while (!validPin) {
        printf("Create a 4-digit PIN: ");
        fgets(pin, sizeof(pin), stdin);
        pin[strcspn(pin, "\n")] = '\0';
        validPin = validatePIN(pin);
        if (!validPin) {
            printf("PIN must be exactly 4 digits.\n");
        }
    }
    strncpy(newUser.pin, pin, sizeof(newUser.pin));
    newUser.balance = 0.0f;

    users[userCount++] = newUser;
    saveUsers();

    printf("\nAccount created successfully!\n");
    displayAccountInfo(&newUser);
    return true;
}

bool loginUser() {
    char accountNumber[ACCOUNT_NUM_LENGTH + 2];
    char pin[PIN_LENGTH + 2];
    int attempts = 3;

    printf("\nUser Login\n");

    // Get account number
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

    // Find user
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

    // Verify PIN
    attempts = 3;
    while (attempts > 0) {
        printf("Enter your PIN: ");
        fgets(pin, sizeof(pin), stdin);
        pin[strcspn(pin, "\n")] = '\0';

        if (strcmp(foundUser->pin, pin) == 0) {
            currentUser = foundUser;
            recordTransaction("Login", 0.0f);
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
        recordTransaction("Logout", 0.0f);
        printf("\nGoodbye, %s. Thank you for banking with us.\n", currentUser->name);
        currentUser = NULL;
    }
}

// Transaction implementations
void recordTransaction(const char *type, float amount) {
    if (transactionCount >= MAX_TRANSACTIONS) {
        // Shift old transactions to make room (FIFO)
        for (int i = 0; i < MAX_TRANSACTIONS - 1; i++) {
            transactions[i] = transactions[i + 1];
        }
        transactionCount--;
    }

    Transaction newTrans;
    strncpy(newTrans.accountNumber, currentUser->accountNumber, ACCOUNT_NUM_LENGTH);
    newTrans.timestamp = time(NULL);
    strncpy(newTrans.type, type, sizeof(newTrans.type));
    newTrans.amount = amount;
    newTrans.newBalance = currentUser->balance;

    transactions[transactionCount++] = newTrans;
    saveTransactions();
}

void displayTransactionHistory() {
    printf("\nTransaction History for Account: %s\n", currentUser->accountNumber);
    printf("--------------------------------------------------\n");
    printf("Date/Time           Type            Amount    Balance\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < transactionCount; i++) {
        if (strcmp(transactions[i].accountNumber, currentUser->accountNumber) == 0) {
            char timeStr[20];
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&transactions[i].timestamp));

            printf("%-19s %-15s %8.2f %10.2f\n",
                   timeStr,
                   transactions[i].type,
                   transactions[i].amount,
                   transactions[i].newBalance);
        }
    }
    printf("--------------------------------------------------\n");
}

// ATM operation implementations
void displayMainMenu() {
    int choice;
    bool loggedIn = true;

    while (loggedIn && currentUser != NULL) {
        printf("\nMain Menu - %s\n", currentUser->name);
        printf("1. Check Balance\n");
        printf("2. Deposit Funds\n");
        printf("3. Withdraw Funds\n");
        printf("4. View Transaction History\n");
        printf("5. Account Information\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                handleBalanceInquiry();
                break;
            case 2:
                handleDeposit();
                break;
            case 3:
                handleWithdrawal();
                break;
            case 4:
                displayTransactionHistory();
                break;
            case 5:
                displayAccountInfo(currentUser);
                break;
            case 6:
                logoutUser();
                loggedIn = false;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void handleBalanceInquiry() {
    printf("\nCurrent Balance: $%.2f\n", currentUser->balance);
    recordTransaction("Balance Inquiry", 0.0f);
}

void handleDeposit() {
    float amount;
    printf("\nEnter amount to deposit: $");

    if (scanf("%f", &amount) != 1 || amount <= 0) {
        clearInputBuffer();
        printf("Invalid amount. Please enter a positive number.\n");
        return;
    }
    clearInputBuffer();

    currentUser->balance += amount;
    printf("Deposit successful. New balance: $%.2f\n", currentUser->balance);
    recordTransaction("Deposit", amount);
    saveUsers();
}

void handleWithdrawal() {
    float amount;
    printf("\nEnter amount to withdraw: $");

    if (scanf("%f", &amount) != 1 || amount <= 0) {
        clearInputBuffer();
        printf("Invalid amount. Please enter a positive number.\n");
        return;
    }
    clearInputBuffer();

    if (amount > currentUser->balance) {
        printf("Insufficient funds. Current balance: $%.2f\n", currentUser->balance);
        return;
    }

    currentUser->balance -= amount;
    printf("Withdrawal successful. New balance: $%.2f\n", currentUser->balance);
    recordTransaction("Withdrawal", amount);
    saveUsers();
}

// Utility function implementations
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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

void generateAccountNumber(char *accountNumber) {
    // Simple random account number generation
    for (int i = 0; i < ACCOUNT_NUM_LENGTH; i++) {
        accountNumber[i] = '0' + rand() % 10;
    }
    accountNumber[ACCOUNT_NUM_LENGTH] = '\0';
}

void displayAccountInfo(const User *user) {
    printf("\nAccount Information\n");
    printf("-------------------\n");
    printf("Account Number: %s\n", user->accountNumber);
    printf("Account Holder: %s\n", user->name);
    printf("Current Balance: $%.2f\n", user->balance);
    printf("-------------------\n");
}

void displayWelcomeMessage() {
    printf("\n***************************************\n");
    printf("*                                     *\n");
    printf("*      Welcome to the ATM Simulator   *\n");
    printf("*                                     *\n");
    printf("***************************************\n");
}

void displayGoodbyeMessage() {
    printf("\n***************************************\n");
    printf("*                                     *\n");
    printf("*    Thank you for using our ATM!     *\n");
    printf("*                                     *\n");
    printf("***************************************\n");
}
