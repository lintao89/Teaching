#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void XML(char HL7[],int opt);
void JSON(int opt);
void Fulljson(char al[]);

int main()
{
    FILE* fptr;  //�}HL7���ɮ׫���
    FILE* fptr2; //�gXML���ɮ׫���
    char HL7[500] = { "" };  //��HL7���}�C
    char data[50] = { "" };    //AL1(Allergic)
    int i = 0, opt = 0;
    /**********�{���}�l**********/

    if ((fptr = fopen("HL7.hl7", "r")) != NULL)
    {
        while (!feof(fptr)) //�٨S���ɮ׵������ܴN�i�JŪ��
        {
            fscanf(fptr, "%c", &HL7[i]); //�N�ɮ׸�Ƨ��}�C�̭�
            i++;
        }
        printf("%s\n", HL7);
        fclose(fptr);
    }
    else
    {
        printf("�}�ɥ���!\n");
    }

    while (opt != -1)
    {
        i = 0;
        printf("\n1)�f�wPID�X2)�L�ӥN�X3)�E�_����4)��X����FHIR�ɮ�0)�NXML�ɮ���JSON�榡-1)���}:");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            XML(HL7,opt);
            break;
        case 2:
            XML(HL7, opt);
            break;
        case 3:
            XML(HL7, opt);
            break;
        case 4:
            Fulljson(data);
            break;
        case 0:
            printf("\n1)�N�f�H��T(PID)�ഫ��JSON\n2)�N�L�Ӹ�T(AL1)�ഫ��JSON\n3)�N�E�_��T(DG1)�ഫ��JSON\n0)�^�D���\n�ﶵ:");
            scanf("%d", &opt);

            switch (opt)
            {
            case 1:
                JSON(opt);
                break;
            case 2:
                JSON(opt);
                break;
            case 3:
                JSON(opt);
                break;
            case 0:
                break;
            default:
                printf("��J���~�Э��s��J!\n");
                break;
            }
            break;
        case -1:
            break;
        default:
            printf("��J���~�Э��s��J!\n");
            break;
        }
    }

    return 0;
}
void XML(char HL7[],int opt) {
    FILE* fptr;  //�}HL7���ɮ׫���
    char* ptr;
    char data[50] = { "" };    //PID(Patient ID)
    int i = 0;
    char tf; //�O�_����(True or False)
    if (opt == 1) {
        ptr = strstr(HL7, "PID");
        if (ptr)
        {
            ptr += 4;
            for (i = 0; i < 10; i++)
            {
                data[i] = *ptr;
                ptr++;
            }
            data[10] = '\0';
            printf("%s", data);
        }
    }
    else if (opt == 2) {
        ptr = strstr(HL7, "AL1");
        if (ptr)
        {
            ptr += 7;
            for (i = 0; i < 10; i++)
            {
                data[i] = *ptr;
                ptr++;
                if (ptr == NULL)
                {
                    break;
                }
            }
            data[i] = '\0';
            printf("%s", data);
        }
    }
    else if (opt == 3) {
        ptr = strstr(HL7, "DG1");
        if (ptr)
        {
            ptr += 13;
            for (i = 0; i < 20; i++)
            {
                data[i] = *ptr;
                ptr++;
                if (ptr == NULL)
                {
                    break;
                }
            }
            data[i] = '\0';
            printf("%s", data);
        }
    }
    

    printf("\n�O�_��XML��? y/n:");
    scanf(" %c", &tf);
    if (tf == 'y')
    {
        if (opt == 1) {
            if ((fptr = fopen("PID", "w+")) != NULL)
            {
                fprintf(fptr, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", data, data, data, data, data);
                fclose(fptr);
            }
        }
        else if (opt == 2) {
            if ((fptr = fopen("AL1", "w+")) != NULL)
            {
                fprintf(fptr, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", data, data, data, data, data);
                fclose(fptr);
            }
        }
        else if (opt == 3) {
            if ((fptr = fopen("DG1", "w+")) != NULL)
            {
                fprintf(fptr, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", data, data, data, data, data);
                fclose(fptr);
            }
        }

    }
}
void JSON(int opt) {
    FILE* fptr = NULL;
    int i = 0;
    char json[1000] = { "" };
    bool flag = false;
    if (opt == 1) {
        fptr = fopen("PID", "r");
    }
    else if (opt == 2) {
        fptr = fopen("AL1", "r");
    }
    else if (opt == 3) {
        fptr = fopen("DG1", "r");
    }
    if (fptr != NULL)
    {
        i = 0;
        while (!feof(fptr))
        {
            fscanf(fptr, "%c", &json[i]);
            i++;
        }
        json[i] = '\0';
        fclose(fptr);
    }
    else
    {
        printf("�d�L���ɮסA�Х���XML�ɦb�i������!\n");
    }
    printf("%s", json);
    if (opt == 1) {
        fptr = fopen("PID.json", "w+");
    }
    else if (opt == 2) {
        fptr = fopen("AL1.json", "w+");
    }
    else if (opt == 3) {
        fptr = fopen("DG1.json", "w+");
    }
    if (fptr != NULL)
    {
        i = 0;
        while (json[i] != '\0') {

            if (isdigit(json[i]) || isalpha(json[i]) || json[i] == '.') {
                fprintf(fptr, "%c", json[i]);
                i++;
            }
            else if (json[i] == '<' && json[i + 1] != '/') {
                fprintf(fptr, "\n{\n\"", json[i]);
                i++;
            }
            else if (json[i] == '>') {
                fprintf(fptr, "\":", json[i]);
                if (json[i + 1] != '<') {
                    fprintf(fptr, "\"", json[i]);
                    while (json[i + 1] != '<') {
                        i++;
                        fprintf(fptr, "%c", json[i]);
                        flag = true;
                    }
                    if (flag == true) {
                        fprintf(fptr, "\"", json[i]);
                    }
                }
                i++;
            }
            else if ((isdigit(json[i] || isalpha(json[i]))) && json[i + 1] == '<') {
                fprintf(fptr, "\"", json[i]);
            }
            else if (json[i] == '<' && json[i + 1] == '/') {
                while (json[i] != '>')
                {
                    i++;
                }
                fprintf(fptr, "\n}\n", json[i]);
                i++;
            }
            else {
                i++;
            }
        }
        fclose(fptr);
    }
}
void Fulljson(char al[])
{
    FILE* fptr;  //�ɮ׫���
    char json[5000] = { "" };
    int i;
    char* j;
    if ((fptr = fopen("fullAL1info.json", "r")) != NULL)
    {
        i = 0;
        while (!feof(fptr))
        {
            fscanf(fptr, "%c", &json[i]);
            i++;
        }
        json[i] = '\0';
        fclose(fptr);
    }
    else
    {
        printf("�d�L���ɮסA�Х���XML�ɦb�i������!\n");
    }
    printf("%s", json);
    if ((fptr = fopen("allergyintolerance-medication.json", "w+")) != NULL)
    {
        i = 0;
        j = strstr(json, "http://www.nlm.nih.gov/research/umls/rxnorm");
        printf("\n�O�����m: %p\n", j);
        j = strstr(j, "display");
        j += 11;
        while (&json[i] < j) {
            fprintf(fptr, "%c", json[i]);
            i++;
        }
  
        fclose(fptr);
    }
    if ((fptr = fopen("allergyintolerance-medication.json", "a+")) != NULL)
    {
        i = 0;
        j = strstr(json, "http://www.nlm.nih.gov/research/umls/rxnorm");
        printf("\n�O�����m: %p\n", j);
        j = strstr(j, "display");
        j += 11;
        fprintf(fptr, "%s", al);
        j += sizeof(al);
        while (*j != '"')
            j++;
        fprintf(fptr, "%s", j);

        fclose(fptr);
    }
}