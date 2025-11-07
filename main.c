#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void menu();

int main() {
    menu();
    return 0;
}

void menu()
{
    int choice, type;

    do {
        printf("\n===== MAIN MENU =====\n");
        printf("1. Registration\n");
        printf("2. View\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("\n--- REGISTRATION ---\n");
                printf("1. Inpatient\n");
                printf("2. Outpatient\n");
                printf("Enter your choice: ");
                scanf("%d", &type);

                if(type == 1)
                    printf("You selected: Registration - Inpatient\n");
                else if(type == 2)
                    printf("You selected: Registration - Outpatient\n");
                else
                    printf("Invalid choice for Registration.\n");
                break;

            case 2:
                printf("\n--- VIEW ---\n");
                printf("1. Inpatient\n");
                printf("2. Outpatient\n");
                printf("Enter your choice: ");
                scanf("%d", &type);

                if(type == 1)
                    printf("You selected: View - Inpatient\n");
                else if(type == 2)
                    printf("You selected: View - Outpatient\n");
                else
                    printf("Invalid choice for View.\n");
                break;

            case 3:
                printf("\nExiting program...\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while(choice != 3);
}

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
    *totalPrice = 0;
    result[0] = '\0';

    strcat(result, "Health Consultation");
    *totalPrice += tests[numTests - 1].prices;

    int additional = rand() % 3;

    for (int i = 0; i < additional; i++) {
        int idx = rand() % (numTests - 1);
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
    
       if (strcmp(fileName, "inpatientBill") == 0)
    {
        patRecords = fopen("inpatientBills.bin", "ab");
        structType = sizeof(billing);
        
        if (patRecords == NULL) return false;

    }

       if (strcmp(fileName, "outpatientBill") == 0)
    {
        patRecords = fopen("outpatientBills.bin", "ab");
        structType = sizeof(billing);
        
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
        patRecords = fopen("outpatientRecords.bin", "ab");
        structType = sizeof(User);

        if (patRecords == NULL) return NULL;
    }

       if (strcmp(fileName, "inpatient") == 0)
    {
        patRecords = fopen("inpatientRecords.bin", "ab");
        structType = sizeof(User);
        
        if (patRecords == NULL) return NULL;

    }
    
       if (strcmp(fileName, "inpatientBill") == 0)
    {
        patRecords = fopen("inpatientBills.bin", "ab");
        structType = sizeof(billing);
        
        if (patRecords == NULL) return NULL;

    }

       if (strcmp(fileName, "outpatientBill") == 0)
    {
        patRecords = fopen("outpatientBills.bin", "ab");
        structType = sizeof(billing);
        
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
            temp[strcspn(temp, "\n")] = 0;
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

void collectPatientInfo(User *u) {
    int choice, done = 0;
    u.patID = unique_IDGen();
    strcpy(u->date, currentDate());
    getRandomLabTestsString(u->patBilling.billBreakdown, &u->patBilling.paymentAmount);
    getRandomDepartment(u->patDepartment, u->patAssignedDoctor);

    printf("Enter patient name: ");
    fgets(u->patName, sizeof(u->patName), stdin);
    u->patName[strcspn(u->patName, "\n")] = 0;

    printf("Enter patient age: ");
    fgets(u->patAge, sizeof(u->patAge), stdin);
    u->patAge[strcspn(u->patAge, "\n")] = 0;

    printf("Enter date of birth (DD/MM/YYYY): ");
    fgets(u->patDOB, sizeof(u->patDOB), stdin);
    u->patDOB[strcspn(u->patDOB, "\n")] = 0;

    printf("Enter sex (M/F): ");
    fgets(u->patSex, sizeof(u->patSex), stdin);
    u->patSex[strcspn(u->patSex, "\n")] = 0;

    printf("Enter contact number: ");
    fgets(u->patContactNumber, sizeof(u->patContactNumber), stdin);
    u->patContactNumber[strcspn(u->patContactNumber, "\n")] = 0;

    printf("Enter full address: ");
    fgets(u->patFullAddress, sizeof(u->patFullAddress), stdin);
    u->patFullAddress[strcspn(u->patFullAddress, "\n")] = 0;

    printf("Is the patient a Senior Citizen or PWD? (yes/no): ");
    fgets(u->isSeniorOrPwd, sizeof(u->isSeniorOrPwd), stdin);
    u->isSeniorOrPwd[strcspn(u->isSeniorOrPwd, "\n")] = 0;

    selectHMO(u);
    selectPayment(u);
    selectRoom(u);
    selectSymptoms(u);

    do {
        printf("\n--- Review Patient Information ---\n");
        printf("Name: %s\nAge: %s\nDOB: %s\nSex: %s\nContact: %s\nAddress: %s\n",
               u->patName, u->patAge, u->patDOB, u->patSex, u->patContactNumber, u->patFullAddress);
        printf("Senior/PWD: %s\nHMO: %s\nPayment Method: %s\nRoom: %s\nSymptoms: %s\n",
               u->isSeniorOrPwd, u->patHMO, u->patBilling.paymentMode, u->hospitalRooms.roomType, u->patSymptoms);

        printf("\nConfirm info? (1-Yes / 2-Edit): ");
        scanf("%d", &choice);
        getchar();

        if (choice == 2) {
            printf("\nSelect what to edit:\n");
            printf("1. Name\n2. Age\n3. DOB\n4. Sex\n5. Contact\n6. Address\n");
            printf("7. Senior/PWD\n8. HMO\n9. Payment Method\n10. Room\n11. Symptoms\n");
            printf("Choice: ");
            scanf("%d", &choice);
            getchar();

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
                case 10: selectRoom(u); break;
                case 11: selectSymptoms(u); break;
                default: printf("Invalid choice.\n"); continue;
            }

            u->patName[strcspn(u->patName, "\n")] = 0;
            u->patAge[strcspn(u->patAge, "\n")] = 0;
            u->patDOB[strcspn(u->patDOB, "\n")] = 0;
            u->patSex[strcspn(u->patSex, "\n")] = 0;
            u->patContactNumber[strcspn(u->patContactNumber, "\n")] = 0;
            u->patFullAddress[strcspn(u->patFullAddress, "\n")] = 0;
            u->isSeniorOrPwd[strcspn(u->isSeniorOrPwd, "\n")] = 0;
        } else {
            done = 1;
        }
    } while (!done);
}


void generateAndPrintBill(User *u)
{
    int roomCost = 0;

    // Find the selected room’s cost
    for (int i = 0; i < 6; i++) {
        if (strcmp(u->hospitalRooms.roomType, hospitalRooms[i].roomType) == 0) {
            roomCost = hospitalRooms[i].roomPrice;
            break;
        }
    }

    float totalCost = roomCost + u->patBilling.paymentAmount;
    float discount = 0.0, hmoCoverage = 0.0, amountDue = 0.0;

    // Apply senior/PWD discount
    if (strcasecmp(u->isSeniorOrPwd, "yes") == 0)
        discount = 0.20 * totalCost;

    // Apply HMO coverage if any HMO selected
    if (strcmp(u->patHMO, "None") != 0)
        hmoCoverage = 0.60 * (totalCost - discount);

    amountDue = totalCost - discount - hmoCoverage;

    // Save to struct
    u->patBilling.paymentAmount = (int)amountDue;
    sprintf(u->patBilling.billBreakdown,
            "Room: %s (₱%d)\n"
            "Lab Tests: %s (₱%d)\n"
            "Discount: ₱%.2f\n"
            "HMO (60%%): ₱%.2f\n"
            "Total Due: ₱%.2f\n",
            u->hospitalRooms.roomType, roomCost,
            u->patLab.labTests, u->patLab.labCost,
            discount, hmoCoverage, amountDue);

    // Print final receipt


    
    printf("\n================================================\n");
    printf("              Makulay General Hospital\n");
    printf("                 Lipa City, Batangas\n");
    printf("================================================\n");
    printf("Date/Time:        %s\n", u->date);
    printf("Patient Name:     %s\n", u->patName);
    printf("Patient ID:       %d\n", u->patID);
    printf("Department:       %s\n", u->patDepartment);
    printf("Assigned Doctor:  %s\n", u->patAssignedDoctor);
    printf("HMO:              %s\n", u->patHMO);
    printf("Senior/PWD:       %s\n", u->isSeniorOrPwd);
    printf("------------------------------------------------\n");
    printf("%s", u->patBilling.billBreakdown);
    printf("Payment Method:   %s\n", u->patBilling.paymentMode);
    printf("================================================\n");
    printf("   Thank you for trusting Makulay General Hospital!\n");
    printf("================================================\n\n");
}


// void printTable(int choice) //tentative
// {
//     User *records = patReadData(choice);
//     if (records == NULL)
//     {
//         printf("\nNo records found.\n");
//         return;
//     }
//      FILE *patRecords;
//     if (choice == 1)
//     {
//         patRecords = fopen("inpatientRecords.bin", "rb");
//         if (patRecords == NULL)
//         {
//             printf("\nNo records found.\n");
//             return;
//         }
//     }

//     if (choice == 2)
//     {
//         patRecords = fopen("outpatientRecords.bin", "rb");
//         if (patRecords == NULL)
//         {
//             printf("\nNo records found.\n");
//             return;
//         }
//     }

//     fseek(patRecords, 0, SEEK_END);
//     int fileSize = ftell(patRecords);
//     fseek(patRecords, 0, SEEK_SET);

//     int recordTotal = fileSize / sizeof(User);

//     printf("\n---------------------------------------------------------------------------------------------------------------\n");
//     for (int i = 0; i < recordTotal + 1; i++)
//     {
//         User u;
//         if (fread(&u, sizeof(User), 1, patRecords) != 1) break;

//         printf("| ID: %d | Name: %s | DOB: %s | Contact: %s | Sex: %s | Address: %s | Height: %s | Weight: %s | Diagnosis: %s | Facility: %s | Payment Mode: %s | Payment Amount: %d |\n",
//                u.patID, u.patName, u.patDOB, u.patContactNumber, u.patSex, u.patFullAddress,
//                u.patHeight, u.patWeight, u.patDiagnosis, u.patFacility, u.patModeofPayment, u.patPaymentAmount);
//     }
//     printf("---------------------------------------------------------------------------------------------------------------\n");
//     fclose(patRecords);
//     free(records);
// }


int menu()
    {
        int choice;
        printf("\n1. Inpatient Registration\n");
        printf("2. Outpatient Registration\n");
        printf("3. View Records\n");
        printf("4. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        return choice;
    }
 void personalDetails();
 {
 void printLine() {
    printf("+--------------------------------------------------------------------------------------------------------------+\n");
}

void printHeader() {
    printLine();
    printf("| %-5s | %-10s | %-15s | %-3s | %-10s | %-6s | %-12s | %-15s | %-10s | %-12s | %-10s | %-15s | %-10s |\n",
           "ID", "Date", "Name", "Age", "DOB", "Sex", "Contact", "Department", "HMO", "Doctor", "Symptom", "Address", "Sr/PWD");
    printLine();
}

int main() {
    struct Patient p[MAX];
    int n, i;

    printf("Enter number of patients (max %d): ", MAX);
    scanf("%d", &n);
    getchar(); // clear buffer

    for (i = 0; i < n; i++) {
        printf("\n--- Enter details for Patient %d ---\n", i + 1);
        printf("Patient ID: ");
        scanf("%d", &p[i].patID); getchar();
        printf("Date (MM/DD/YYYY): ");
        fgets(p[i].date, sizeof(p[i].date), stdin); strtok(p[i].date, "\n");
        printf("Name: ");
        fgets(p[i].name, sizeof(p[i].name), stdin); strtok(p[i].name, "\n");
        printf("Age: ");
        scanf("%d", &p[i].age); getchar();
        printf("Date of Birth (MM/DD/YYYY): ");
        fgets(p[i].dob, sizeof(p[i].dob), stdin); strtok(p[i].dob, "\n");
        printf("Sex: ");
        fgets(p[i].sex, sizeof(p[i].sex), stdin); strtok(p[i].sex, "\n");
        printf("Contact Number: ");
        fgets(p[i].contact, sizeof(p[i].contact), stdin); strtok(p[i].contact, "\n");
        printf("Full Address: ");
        fgets(p[i].address, sizeof(p[i].address), stdin); strtok(p[i].address, "\n");
        printf("Symptoms: ");
        fgets(p[i].symptoms, sizeof(p[i].symptoms), stdin); strtok(p[i].symptoms, "\n");
        printf("Department: ");
        fgets(p[i].department, sizeof(p[i].department), stdin); strtok(p[i].department, "\n");
        printf("HMO: ");
        fgets(p[i].hmo, sizeof(p[i].hmo), stdin); strtok(p[i].hmo, "\n");
        printf("Assigned Doctor: ");
        fgets(p[i].doctor, sizeof(p[i].doctor), stdin); strtok(p[i].doctor, "\n");
        printf("Senior or PWD (Yes/No): ");
        fgets(p[i].seniorOrPwd, sizeof(p[i].seniorOrPwd), stdin); strtok(p[i].seniorOrPwd, "\n");
    }

    printf("\n\n================= INPATIENT RECORD TABLE =================\n");
    printHeader();
    for (i = 0; i < n; i++) {
        printf("| %-5d | %-10s | %-15s | %-3d | %-10s | %-6s | %-12s | %-15s | %-10s | %-12s | %-10s | %-15s | %-10s |\n",
               p[i].patID, p[i].date, p[i].name, p[i].age, p[i].dob, p[i].sex, p[i].contact,
               p[i].department, p[i].hmo, p[i].doctor, p[i].symptoms, p[i].address, p[i].seniorOrPwd);
        printLine(); 
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
            int viewChoice;
            printf("\nView Records:\n");
            printf("1. Inpatient Records\n");
            printf("2. Outpatient Records\n");
            printf("Enter your choice: ");
            scanf("%d", &viewChoice);

            if (viewChoice == 1 || viewChoice == 2)
            {
                // printTable(viewChoice);
                break;  
            }
            else
            {
                printf("\nInvalid choice. Please try again.\n");
            }
        }

        if (choice == 4)
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
