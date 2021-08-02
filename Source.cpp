#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void PIDXML(char HL7[]);
void DG1XML(char HL7[]);
void Fulljson(char al[]);

int main()
{
    FILE* fptr;  //開HL7的檔案指標
    FILE* fptr2; //寫XML的檔案指標
    char* ptr;
    char HL7[500] = { "" };  //放HL7的陣列
    char id[11] = { "" };    //PID(Patient ID)
    char al[50] = { "" };    //AL1(Allergic)
    char diag[100] = { "" }; //DG1(Diagnosis)
    char json[1000] = { "" };
    char tf; //是否轉檔(True or False)
    int i = 0, opt = 0, flag = 0;
    /*程式開始*/

    if ((fptr = fopen("HL7.hl7", "r")) != NULL)
    {
        while (!feof(fptr)) //還沒到檔案結尾的話就進入讀檔
        {
            fscanf(fptr, "%c", &HL7[i]); //將檔案資料抓到陣列裡面
            i++;
        }
        printf("%s\n", HL7);
        fclose(fptr);
    }
    else
    {
        printf("開檔失敗!\n");
    }

    while (opt != -1)
    {
        i = 0;
        printf("\n1)病患PID碼2)過敏代碼3)診斷說明4)將AL1 json輸入完整FHIR檔案0)將XML檔案轉JSON格式-1)離開:");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            PIDXML(HL7);
            break;
        case 2:
            ptr = strstr(HL7, "AL1");
            if (ptr)
            {
                ptr += 7;
                for (i = 0; i < 10; i++)
                {
                    al[i] = *ptr;
                    ptr++;
                    if (ptr == NULL)
                    {
                        break;
                    }
                }
                al[i] = '\0';
                printf("%s", al);
            }
            printf("\n是否轉XML檔? y/n:");
            scanf(" %c", &tf);
            if (tf == 'y')
            {
                if ((fptr2 = fopen("AL1", "w+")) != NULL)
                {
                    fprintf(fptr2, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", al, al, al, al, al);
                    fclose(fptr2);
                }
            }
            Fulljson(al);
            break;
        case 3:
            DG1XML(HL7);
            break;

        case 0:
            printf("\n1)將病人資訊(PID)轉換成JSON\n2)將過敏資訊(AL1)轉換成JSON\n3)將診斷資訊(DG1)轉換成JSON\n0)回主選單\n選項:");
            scanf("%d", &opt);

            switch (opt)
            {
            case 1:
                if ((fptr2 = fopen("PID", "r")) != NULL)
                {
                    i = 0;
                    while (!feof(fptr2))
                    {
                        fscanf(fptr2, "%c", &json[i]);
                        i++;
                    }
                    json[i] = '\0';
                    fclose(fptr2);
                }
                else
                {
                    printf("查無此檔案，請先建XML檔在進來轉檔!\n");
                }
                printf("%s", json);
                if ((fptr2 = fopen("PID.json", "w+")) != NULL)
                {
                    i = 0;
                    while (json[i] != '\0') {

                        if (isdigit(json[i]) || isalpha(json[i]) || json[i] == '.') {
                            fprintf(fptr2, "%c", json[i]);
                            i++;
                        }
                        else if (json[i] == '<' && json[i + 1] != '/') {
                            fprintf(fptr2, "\n{\n\"", json[i]);
                            i++;
                        }
                        else if (json[i] == '>') {
                            fprintf(fptr2, "\":", json[i]);
                            if (json[i+1]!='<') {
                                 fprintf(fptr2, "\"", json[i]);
                                 while (json[i+1] != '<') {
                                     i++;
                                     fprintf(fptr2, "%c", json[i]);
                                     flag = 1;
                                 }
                                 if (flag == 1) {
                                     fprintf(fptr2, "\"", json[i]);
                                 }
                            }                          
                            i++;
                        }
                        else if ((isdigit(json[i] || isalpha(json[i]))) && json[i + 1]=='<') {
                            fprintf(fptr2, "\"", json[i]);
                        }
                        else if (json[i] == '<' && json[i + 1] == '/') {
                            while (json[i]!='>')
                            {
                                i++;
                            }
                            fprintf(fptr2, "\n}\n", json[i]);
                            i++;
                        }
                        else {
                            i++;
                        }
                    }
                    fclose(fptr2);
                }
                break;
            case 2:
                if ((fptr2 = fopen("AL1", "r")) != NULL)
                {
                    i = 0;
                    while (!feof(fptr2))
                    {
                        fscanf(fptr2, "%c", &json[i]);
                        i++;
                    }
                    json[i] = '\0';
                    fclose(fptr2);
                }
                else
                {
                    printf("查無此檔案，請先建XML檔在進來轉檔!\n");
                }
                printf("%s", json);
                if ((fptr2 = fopen("AL1.json", "w+")) != NULL)
                {
                    i = 0;
                    while (json[i] != '\0') {

                        if (isdigit(json[i]) || isalpha(json[i]) || json[i] == '.') {
                            fprintf(fptr2, "%c", json[i]);
                            i++;
                        }
                        else if (json[i] == '<' && json[i + 1] != '/') {
                            fprintf(fptr2, "\n{\n\"", json[i]);
                            i++;
                        }
                        else if (json[i] == '>') {
                            fprintf(fptr2, "\":", json[i]);
                            if (json[i+1]!='<') {
                                 fprintf(fptr2, "\"", json[i]);
                                 while (json[i+1] != '<') {
                                     i++;
                                     fprintf(fptr2, "%c", json[i]);
                                     flag = 1;
                                 }
                                 if (flag == 1) {
                                     fprintf(fptr2, "\"", json[i]);
                                 }
                            }                          
                            i++;
                        }
                        else if ((isdigit(json[i] || isalpha(json[i]))) && json[i + 1]=='<') {
                            fprintf(fptr2, "\"", json[i]);
                        }
                        else if (json[i] == '<' && json[i + 1] == '/') {
                            while (json[i]!='>')
                            {
                                i++;
                            }
                            fprintf(fptr2, "\n}\n", json[i]);
                            i++;
                        }
                        else {
                            i++;
                        }
                    }
                    fclose(fptr2);
                }
                Fulljson(al);
                break;
            case 3:
                if ((fptr2 = fopen("DG1", "r")) != NULL)
                {
                    i = 0;
                    while (!feof(fptr2))
                    {
                        fscanf(fptr2, "%c", &json[i]);
                        i++;
                    }
                    json[i] = '\0';
                    fclose(fptr2);
                }
                else
                {
                    printf("查無此檔案，請先建XML檔在進來轉檔!\n");
                }
                printf("%s", json);
                if ((fptr2 = fopen("DG1.json", "w+")) != NULL)
                {
                    i = 0;
                    while (json[i] != '\0') {

                        if (isdigit(json[i]) || isalpha(json[i]) || json[i] == '.') {
                            fprintf(fptr2, "%c", json[i]);
                            i++;
                        }
                        else if (json[i] == '<' && json[i + 1] != '/') {
                            fprintf(fptr2, "\n{\n\"", json[i]);
                            i++;
                        }
                        else if (json[i] == '>') {
                            fprintf(fptr2, "\":", json[i]);
                            if (json[i + 1] != '<') {
                                fprintf(fptr2, "\"", json[i]);
                                while (json[i + 1] != '<') {
                                    i++;
                                    fprintf(fptr2, "%c", json[i]);
                                    flag = 1;
                                }
                                if (flag == 1) {
                                    fprintf(fptr2, "\"", json[i]);
                                }
                            }
                            i++;
                        }
                        else if ((isdigit(json[i] || isalpha(json[i]))) && json[i + 1] == '<') {
                            fprintf(fptr2, "\"", json[i]);
                        }
                        else if (json[i] == '<' && json[i + 1] == '/') {
                            while (json[i] != '>')
                            {
                                i++;
                            }
                            fprintf(fptr2, "\n}\n", json[i]);
                            i++;
                        }
                        else {
                            i++;
                        }
                    }
                    fclose(fptr2);
                }
                break;
            case 0:
                break;
            default:
                printf("輸入錯誤請重新輸入!\n");
                break;
            }
            break;
        case -1:
            break;
        default:
            printf("輸入錯誤請重新輸入!\n");
            break;
        }
    }

    return 0;
}
void PIDXML(char HL7[]) {
    FILE* fptr;  //開HL7的檔案指標
    FILE* fptr2; //寫XML的檔案指標
    char* ptr;
    char id[11] = { "" };    //PID(Patient ID)
    int i = 0;
    char tf; //是否轉檔(True or False)
    ptr = strstr(HL7, "PID");
    if (ptr)
    {
        ptr += 4;
        for (i = 0; i < 10; i++)
        {
            id[i] = *ptr;
            ptr++;
        }
        id[10] = '\0';
        printf("%s", id);
    }
    printf("\n是否轉XML檔? y/n:");
    scanf(" %c", &tf);
    if (tf == 'y')
    {
        if ((fptr2 = fopen("PID", "w+")) != NULL)
        {
            fprintf(fptr2, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", id, id, id, id, id);
            fclose(fptr2);
        }
    }
}
void DG1XML(char HL7[]) 
{
    FILE* fptr2; //寫XML的檔案指標
    char* ptr;
    char diag[100] = { "" }; //DG1(Diagnosis)
    char tf; //是否轉檔(True or False)
    int i = 0;
    ptr = strstr(HL7, "DG1");
    if (ptr)
    {
        ptr += 13;
        for (i = 0; i < 20; i++)
        {
            diag[i] = *ptr;
            ptr++;
            if (ptr == NULL)
            {
                break;
            }
        }
        diag[i] = '\0';
        printf("%s", diag);
    }
    printf("\n是否轉XML檔? y/n:");
    scanf(" %c", &tf);
    if (tf == 'y')
    {
        if ((fptr2 = fopen("DG1", "w+")) != NULL)
        {
            fprintf(fptr2, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", diag, diag, diag, diag, diag);
            fclose(fptr2);
        }
    }
}
void Fulljson(char al[])
{
    FILE* fptr;  //檔案指標
    char json[5000] = { "" };
    //char upper[2000] = { "" };
    char lower[20] = { "" };
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
        printf("查無此檔案，請先建XML檔在進來轉檔!\n");
    }
    printf("%s", json);
    if ((fptr = fopen("allergy.json", "w+")) != NULL)
    {
        /*先把目的的名字換好*/
        i = 0;
        j = strstr(json, "http://www.nlm.nih.gov/research/umls/rxnorm");
        printf("\n記憶體位置: %p\n", j);
        j = strstr(j, "display");
        j += 11;
        printf("%s", j);
        while (&json[i] < j) {
            fprintf(fptr, "%c", json[i]);
            i++;
        }
  
        /*再把上半部補齊*/

        fclose(fptr);
    }
    if ((fptr = fopen("allergy.json", "a+")) != NULL)
    {
        /*先把目的的名字換好*/
        i = 0;
        j = strstr(json, "http://www.nlm.nih.gov/research/umls/rxnorm");
        printf("\n記憶體位置: %p\n", j);
        j = strstr(j, "display");
        j += 11;
        printf("%s", j);
        fprintf(fptr, "%s", al);
        j += sizeof(al);
        while (*j != '"')
            j++;
        fprintf(fptr, "%s", j);
        /*再把上半部補齊*/

        fclose(fptr);
    }
}