#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE* data; //opening file - if file doesn't exist, it will create a new one
    data = fopen("patientRecords.bin", "r+");
    if (data == NULL)
    {
        fopen("patientRecords.bin", "w+");
        if (data == NULL)
        {
            printf("ERROR : File cannot be accessed\n");
            return 1;
        }
    }

    typedef struct
    {
        int ordinal_ID;
        int Pat_ID;
        int date;
        char Pat_Type;
        char Pat_Name[50];
    } User;
    User Patient;
    
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

    int last_ID(data)
    {
        User buffer;
        if (ftell(data) == 0)
        {
            printf("No content within the file.\n");
            return 1;
        } else
        {
            fseek(data, -sizeof(User), SEEK_END);
            fread(buffer, sizeof(User), 1, data);
            return buffer.ordinal_ID;

        }
    }

    int patient_Order = last_ID(data);

        int unique_IDGen()
    {
        // loop through all unique ids while (fread(   ))
    }

    
    int patient_Registration(int patient_Type)
    {
        if (patient_Type == 1)
        {
            printf("What is your name?: ");
            scanf("%[^\n]", Patient.name);


            
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

    
    fclose(data);
        printf("The file is now closed.\n");

}