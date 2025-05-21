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
    return false;
}
