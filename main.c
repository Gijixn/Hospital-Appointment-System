#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE* dataptr; //opening file - if file doesn't exist, it will create a new one
    dataptr = fopen("patientRecords.txt", "r+");
    if (dataptr == NULL)
    {
        fopen("patientRecords.txt", "w+");
        if (dataptr == NULL)
        {
            printf("ERROR : File cannot be accessed\n");
            return 1;
        }
    }

    typedef struct
    {
        int Pat_ID;
        int Date;
        char Pat_Name[50];
    } User;

    User inpatient[100];
    User outpatient[100];
    char test[50];
    int inpatient_Registered = 0;
    int outpatient_Registered = 0;
    
    int unique_IDGen()
    {
        // store and check for existing id
        // generate 1000000 + rand() % 9000001;
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

    int patient_Registration(int patient_Type)
    {
        switch(patient_Type)
        {
            case 1:
            printf("Enter your name: ");
            scanf("%s", test);
            fputs(test, dataptr);
            fputs("\n", dataptr);
            break;
            case 2:
            //outpatient store
            break;
            default:
            printf("Invalid Input in patient_Registration");
            break;
        }
    }

    while(1)
    {
        int choice = menu();
        if (choice == 3)
        {
            break;
        }
        patient_Registration(choice);
    }

    
    fclose(dataptr);
        printf("The file is now closed.\n");

}