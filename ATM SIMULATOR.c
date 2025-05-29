#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CUSTOMERS 100
#define CUSTOMER_DATA_FILE "customer_data.dat"
#define ACCOUNT_NUM_LENGTH 16
#define PIN_LENGTH 4
#define INITIAL_CUSTOMERS 5

#define GREEN "\033[0;32m"
#define RESET "\033[0m"

typedef struct {
    char accountNumber[ACCOUNT_NUM_LENGTH + 1];
    char name[50];
    char pin[PIN_LENGTH + 1];
    float balance;
} Customer;

Customer customers[MAX_CUSTOMERS];
int customerCount = 0;
Customer *currentCustomer = NULL;

void initializeSystem();
void loadCustomerData();
void saveCustomerData();
void createInitialCustomers();
bool loginCustomer();
void logoutCustomer();
void displayMainMenu();
void handleBalanceInquiry();
void handleDeposit();
void handleWithdrawal();
void displayAccountInfo(const Customer *customer);
void clearInputBuffer();
bool validateAccountNumber(const char *accountNumber);
bool validatePIN(const char *pin);
void printHeader(const char *title);

int main() {
    initializeSystem();
    loadCustomerData();

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
                if (loginCustomer()) {
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

    saveCustomerData();
    printf("\nThank you for using the ATM. Goodbye!\n");
    return 0;
}

void initializeSystem() {
    customerCount = 0;
    currentCustomer = NULL;

    FILE *file = fopen(CUSTOMER_DATA_FILE, "rb");
    if (file == NULL) {
        createInitialCustomers();
        saveCustomerData();
    } else {
        fclose(file);
    }
}

void createInitialCustomers() {
    Customer initialCustomers[INITIAL_CUSTOMERS] = {
        {"1234567890123456", "Ravindu Chamikara", "1234", 15721.00f},
        {"2345678901234567", "Chalana Dilshan", "2345", 1287.50f},
        {"3456789012345678", "Anusha Avishki", "3456", 1126.75f},
        {"4567890123456789", "Sanka Dulanjana", "5678", 45000.21f},
        {"4567890123456781", "Usam Hafrath", "4321", 41000.21f}
    };

    for (int i = 0; i < INITIAL_CUSTOMERS; i++) {
        customers[customerCount++] = initialCustomers[i];
    }
}

void loadCustomerData() {
    FILE *file = fopen(CUSTOMER_DATA_FILE, "rb");
    if (file != NULL) {
        fread(&customerCount, sizeof(int), 1, file);
        if (customerCount > MAX_CUSTOMERS) customerCount = MAX_CUSTOMERS;
        fread(customers, sizeof(Customer), customerCount, file);
        fclose(file);
    }
}

void saveCustomerData() {
    FILE *file = fopen(CUSTOMER_DATA_FILE, "wb");
    if (file != NULL) {
        fwrite(&customerCount, sizeof(int), 1, file);
        fwrite(customers, sizeof(Customer), customerCount, file);
        fclose(file);
    }
}

bool loginCustomer() {
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

    Customer *foundCustomer = NULL;
    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].accountNumber, accountNumber) == 0) {
            foundCustomer = &customers[i];
            break;
        }
    }

    if (foundCustomer == NULL) {
        printf("Account not found.\n");
        return false;
    }

    attempts = 3;
    while (attempts > 0) {
        printf("Enter your PIN: ");
        fgets(pin, sizeof(pin), stdin);
        pin[strcspn(pin, "\n")] = '\0';

        if (strcmp(foundCustomer->pin, pin) == 0) {
            currentCustomer = foundCustomer;
            printf("\nWelcome, %s!\n", currentCustomer->name);
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

void logoutCustomer() {
    if (currentCustomer != NULL) {
        printf("\nGoodbye, %s. Thank you for banking with us.\n", currentCustomer->name);
        currentCustomer = NULL;
    }
}

void displayMainMenu() {
    int choice;
    bool loggedIn = true;

    while (loggedIn && currentCustomer != NULL) {
        printHeader("MAIN MENU");
        printf("Account Holder: %s\n", currentCustomer->name);
        printf("Account Number: %s\n\n", currentCustomer->accountNumber);

        printf("1. Check Balance\n");
        printf("2. Deposit Funds\n");
        printf("3. Withdraw Funds\n");
        printf("4. Account Information\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\nInvalid input. Please enter a number (1-5).\n");
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
                displayAccountInfo(currentCustomer);
                break;
            case 5:
                logoutCustomer();
                loggedIn = false;
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 5.\n");
        }
    }
}

void handleBalanceInquiry() {
    printHeader("BALANCE INQUIRY");
    printf("Current Balance: LKR %.2f\n", currentCustomer->balance);
}

void handleDeposit() {
    float amount;
    printHeader("DEPOSIT FUNDS");
    printf("Current Balance: LKR %.2f\n", currentCustomer->balance);
    printf("Enter amount to deposit: LKR ");

    if (scanf("%f", &amount) != 1 || amount <= 0) {
        clearInputBuffer();
        printf("\nInvalid amount. Please enter a positive number.\n");
        return;
    }
    clearInputBuffer();

    currentCustomer->balance += amount;
    printf("\nDeposit successful. New balance: LKR %.2f\n", currentCustomer->balance);
    saveCustomerData();
}

void handleWithdrawal() {
    float amount;
    printHeader("WITHDRAW FUNDS");
    printf("Current Balance: LKR %.2f\n", currentCustomer->balance);
    printf("Enter amount to withdraw: LKR ");

    if (scanf("%f", &amount) != 1 || amount <= 0) {
        clearInputBuffer();
        printf("\nInvalid amount. Please enter a positive number.\n");
        return;
    }
    clearInputBuffer();

    if (amount > currentCustomer->balance) {
        printf("\nInsufficient funds. Current balance: LKR %.2f\n", currentCustomer->balance);
        return;
    }

    currentCustomer->balance -= amount;
    printf("\nWithdrawal successful. New balance: LKR %.2f\n", currentCustomer->balance);
    saveCustomerData();
}

void displayAccountInfo(const Customer *customer) {
    printHeader("ACCOUNT INFORMATION");
    printf("Account Number: %s\n", customer->accountNumber);
    printf("Account Holder: %s\n", customer->name);
    printf("Current Balance: LKR %.2f\n", customer->balance);
    printf("--------------------------------------------------\n");
}

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

void printHeader(const char *title) {
    printf("\n--------------------------------------------------\n");
    printf("%s\n", title);
    printf("--------------------------------------------------\n");
}
