#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SYMPTOMS 10

typedef struct
{
    char name[50];
    char doctors[3][50];
} department;

department  departments[] = {
    {"Pediatrics", {"Dr. Garcia", "Dr. Martinez", "Dr. Hernandez"}},
    {"Surgery", {"Dr. Lopez", "Dr. Gonzalez", "Dr. Wilson"}},
    {"Internal Medicine", {"Dr. Moore", "Dr. Jackson", "Dr. White"}},
    {"Gynecology", {"Dr. Harris", "Dr. Clark", "Dr. Lewis"}},
    {"Radiology", {"Dr. Hill", "Dr. Flores", "Dr. Green"}},
    {"Laboratory", {"Dr. Adams", "Dr. Nelson", "Dr. Baker"}}
};

typedef struct
{
    char labTests[50];
    int prices;
} labTests;

labTests tests[] = {
    {"Complete Blood Count (CBC)", 500},
    {"Lipid Profile", 700},
    {"Urinalysis", 300},
    {"Fecalysis", 350},
    {"Blood Glucose Test", 400},
    {"Electrolyte Panel", 600},
    {"Coagulation Profile", 750},
    {"Liver Function Test", 800},
    {"Kidney Function Test", 850},
    {"X-Ray", 1500},
    {"Ultrasound", 2000},
    {"Electrocardiogram (ECG)", 1200},
    {"Health Consultation", 500},
};

typedef struct
{
    char roomType[50];
    int roomPrice;
} room;

room hospitalRooms[] = {
    {"General Ward", 2000},
    {"Semi-Private Room", 4000},
    {"Private Room", 6000},
    {"ICU", 10000},
    {"Maternity Ward", 5000},
    {"None", 0}
};

typedef struct
{  
    char billBreakdown[200];
    char paymentMode[50];
    int paymentAmount;
} billing;

const char *symptomsList[] = {
    "Fever",
    "Cough",
    "Headache",
    "Fatigue",
    "Nausea",
    "Dizziness",
    "Shortness of Breath",
    "Chest Pain",
    "Abdominal Pain",
    "Sore Throat"
};

const char *paymentMethods[] =  {
    "Cash",
    "Card",
    "Online",
    "Cheque"
};

const char *hmoList[] = {
    "None",
    "Maxicare",
    "Intellicare",
    "Medicard",
    "PhilCare"
};

typedef struct
    {
        int patID;
        char date[20];
        char patName[50];
        char patAge[5];
        char patDOB[15];
        char patSex[10];
        char patContactNumber[15];
        char patFullAddress[200];
        char patSymptoms[200];
        char patDepartment[50];
        char patHMO[50];
        char patAssignedDoctor[50];
        billing patBilling;
        room hospitalRooms;
        char isSeniorOrPwd[5];
        char patDaysofStay[5];
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
        id = rand() % 9000000 + 1000000; 

        if (!idCheck("inpatientRecords.bin", id) && !idCheck("outpatientRecords.bin", id))
        {
            return id;
        }
    }
    
}

void getRandomDepartment(char *departmentName, char *doctorName)
{
    int numDepartments = sizeof(departments) / sizeof(departments[0]);
    int deptIndex = rand() % numDepartments;
    int docIndex = rand() % 3;

    strcpy(departmentName, departments[deptIndex].name);
    strcpy(doctorName, departments[deptIndex].doctors[docIndex]);
}

void getRandomLabTestsString(char *result, int *totalPrice) {
    int numTests = sizeof(tests) / sizeof(tests[0]);
    int used[20] = {0};
    *totalPrice = 0;
    result[0] = '\0';

    strcat(result, "Health Consultation");
    *totalPrice += tests[numTests - 1].prices;
    used[numTests - 1] = 1; 

    int additional = rand() % 3;

    for (int i = 0; i < additional; i++) {
        int idx;

        do {
            idx = rand() % (numTests - 1);
        } while (used[idx]);

        used[idx] = 1; 

        strcat(result, "+");
        strcat(result, tests[idx].labTests);
        *totalPrice += tests[idx].prices;
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

bool patWriteData(User data, char *fileName)
{
    FILE *patRecords;
    size_t structType;
    if (strcmp(fileName, "outpatient") == 0)
    {
        patRecords = fopen("outpatientRecords.bin", "ab");
        structType = sizeof(User);

        if (patRecords == NULL) return false;
    }

       if (strcmp(fileName, "inpatient") == 0)
    {
        patRecords = fopen("inpatientRecords.bin", "ab");
        structType = sizeof(User);
        
        if (patRecords == NULL) return false;

    }

    else {
        printf("Invalid Choice\n");
        return false;
    }

    if (fwrite(&data, structType, 1, patRecords) != 1) return false;

    if (fclose(patRecords)  == EOF) return false;

    return true;

}

User *patReadData(char *fileName)
{
    FILE *patRecords;
    size_t structType;
    if (strcmp(fileName, "outpatient") == 0)
    {
        patRecords = fopen("outpatientRecords.bin", "rb");
        structType = sizeof(User);

        if (patRecords == NULL) return NULL;
    }

       if (strcmp(fileName, "inpatient") == 0)
    {
        patRecords = fopen("inpatientRecords.bin", "rb");
        structType = sizeof(User);
        
        if (patRecords == NULL) return NULL;

    }
    
    else {
        printf("Invalid Choice\n");
        return NULL;
    }

    fseek(patRecords, 0, SEEK_END);
    int fileSize = ftell(patRecords);
    fseek(patRecords, 0, SEEK_SET);

    int recordTotal = fileSize / structType;

    User *bufferRead = malloc(structType * recordTotal);

    if (fread(bufferRead, structType, recordTotal, patRecords) != 1) return NULL;

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

void selectHMO(User *u) {
    int choice;
    printf("\nSelect HMO:\n");
    for (int i = 0; i < 5; i++)
        printf("%d. %s\n", i + 1, hmoList[i]);
    printf("Choice: ");
    scanf("%d", &choice);
    getchar();
    if (choice >= 1 && choice <= 5)
        strcpy(u->patHMO, hmoList[choice - 1]);
}

void selectPayment(User *u) {
    int choice;
    printf("\nSelect Payment Method:\n");
    for (int i = 0; i < 4; i++)
        printf("%d. %s\n", i + 1, paymentMethods[i]);
    printf("Choice: ");
    scanf("%d", &choice);
    getchar();
    if (choice >= 1 && choice <= 4)
        strcpy(u->patBilling.paymentMode, paymentMethods[choice - 1]);
}

void selectRoom(User *u) {
    int choice;
    printf("\nSelect Room Type:\n");
    for (int i = 0; i < 6; i++)
        printf("%d. %s\n", i + 1, hospitalRooms[i].roomType);
    printf("Choice: ");
    scanf("%d", &choice);
    getchar();
    if (choice >= 1 && choice <= 6)
        strcpy(u->hospitalRooms.roomType, hospitalRooms[choice - 1].roomType);
}

void selectSymptoms(User *u) {
    int choice;
    char temp[200];
    u->patSymptoms[0] = '\0';

    printf("\nSelect Symptoms (enter numbers separated by space, 0 to finish):\n");
    for (int i = 0; i < MAX_SYMPTOMS; i++)
        printf("%d. %s\n", i + 1, symptomsList[i]);
    printf("%d. Other\n", MAX_SYMPTOMS + 1);

    while (1) {
        scanf("%d", &choice);
        getchar();
        if (choice == 0) break;

        if (choice == MAX_SYMPTOMS + 1) {
            printf("Enter other symptom: ");
            fgets(temp, sizeof(temp), stdin);
            removeNewline(temp);
        } else if (choice >= 1 && choice <= MAX_SYMPTOMS) {
            strcpy(temp, symptomsList[choice - 1]);
        } else {
            continue;
        }

        if (strlen(u->patSymptoms) > 0)
            strcat(u->patSymptoms, "+");
        strcat(u->patSymptoms, temp);
    }
}

void collectPatientInfo(User *u, int patientType) {
    int choice, done = 0;
    u->patID = unique_IDGen();
    strcpy(u->date, currentDate());
    getRandomLabTestsString(u->patBilling.billBreakdown, &u->patBilling.paymentAmount);
    getRandomDepartment(u->patDepartment, u->patAssignedDoctor);

    printf("Enter patient name: ");
    fgets(u->patName, sizeof(u->patName), stdin);
    removeNewline(u->patName);

    printf("Enter patient age: ");
    fgets(u->patAge, sizeof(u->patAge), stdin);
    removeNewline(u->patAge);

    printf("Enter date of birth (DD/MM/YYYY): ");
    fgets(u->patDOB, sizeof(u->patDOB), stdin);
    removeNewline(u->patDOB);

    printf("Enter sex (M/F): ");
    fgets(u->patSex, sizeof(u->patSex), stdin);
    removeNewline(u->patSex);

    printf("Enter contact number: ");
    fgets(u->patContactNumber, sizeof(u->patContactNumber), stdin);
    removeNewline(u->patContactNumber);

    printf("Enter full address: ");
    fgets(u->patFullAddress, sizeof(u->patFullAddress), stdin);
    removeNewline(u->patFullAddress);   

    printf("Is the patient a Senior Citizen or PWD? (yes/no): ");
    fgets(u->isSeniorOrPwd, sizeof(u->isSeniorOrPwd), stdin);
    removeNewline(u->isSeniorOrPwd);

    selectHMO(u);
    selectPayment(u);
    if (patientType == 1) {
        selectRoom(u);
        printf("Enter number of days of stay: ");
        fgets(u->patDaysofStay, sizeof(u->patDaysofStay), stdin);
        removeNewline(u->patDaysofStay);
    } else {
        strcpy(u->hospitalRooms.roomType, "N/A");
        strcpy(u->patDaysofStay, "0");
    }
    selectSymptoms(u);

    do {
        printf("\n--- Review Patient Information ---\n");
        printf("Name: %s\nAge: %s\nDOB: %s\nSex: %s\nContact: %s\nAddress: %s\n",
               u->patName, u->patAge, u->patDOB, u->patSex, u->patContactNumber, u->patFullAddress);
        printf("Senior/PWD: %s\nHMO: %s\nPayment Method: %s\nRoom: %s\nDays of Stay: %s\nSymptoms: %s\n",
               u->isSeniorOrPwd, u->patHMO, u->patBilling.paymentMode, u->hospitalRooms.roomType, u-> patDaysofStay, u->patSymptoms);

        printf("\nConfirm info? (1-Yes / 2-Edit): ");
        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 2) {
            printf("\nSelect what to edit:\n");
            printf("1. Name\n2. Age\n3. DOB\n4. Sex\n5. Contact\n6. Address\n");
            printf("7. Senior/PWD\n8. HMO\n9. Payment Method\n10. Room\n11. Symptoms\n");
            printf("Choice: ");
            scanf("%d", &choice);
            clearInputBuffer();

            switch (choice) {
                case 1: printf("Enter new name: "); fgets(u->patName, sizeof(u->patName), stdin); break;
                case 2: printf("Enter new age: "); fgets(u->patAge, sizeof(u->patAge), stdin); break;
                case 3: printf("Enter new DOB: "); fgets(u->patDOB, sizeof(u->patDOB), stdin); break;
                case 4: printf("Enter new sex: "); fgets(u->patSex, sizeof(u->patSex), stdin); break;
                case 5: printf("Enter new contact: "); fgets(u->patContactNumber, sizeof(u->patContactNumber), stdin); break;
                case 6: printf("Enter new address: "); fgets(u->patFullAddress, sizeof(u->patFullAddress), stdin); break;
                case 7: printf("Enter Senior/PWD status: "); fgets(u->isSeniorOrPwd, sizeof(u->isSeniorOrPwd), stdin); break;
                case 8: selectHMO(u); break;
                case 9: selectPayment(u); break;
                case 10: if (patientType == 1) {selectRoom(u); printf("Enter number of days of stay: "); fgets(u->patDaysofStay, sizeof(u->patDaysofStay), stdin); removeNewline(u->patDaysofStay);} else {printf("Invalid choice.\n"); } break;

                case 11: selectSymptoms(u); break;
                default: printf("Invalid choice.\n"); continue;
            }

            removeNewline(u->patName);
            removeNewline(u->patAge);
            removeNewline(u->patDOB);
            removeNewline(u->patSex);
            removeNewline(u->patContactNumber);
            removeNewline(u->patFullAddress);
            removeNewline(u->isSeniorOrPwd);
        } else {
            done = 1;
        }
    } while (!done);
}

void billingCalculate(User *u, int patientType)
{
    float labTotal = 0;
    float roomCost = 0;
    float subtotal, coveredByHMO = 0, patientPays, totalDue;

    if (patientType == 1) {
        for (int i = 0; i < 6; i++) {
            if (strcmp(u->hospitalRooms.roomType, hospitalRooms[i].roomType) == 0) {
                roomCost = hospitalRooms[i].roomPrice * atoi(u->patDaysofStay);
                break;
            }
        }
    }

    printf("\n===============================================\n");
    printf("             Makulay General Hospital\n");
    printf("               Levitown, Lipa City\n");
    printf("       Tel: (02) 8123-4567 | TIN: 004-567-890\n");
    printf("===============================================\n");
    printf("Patient Name: %s\n", u->patName);
    printf("Patient ID  : %d\n", u->patID);
    printf("Date        : %s\n", u->date);
    printf("Assigned Doctor: %s\n", u->patAssignedDoctor);
    printf("Department: %s\n", u->patDepartment);
    printf("Payment Mode: %s\n", u->patBilling.paymentMode);
    printf("===============================================\n");

    printf("Lab Tests:\n");
    printf("%-35s %10s\n", "Test Name", "Price(₱)");
    printf("-----------------------------------------------\n");

    char temp[200];
    char *token = strtok(u->patBilling.billBreakdown, "+");
    while (token != NULL) {
        for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
            if (strcmp(token, tests[i].labTests) == 0) {
                printf("%-35s %10.2f\n", tests[i].labTests, (float)tests[i].prices);
                labTotal += tests[i].prices;
                break;
            }
        }
        token = strtok(NULL, "+");
    }

    if (patientType == 1) {
        printf("\nRoom Charges:\n");
        printf("%-35s %10s\n", "Room Type (Days)", "Cost(₱)");
        printf("-----------------------------------------------\n");
        printf("%-25s (%s days) %10.2f\n", u->hospitalRooms.roomType, u->patDaysofStay, roomCost);
    }

    subtotal = labTotal + roomCost;

    if (strcmp(u->patHMO, "None") != 0) {
        coveredByHMO = subtotal * 0.60;
        patientPays = subtotal * 0.40;
    } else {
        patientPays = subtotal;
    }

    if (strcasecmp(u->isSeniorOrPwd, "yes") == 0) {
        patientPays *= 0.80;
    }

    totalDue = patientPays;
    u->patBilling.paymentAmount = (int)totalDue;

    printf("\nSummary:\n");
    printf("%-35s %10.2f\n", "Subtotal", subtotal);
    printf("%-35s %10.2f\n", "Covered by HMO", coveredByHMO);
    printf("%-35s %10s\n", "Discounts Applied", strcasecmp(u->isSeniorOrPwd, "yes") == 0 ? "Senior/PWD (20%)" : "None");
    printf("%-35s %10.2f\n", "TOTAL DUE", totalDue);
    printf("===============================================\n\n");
    printf("     Thank you for trusting Makulay GH!\n");
    printf("    Wishing you a speedy recovery and health.\n");
    printf("===============================================\n\n");
}

void viewRecords(char *fileName)
{
    int patientType = (strcmp(fileName, "inpatient") == 0) ? 1 : 2;
    User *records = patReadData(fileName);
    if (records == NULL) {
        printf("No records found or error reading file.\n");
        return;
    }

    int totalRecords = sizeof(records) / sizeof(records[0]);

    printf("\n==============================================================\n");
    printf(patientType == 1 ? "                 Inpatient Records\n" : "                 Outpatient Records\n");
    printf("==============================================================\n");

    printf("| %-7s | %-15s | %-3s | %-3s | %-10s | %-12s | %-4s | %-10s |\n",
           "ID", "Name", "Age", "Sex", "HMO", "Room", "Days", "Total(₱)");
    printf("|---------|-----------------|-----|-----|------------|--------------|------|------------|\n");

    for (int i = 0; i < totalRecords; i++) 
    {
        char name[16]; strncpy(name, records[i].patName, 15); name[15] = '\0';
        char hmo[11]; strncpy(hmo, records[i].patHMO, 10); hmo[10] = '\0';
        char room[13]; strncpy(room, patientType == 1 ? records[i].hospitalRooms.roomType : "N/A", 12); room[12] = '\0';

        printf("| %-7d | %-15s | %-3s | %-3s | %-10s | %-12s | %-4s | %-10d |\n",
               records[i].patID,
               name,
               records[i].patAge,
               records[i].patSex,
               hmo,
               room,
               patientType == 1 ? records[i].patDaysofStay : "0",
               records[i].patBilling.paymentAmount);
    }

    printf("==============================================================\n\n");
    free(records);

}


void menu()
    {
        int choice;
        do
        {
        printf("==============================================\n");
        printf("  __  __    _    _  ___   _ _        _ __   __\n");
        printf(" |  \\/  |  / \\  | |/ / | | | |      / \\\\ \\ / /\n");
        printf(" | |\\/| | / _ \\ | ' /| | | | |     / _ \\\\ V / \n");
        printf(" | |  | |/ ___ \\| . \\| |_| | |___ / ___ \\| |  \n");
        printf(" |_|  |_/_/   \\_\\_|\\_\\\\___/|_____/_/   \\_\\_|  \n");
        printf("                                              \n");
        printf("==============================================\n");
        printf("      Welcome to Makulay General Hospital\n");
        printf("             Levitown, Lipa City\n");
        printf("       Tel: (02) 8123-4567 | TIN: 004-567-890\n");
        printf("==============================================\n");
        printf("               Main Menu\n");
        printf("\n1. Inpatient Registration\n");
        printf("2. Outpatient Registration\n");
        printf("3. View Records\n");
        printf("4. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 1)
        {
            User inpatient;
            collectPatientInfo(&inpatient, 1);
            billingCalculate(&inpatient, 1);
            patWriteData(inpatient, "inpatient");;
        }
        else if (choice == 2)
        {
            User outpatient;
            collectPatientInfo(&outpatient, 2);
            billingCalculate(&outpatient, 2);
            patWriteData(outpatient, "outpatient");
        }
        else if (choice == 3)
        {
            int viewChoice;
            do {
                printf("\n 1. View Inpatient Records\n");
                printf(" 2. View Outpatient Records\n");
                printf(" 3. Back to Main Menu\n");
                printf(" Enter your choice: ");
                scanf("%d", &viewChoice);
                clearInputBuffer();

                switch(viewChoice) {
                    case 1: viewRecords("inpatient"); break;
                    case 2: viewRecords("outpatient"); break;
                    case 3: break; 
                    default: printf("Invalid choice.\n"); break;
                }

            } while (viewChoice != 3);

        }
        else if (choice == 4)
        {
            printf("Exiting the program. Goodbye!\n");
            exit(0);
        }
        else
        {
            printf("Invalid choice. Please try again.\n");


        } 
        }while (choice != 4);

    }

int main()
{
    // menu();
    User *testpatient = patReadData("inpatient");
    printf("%d\n", testpatient[0].patID);


    free(testpatient);

    return 0;
    
}
