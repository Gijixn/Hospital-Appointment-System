                                 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct
    {
        int Pat_ID;
        int date;
        char Pat_Name[50];
    } User;
    
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

int unique_IDGen()
{
    // loop through all unique ids while (fread(   ))
    
}


// int patient_Registration(int patient_Type)
// {
//     if (patient_Type == 1)
//     {
//         printf("What is your name?: ");
//         scanf("%[^\n]", Patient.name);


        
//     }
// }

bool patWriteData(User data, int choice)
{
    if (choice == 1)
    {
        FILE *outData;
        outData = fopen("outpatientRecords.bin", "ab");
        
        if (outData == NULL) return false;

        if (fwrite(&data, sizeof(User), 1, outData) != 1) return false;

        if (fclose(outData)  == EOF) return false;

    }

       else if (choice == 2)
    {
        FILE *inData;
        inData = fopen("inpatientRecords.bin", "ab");
        
        if (inData == NULL) return false;

        if (fwrite(&data, sizeof(User), 1, inData) != 1) return false;

        if (fclose(inData)  == EOF) return false;

    } else return false;

}

int main()
{
    User buffer;

    buffer.Pat_ID = 20;
    buffer.date = 12;
    strcpy(buffer.Pat_Name, "David");

    patWriteData(buffer, 1);

    // while(1)
    // {
    //     int choice = menu();
    //     if (choice == 3)
    //     {
    //         break;
    //     }
    //     patient_Registration(choice);
    // }
}
