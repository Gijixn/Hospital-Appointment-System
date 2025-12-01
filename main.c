#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

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
    int paymentTotal;
    int paymentAmount;
    int balance;
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
    "Online Payment",
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

       else if (strcmp(fileName, "inpatient") == 0)
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

       else if (strcmp(fileName, "inpatient") == 0)
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
    printf("\nSelect HMO (HMOs covers 60%% of total payment):\n");
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
    printf("===============================================\n");
    printf("               ROOM PRICE TABLE                \n");
    printf("===============================================\n");
    printf("Room Type                 | Price (per day)\n");
    printf("===============================================\n");
    printf("General Ward              | ₱2000.00\n");
    printf("Semi-Private Room         | ₱4000.00\n");
    printf("Private Room              | ₱6000.00\n");
    printf("ICU                       | ₱10000.00\n");
    printf("Maternity Ward            | ₱5000.00\n");
    printf("===============================================\n");

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

void readString(const char *prompt, char *buffer, int size) {
    do {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        removeNewline(buffer);

        if (strlen(buffer) == 0)
            printf("Input cannot be empty. Try again.\n");
    } while (strlen(buffer) == 0);
}

void readInt(const char *prompt, char *buffer, int size) {
    int valid = 0;
    do {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        removeNewline(buffer);

        valid = 1;
        for (int i = 0; i < strlen(buffer); i++) {
            if (!isdigit(buffer[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid)
            printf("Invalid number. Try again.\n");

    } while (!valid);
}

void readSex(const char *prompt, char *buffer, int size) {
    do {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        removeNewline(buffer);

        if (strcmp(buffer, "M") != 0 && strcmp(buffer, "F") != 0) {
            printf("Invalid. Enter M or F.\n");
        } else break;
    } while (1);
}

void readYesNo(const char *prompt, char *buffer, int size) {
    do {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        removeNewline(buffer);

        if (strcmp(buffer, "yes") != 0 && strcmp(buffer, "no") != 0) {
            printf("Invalid. Enter yes or no.\n");
        } else break;
    } while (1);
}


void collectPatientInfo(User *u, int patientType) {
    int choice, done = 0;
    char tmp[10];

    u->patID = unique_IDGen();
    strcpy(u->date, currentDate());
    getRandomLabTestsString(u->patBilling.billBreakdown, &u->patBilling.paymentTotal);
    getRandomDepartment(u->patDepartment, u->patAssignedDoctor);

    readString("Enter patient name: ", u->patName, sizeof(u->patName));
    readInt("Enter patient age: ", u->patAge, sizeof(u->patAge));
    readString("Enter date of birth (DD/MM/YYYY): ", u->patDOB, sizeof(u->patDOB));
    readSex("Enter sex (M/F): ", u->patSex, sizeof(u->patSex));
    readString("Enter contact number: ", u->patContactNumber, sizeof(u->patContactNumber));
    readString("Enter full address: ", u->patFullAddress, sizeof(u->patFullAddress));
    readYesNo("Is the patient a Senior Citizen or PWD? (yes/no): ", u->isSeniorOrPwd, sizeof(u->isSeniorOrPwd));
    if (patientType == 1) {
        selectRoom(u);
        readInt("Enter number of days of stay: ", u->patDaysofStay, sizeof(u->patDaysofStay));
    } else {
        strcpy(u->hospitalRooms.roomType, "N/A");
        strcpy(u->patDaysofStay, "0");
    }
    selectSymptoms(u);
    selectHMO(u);
    selectPayment(u);

    do {
        printf("\n--- Review Patient Information ---\n");
        printf("Name: %s\nAge: %s\nDOB: %s\nSex: %s\nContact: %s\nAddress: %s\n",
               u->patName, u->patAge, u->patDOB, u->patSex, u->patContactNumber, u->patFullAddress);
        printf("Senior/PWD: %s\nHMO: %s\nPayment Method: %s\nRoom: %s\nDays of Stay: %s\nSymptoms: %s\n",
               u->isSeniorOrPwd, u->patHMO, u->patBilling.paymentMode, u->hospitalRooms.roomType,
               u->patDaysofStay, u->patSymptoms);

        readInt("\nConfirm info? (1-Yes / 2-Edit): ", tmp, sizeof(tmp));
        choice = atoi(tmp);

        if (choice == 2) {
            printf("\nSelect what to edit:\n");
            printf("1. Name\n2. Age\n3. DOB\n4. Sex\n5. Contact\n6. Address\n");
            printf("7. Senior/PWD\n8. Symptoms\n9. Room\n10. HMO\n11. Payment Method\n");
            readInt("Choice: ", tmp, sizeof(tmp));
            choice = atoi(tmp);

            switch (choice) {
                case 1: readString("Enter new name: ", u->patName, sizeof(u->patName)); break;
                case 2: readInt("Enter new age: ", u->patAge, sizeof(u->patAge)); break;
                case 3: readString("Enter new DOB: ", u->patDOB, sizeof(u->patDOB)); break;
                case 4: readSex("Enter new sex (M/F): ", u->patSex, sizeof(u->patSex)); break;
                case 5: readString("Enter new contact: ", u->patContactNumber, sizeof(u->patContactNumber)); break;
                case 6: readString("Enter new address: ", u->patFullAddress, sizeof(u->patFullAddress)); break;
                case 7: readYesNo("Enter Senior/PWD status (yes/no): ", u->isSeniorOrPwd, sizeof(u->isSeniorOrPwd)); break;
                case 8: selectSymptoms(u); break;
                case 9: if (patientType == 1) {
                            selectRoom(u);
                            readInt("Enter number of days of stay: ", u->patDaysofStay, sizeof(u->patDaysofStay));
                        } else {
                            printf("Invalid choice.\n");
                        }
                        break;
                case 10: selectHMO(u);
                case 11: selectPayment(u);
                default: printf("Invalid choice.\n"); continue;
            }
        } else if (choice == 1) {
            done = 1;
        } else {
            printf("Invalid input. Enter 1 to confirm or 2 to edit.\n");
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
    printf("=================BILLING SUMMARY=================\n");
    printf("Patient Name: %s\n", u->patName);
    printf("Patient ID  : %d\n", u->patID);
    printf("Date        : %s\n", u->date);
    printf("HMO           : %s\n", u->patHMO);
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
    u->patBilling.paymentTotal = (int)totalDue;

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

void removePatientRecord(int patID, int patientType)
{
    const char *fileName = (patientType == 1) ? "inpatientRecords.bin" : "outpatientRecords.bin";
    const char *tempName = "temp_records.bin";

    FILE *src = fopen(fileName, "rb");
    if (!src) {
        return;
    }

    FILE *tmp = fopen(tempName, "wb");
    if (!tmp) {
        fclose(src);
        return;
    }

    User buf;
    while (fread(&buf, sizeof(User), 1, src) == 1) {
        if (buf.patID != patID) {
            fwrite(&buf, sizeof(User), 1, tmp);
        }
    }

    fclose(src);
    fclose(tmp);

    if (remove(fileName) != 0) {
        remove(tempName);
        return;
    }
    rename(tempName, fileName);
}

bool updatePatientRecordPayment(User *u, int patientType)
{
    const char *fileName = (patientType == 1) ? "inpatientRecords.bin" : "outpatientRecords.bin";
    FILE *f = fopen(fileName, "r+b");
    if (!f) return false;

    User buf;
    long pos = 0;
    bool found = false;
    while (fread(&buf, sizeof(User), 1, f) == 1) {
        if (buf.patID == u->patID) {
            found = true;
            fseek(f, pos, SEEK_SET);
            if (fwrite(u, sizeof(User), 1, f) != 1) {
                fclose(f);
                return false;
            }
            break;
        }
        pos += sizeof(User);
    }

    fclose(f);
    return found;
}

void printReceipt(User *u, int patientType)
{
    if (!u) return;

    int payment = 0;
    int minPartial = (u->patBilling.paymentTotal * 20) / 100;

    printf("\n===== PAYMENT PROCESSING =====\n");
    printf("Patient: %s (ID: %d)\n", u->patName, u->patID);
    printf("Total Amount Due : ₱%d\n", u->patBilling.paymentTotal);
    printf("Payment Method   : %s\n\n", u->patBilling.paymentMode);

    while (1)
    {
        printf("Enter payment amount: ");
        if (scanf("%d", &payment) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }
        clearInputBuffer();

        if (payment < 0) {
            printf("Invalid payment. Amount must be positive.\n");
            continue;
        }

        if (payment < minPartial && payment < u->patBilling.paymentTotal) {
            printf("Minimum initial payment is 20%% of total (₱%d). Please pay at least this amount.\n", minPartial);
            continue;
        }

        u->patBilling.paymentAmount = payment;
        u->patBilling.balance = u->patBilling.paymentTotal - payment;

        if (u->patBilling.balance > 0) {
            printf("\nPartial payment accepted.\n");
        } else {
            u->patBilling.balance = 0;
            printf("\nFull payment complete!\n");
        }

        break;
    }

    int change = (payment > u->patBilling.paymentTotal) ? payment - u->patBilling.paymentTotal : 0;

    printf("\n===============================================\n");
    printf("             Makulay General Hospital\n");
    printf("               Levitown, Lipa City\n");
    printf("       Tel: (02) 8123-4567 | TIN: 004-567-890\n");
    printf("=================PAYMENT RECEIPT=================\n");
    printf("Patient Name : %s\n", u->patName);
    printf("Patient ID   : %d\n", u->patID);
    printf("Date         : %s\n", u->date);
    printf("Payment Method: %s\n", u->patBilling.paymentMode);
    printf("-----------------------------------------------\n");
    printf("%-30s %10.2f\n", "Total Due (₱)", (float)u->patBilling.paymentTotal);
    printf("%-30s %10.2f\n", "Amount Paid (₱)", (float)u->patBilling.paymentAmount);
    printf("%-30s %10.2f\n", "Remaining Balance (₱)", (float)u->patBilling.balance);
    printf("%-30s %10.2f\n", "Change (₱)", (float)change);
    printf("===============================================\n\n");
    printf("     Thank you for trusting Makulay GH!\n");
    printf("    Wishing you a speedy recovery and health.\n");
    printf("===============================================\n\n");
}



void viewRecords(char *fileName)
{
    FILE *patRecords;
    size_t structType = sizeof(User);

    if (strcmp(fileName, "outpatient") == 0)
        patRecords = fopen("outpatientRecords.bin", "rb");
    else if (strcmp(fileName, "inpatient") == 0)
        patRecords = fopen("inpatientRecords.bin", "rb");
    else {
        printf("Invalid file.\n");
        return;
    }

    if (!patRecords) {
        printf("No records found.\n");
        return;
    }

    fseek(patRecords, 0, SEEK_END);
    int fileSize = ftell(patRecords);
    fseek(patRecords, 0, SEEK_SET);

    int totalRecords = fileSize / structType;

    User *records = malloc(fileSize);

    fread(records, structType, totalRecords, patRecords);
    fclose(patRecords);

    int patientType = strcmp(fileName, "inpatient") == 0 ? 1 : 2;

    printf("\n=========================================================================================\n");
    printf(patientType == 1 ? "                           Inpatient Records\n"
                            : "                           Outpatient Records\n");
    printf("=========================================================================================\n");

    printf("| %-7s | %-15s | %-3s | %-3s | %-10s | %-12s | %-4s | %-10s |\n",
           "ID", "Name", "Age", "Sex", "HMO", "Room", "Days", "Balance(₱)");
    printf("|---------|-----------------|-----|-----|------------|--------------|------|------------|\n");

    for (int i = 0; i < totalRecords; i++)
    {
        char name[16]; strncpy(name, records[i].patName, 15); name[15] = '\0';
        char hmo[11]; strncpy(hmo, records[i].patHMO, 10); hmo[10] = '\0';
        char room[13];
        strncpy(room,
                patientType == 1 ? records[i].hospitalRooms.roomType : "N/A",
                12);
        room[12] = '\0';

        printf("| %-7d | %-15s | %-3s | %-3s | %-10s | %-12s | %-4s | %-10d |\n",
               records[i].patID,
               name,
               records[i].patAge,
               records[i].patSex,
               hmo,
               room,
               patientType == 1 ? records[i].patDaysofStay : "0",
               records[i].patBilling.balance);
    }

    printf("=========================================================================================\n\n");

    free(records);
}

void viewUserRecord(char *fileName) {
    FILE *patRecords;
    size_t structType = sizeof(User);

    if (strcmp(fileName, "outpatient") == 0)
        patRecords = fopen("outpatientRecords.bin", "rb");
    else if (strcmp(fileName, "inpatient") == 0)
        patRecords = fopen("inpatientRecords.bin", "rb");
    else {
        printf("Invalid file.\n");
        return;
    }

    if (!patRecords) {
        printf("No records found.\n");
        return;
    }

    fseek(patRecords, 0, SEEK_END);
    int fileSize = ftell(patRecords);
    fseek(patRecords, 0, SEEK_SET);

    int totalRecords = fileSize / structType;

    User *records = malloc(fileSize);

    fread(records, structType, totalRecords, patRecords);
    fclose(patRecords);

    int patID;
    printf("Enter Patient ID to view details (type 0 to cancel): ");
    scanf("%d", &patID);
    clearInputBuffer();

    if (patID == 0)
    {
        free(records);
        return;
    }

    bool found = false;
    for (int i = 0; i < totalRecords; i++) {
        if (records[i].patID == patID) {
            found = true;
            printf("\n================= Patient Detailed Record =================\n");

            printf("\n\n....................\n");
            printf("....................\n");
            printf("....................\n");
            printf("........+**+:.......\n");
            printf(".......+****+.......\n");
            printf(".......+****+.......\n");
            printf("........=++=........\n");
            printf("........:--::.......\n");
            printf(".....-********-.....\n");
            printf("....+**********+....\n");
            printf("...-************-...\n\n\n");


            printf("===========================================================\n");
            printf("Patient Name        : %s\n", records[i].patName);
            printf("Patient ID          : %d\n", records[i].patID);
            printf("Date Registered     : %s\n", records[i].date);
            printf("Age                 : %s\n", records[i].patAge);
            printf("Date of Birth       : %s\n", records[i].patDOB);
            printf("Sex                 : %s\n", records[i].patSex);
            printf("Contact Number      : %s\n", records[i].patContactNumber);
            printf("Full Address        : %s\n", records[i].patFullAddress);
            printf("===========================================================\n");
            printf("Room Type           : %s\n", records[i].hospitalRooms.roomType);
            printf("Days of Stay        : %s\n", records[i].patDaysofStay);
            printf("Assigned Department : %s\n", records[i].patDepartment);
            printf("Assigned Doctor     : %s\n", records[i].patAssignedDoctor);
            printf("Lab Tests           : %s\n", records[i].patBilling.billBreakdown);
            printf("===========================================================\n");
            printf("Senior/PWD          : %s\n", records[i].isSeniorOrPwd);
            printf("HMO                 : %s\n", records[i].patHMO);
            printf("Payment Method      : %s\n", records[i].patBilling.paymentMode);
            printf("Total Payment (₱ )  : %d\n", records[i].patBilling.paymentTotal);
            printf("Total Balance       : %d\n", records[i].patBilling.balance);
            printf("===========================================================\n\n");

            break;
        }
    }

    if (!found) {
        printf("Patient ID not found.\n");
    }

    free(records);
}

void repayBalance(int patientType) {
    char date[20];
    strcpy(date, currentDate());
    int patID;
    printf("Enter Patient ID to repay balance: ");
    scanf("%d", &patID);
    clearInputBuffer();


    const char *fileName = (patientType == 1) ? "inpatientRecords.bin" : "outpatientRecords.bin";
    FILE *f = fopen(fileName, "rb");
    if (!f) {
        printf("No records found.\n");
        return;
    }

    User buf;
    bool found = false;
    while (fread(&buf, sizeof(User), 1, f) == 1) {
        if (buf.patID == patID) {
            found = true;
            break;
        }
    }
    fclose(f);

    if (!found) {
        printf("Patient ID not found.\n");
        return;
    }

    if (buf.patBilling.balance <= 0) {
        printf("No outstanding balance for this patient.\n");
        return;
    }

    printf("Outstanding balance: ₱%d\n", buf.patBilling.balance);
    int minPayment = buf.patBilling.paymentTotal * 0.2;
    printf("Minimum payment required: ₱%d\n", minPayment);

    selectPayment(&buf);
    int payment;
    while (1) {
        printf("Enter payment amount: ");
        if (scanf("%d", &payment) != 1) {
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }
        clearInputBuffer();

        if (payment < minPayment) {
            printf("Payment must be at least 20%% of total due (₱%d).\n", minPayment);
            continue;
        }

        if (payment >= buf.patBilling.balance) {
            int change = payment - buf.patBilling.balance;
            buf.patBilling.paymentAmount += buf.patBilling.balance;
            buf.patBilling.balance = 0;
            printf("Full balance paid. Change: ₱%d\n", change);
        } else {
            buf.patBilling.paymentAmount += payment;
            buf.patBilling.balance -= payment;
            printf("Partial payment accepted. Remaining balance: ₱%d\n", buf.patBilling.balance);
        }
        break;
    }

    if (!updatePatientRecordPayment(&buf, patientType)) {
        printf("Error updating the record.\n");
        return;
    }

    printf("\n===============================================\n");
    printf("             Makulay General Hospital\n");
    printf("               Levitown, Lipa City\n");
    printf("       Tel: (02) 8123-4567 | TIN: 004-567-890\n");
    printf("=================PAYMENT RECEIPT=================\n");
    printf("Patient Name : %s\n", buf.patName);
    printf("Patient ID   : %d\n", buf.patID);
    printf("Date         : %s\n", date);
    printf("Payment Method: %s\n", buf.patBilling.paymentMode);
    printf("-----------------------------------------------\n");
    printf("%-30s %10.2f\n", "Amount Paid (₱)", (double)buf.patBilling.paymentAmount);
    printf("%-30s %10.2f\n", "Remaining Balance (₱)", (double)buf.patBilling.balance);
    printf("===============================================\n\n");
    printf("     Thank you for trusting Makulay GH!\n");
    printf("    Wishing you a speedy recovery and health.\n");
    printf("===============================================\n\n");
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
        printf("4. Balance Payment\n");
        printf("5. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 1)
        {
            User inpatient;
            printf("==============================================\n");
            collectPatientInfo(&inpatient, 1);
            billingCalculate(&inpatient, 1);
            printReceipt(&inpatient, 1);
            patWriteData(inpatient, "inpatient");
        }

        else if (choice == 2)
        {
            User outpatient;
            printf("==============================================\n");
            collectPatientInfo(&outpatient, 2);
            billingCalculate(&outpatient, 2);
            printReceipt(&outpatient, 2);
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
                    case 1: viewRecords("inpatient");  viewUserRecord("inpatient"); break;
                    case 2: viewRecords("outpatient");  viewUserRecord("outpatient"); break;
                    case 3: break;
                    default: printf("Invalid choice.\n"); break;
                }

            } while (viewChoice != 3);

        }
        else if (choice == 4)
        {
            int balChoice;
            do {
                printf("\n 1. Inpatient Balance Payment \n");
                printf(" 2. Outpatient Balance Payment\n");
                printf(" 3. Back to Main Menu\n");
                printf(" Enter your choice: ");
                scanf("%d", &balChoice);
                clearInputBuffer();

                switch(balChoice) {
                    case 1: repayBalance(1); break;
                    case 2: repayBalance(2); break;
                    case 3: break;
                    default: printf("Invalid choice.\n"); break;
                }

            } while (balChoice != 3);
        }

        else if (choice == 5)
        {
            printf("Exiting the program. Goodbye!\n");
            exit(0);
        }
        else
        {
            printf("Invalid choice. Please try again.\n");


        }
        }while (choice != 5);

    }

int main()
{
    menu();

    return 0;

}
