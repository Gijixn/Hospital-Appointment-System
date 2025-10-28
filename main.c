#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct
    {
        int patID;
        char date[20];
        char patName[50];
        char patDOB[15];
        char patContactNumber[15];
        char patSex[2];
        char patFullAddress[200];
        char patHeight[10];
        char patWeight[10];
        char patDiagnosis[200];
        char patFacility[50];
        char patModeofPayment[50];
        int patPaymentAmount;

    } User;
    

int idCheck(const char *filename, int id)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return 0;

    User temp;
    while (fread(&temp, sizeof(User), 1, file) == 1)
    {
        if (temp.patID == id)
        {
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0; 
}

int unique_IDGen(void)
{
    int id;
    srand(time(NULL));

    while(1)
    {
        id = rand() % 9000 + 1000; 

        if (!idCheck("inpatientRecords.bin", id) && !idCheck("outpatientRecords.bin", id))
        {
            return id;
        }
    }
    
}

const char *currentDate(void)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    static char dateStr[20]; 
    strftime(dateStr, sizeof(dateStr), "%H:%M-%m/%d/%Y", t);
    return dateStr;
}

bool patWriteData(User data, int choice)
{
    FILE *patRecords;
    if (choice == 2)
    {
        patRecords = fopen("outpatientRecords.bin", "ab");

        if (patRecords == NULL) return false;
    }

       else if (choice == 1)
    {
        patRecords = fopen("inpatientRecords.bin", "ab");
        
        if (patRecords == NULL) return false;

    } else {
        printf("Invalid Choice\n");
        return false;
    }

    if (fwrite(&data, sizeof(User), 1, patRecords) != 1) return false;

    if (fclose(patRecords)  == EOF) return false;

    return true;

}

User *patReadData(int choice)
{
    FILE *patRecords;
    if (choice == 1)
    {
        patRecords = fopen("inpatientRecords.bin", "rb");

        if (patRecords == NULL) return NULL;

    }

       else if (choice == 2)
    {
        patRecords = fopen("outpatientRecords.bin", "rb");

        if (patRecords == NULL) return NULL;

    } else return NULL;

    fseek(patRecords, 0, SEEK_END);
    int fileSize = ftell(patRecords);
    fseek(patRecords, 0, SEEK_SET);

    int recordTotal = fileSize / sizeof(User);

    User *bufferRead = malloc(sizeof(User) * recordTotal);

    if (fread(bufferRead, sizeof(User), recordTotal, patRecords) != 1) return NULL;

    if (fclose(patRecords)  == EOF) return NULL;
    
    return(bufferRead);

}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removeNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

User inputUserData(void)
{
    User u;
    memset(&u, 0, sizeof(User));
    clearInputBuffer();

    char temp[256];
    int step = 0;
    const int totalSteps = 10;

    while (step < totalSteps)
    {
        printf("\n(Type 'b' to go back to the previous field)\n");

        switch (step)
        {
            case 0: printf("1. Enter patient name: "); break;
            case 1: printf("2. Enter date of birth (YYYY-MM-DD): "); break;
            case 2: printf("3. Enter contact number: "); break;
            case 3: printf("4. Enter sex (M/F): "); break;
            case 4: printf("5. Enter full address: "); break;
            case 5: printf("6. Enter height (e.g., 170cm): "); break;
            case 6: printf("7. Enter weight (e.g., 65kg): "); break;
            case 7: printf("8. Enter diagnosis: "); break;
            case 8: printf("9. Enter facility: "); break;
            case 9: printf("10. Enter mode of payment: "); break;
        }

        fgets(temp, sizeof(temp), stdin);
        removeNewline(temp);

        if (strcmp(temp, "b") == 0 || strcmp(temp, "B") == 0) {
            if (step > 0) step--;
            else printf("Already at first field.\n");
            continue;
        }

        switch (step)
        {
            case 0: strncpy(u.patName, temp, sizeof(u.patName)); break;
            case 1: strncpy(u.patDOB, temp, sizeof(u.patDOB)); break;
            case 2: strncpy(u.patContactNumber, temp, sizeof(u.patContactNumber)); break;
            case 3: strncpy(u.patSex, temp, sizeof(u.patSex)); break;
            case 4: strncpy(u.patFullAddress, temp, sizeof(u.patFullAddress)); break;
            case 5: strncpy(u.patHeight, temp, sizeof(u.patHeight)); break;
            case 6: strncpy(u.patWeight, temp, sizeof(u.patWeight)); break;
            case 7: strncpy(u.patDiagnosis, temp, sizeof(u.patDiagnosis)); break;
            case 8: strncpy(u.patFacility, temp, sizeof(u.patFacility)); break;
            case 9: strncpy(u.patModeofPayment, temp, sizeof(u.patModeofPayment)); break;
        }

        step++;
    }

    printf("11. Enter payment amount: ");
    scanf("%d", &u.patPaymentAmount);
    clearInputBuffer();

    
    while (1)
    {
        printf("\n--- Review Information ---\n");
        printf(" 1. Name: %s\n", u.patName);
        printf(" 2. DOB: %s\n", u.patDOB);
        printf(" 3. Contact: %s\n", u.patContactNumber);
        printf(" 4. Sex: %s\n", u.patSex);
        printf(" 5. Full Address: %s\n", u.patFullAddress);
        printf(" 6. Height: %s\n", u.patHeight);
        printf(" 7. Weight: %s\n", u.patWeight);
        printf(" 8. Diagnosis: %s\n", u.patDiagnosis);
        printf(" 9. Facility: %s\n", u.patFacility);
        printf(" 10. Payment Mode: %s\n", u.patModeofPayment);
        printf(" 11. Payment Amount: %d\n", u.patPaymentAmount);

        char confirm[10];
        printf("\nAre all details correct? (Y/N): ");
        fgets(confirm, sizeof(confirm), stdin);
        removeNewline(confirm);

        if (strcasecmp(confirm, "Y") == 0)
            break;

        else if (strcasecmp(confirm, "N") == 0)
        {
            printf("Which field number do you want to edit? (1–11): ");
            int edit;
            scanf("%d", &edit);
            clearInputBuffer();

            if (edit >= 1 && edit <= 10) 
            {
                step = edit - 1;
                while (step < totalSteps)
                {
                    printf("\n(Type 'b' to go back)\n");
                    switch (step)
                    {
                        case 0: printf("1. Enter patient name: "); break;
                        case 1: printf("2. Enter date of birth (YYYY-MM-DD): "); break;
                        case 2: printf("3. Enter contact number: "); break;
                        case 3: printf("4. Enter sex (M/F): "); break;
                        case 4: printf("5. Enter full address: "); break;
                        case 5: printf("6. Enter height (e.g., 170cm): "); break;
                        case 6: printf("7. Enter weight (e.g., 65kg): "); break;
                        case 7: printf("8. Enter diagnosis: "); break;
                        case 8: printf("9. Enter facility: "); break;
                        case 9: printf("10. Enter mode of payment: "); break;
                    }

                    fgets(temp, sizeof(temp), stdin);
                    removeNewline(temp);

                    if (strcmp(temp, "b") == 0 || strcmp(temp, "B") == 0) {
                        if (step > 0) step--;
                        else printf("Already at first field.\n");
                        continue;
                    }

                    switch (step)
                    {
                        case 0: strncpy(u.patName, temp, sizeof(u.patName)); break;
                        case 1: strncpy(u.patDOB, temp, sizeof(u.patDOB)); break;
                        case 2: strncpy(u.patContactNumber, temp, sizeof(u.patContactNumber)); break;
                        case 3: strncpy(u.patSex, temp, sizeof(u.patSex)); break;
                        case 4: strncpy(u.patFullAddress, temp, sizeof(u.patFullAddress)); break;
                        case 5: strncpy(u.patHeight, temp, sizeof(u.patHeight)); break;
                        case 6: strncpy(u.patWeight, temp, sizeof(u.patWeight)); break;
                        case 7: strncpy(u.patDiagnosis, temp, sizeof(u.patDiagnosis)); break;
                        case 8: strncpy(u.patFacility, temp, sizeof(u.patFacility)); break;
                        case 9: strncpy(u.patModeofPayment, temp, sizeof(u.patModeofPayment)); break;
                    }

                    step++;
                }
            }
            else if (edit == 10) {
                printf("Enter new payment amount: ");
                scanf("%d", &u.patPaymentAmount);
                clearInputBuffer();
            }
            else {
                printf("Invalid field number.\n");
            }
        }
    }
    u.patID = unique_IDGen();
    strncpy(u.date, currentDate(), sizeof(u.date));
    u.date[sizeof(u.date) - 1] = '\0';

    return u;
}

void printReceipt(User u)
{
    printf("\n===========================================\n");
    printf("                Makulay General Hospital\n");
    printf("                  Levitown, Lipa City\n");
    printf("          Tel: (02) 8123-4567 | TIN: 004-567-890\n");
    printf("===========================================\n");

    printf("Receipt Date/Time: %s\n", u.date);
    printf("Patient Name:      %s\n", u.patName);
    printf("Patient ID:        %d\n", u.patID);
    printf("Facility:          %s\n", u.patFacility);
    printf("-------------------------------------------\n");
    printf("Mode of Payment:   %s\n", u.patModeofPayment);
    printf("Amount Paid:       PHP %d.00\n", u.patPaymentAmount);
    printf("-------------------------------------------\n");
    printf("    Thank you for trusting Makulay GH!\n");
    printf("   Wishing you a speedy recovery and health.\n");
    printf("===========================================\n\n");
}

int menu()
    {
        int choice;
        printf("\n1. Inpatient Registration\n");
        printf("2. Outpatient Registration\n");
        printf("3. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        return choice;
    }

int main()
{
    User buffer;


    while(1)
    {
        int choice = menu();
        if (choice == 1 || choice == 2)
        {
            buffer = inputUserData();
            if (patWriteData(buffer, choice))
            {
                printf("\nPatient registered successfully with ID: %d\n", buffer.patID);
                printReceipt(buffer);
            }
            else
            {
                printf("\nError registering patient.\n");
            }
        }

        if (choice == 3)
        {
            break;
        }

        else if (choice < 1 || choice > 3)
        {
            printf("\nInvalid choice. Please try again.\n");
        }

    }
    return 0;
}
