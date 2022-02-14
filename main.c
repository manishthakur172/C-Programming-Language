#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFERLENGTH        54
#define TOTAL_LINE_LEN      43
#define ISR_DATA_LENTH      32
#define EMPTY_DATA_LEN      34

bool isr_address_found = false,merge_data = false;
bool first_address_found,second_address_found,third_address_found,fourth_address_found,fifth_address_found,sixth_address_found,seventh_address_found,new_address_found,eighth_address_found;
FILE *AppFP,*FinalFW_FP,*BootFP;
char empty_buffer[TOTAL_LINE_LEN];
char empty_char = 'F',count_isr_address;
int merge_line_len,remaining_line_len;
char constant_data[]={'S','2','1','4','0','0'};
char first_address[]={'F','F','8','0'};
char second_address[]={'F','F','9','0'};
char third_address[]={'F','F','A','0'};
char fourth_address[]={'F','F','B','0'};
char fifth_address[]={'F','F','C','0'};
char sixth_address[]={'F','F','D','0'};
char seventh_address[]={'F','F','E','0'};
char eighth_address[]={'F','F','F','0'};
char stopping_address[]={'S','2','1','4','0','2','2','0','0','0'};      //S214022000
char store_address[4],address_first_byte,isr_address_compare,address_second_byte;
char reset_vector_address[]={'0','0','F','0','7','D'};
char final_file_buffer[44];
char merge_error_occur;
int main(void)
{
    char i,j;
    char line[BUFFERLENGTH],ch,merge_line[TOTAL_LINE_LEN],append_line[BUFFERLENGTH],file_print_data[TOTAL_LINE_LEN],actual_data[TOTAL_LINE_LEN]={'\0'};
    AppFP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\WWL_Sensor_V02_App\\bin\\WWL_Sensor_V02_App.a43","r");
    //FinalFW_FP=fopen("WWL_Sensor_V02_ISR_Merged.a43","w+");
    FinalFW_FP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\OTA_Bin\\WWLSensor_V02.a43","w+");
    for(i=0; i<EMPTY_DATA_LEN; i++)
        empty_buffer[i] = 'F';
    if((AppFP==0) && (FinalFW_FP==0))
    {
        printf("App file not present.....\n");
        return -1;
    }
    while(fgets(line, BUFFERLENGTH, AppFP))
    {
        if(strstr(line,"0800FF80"))
        {
            fclose(AppFP);
            fclose(FinalFW_FP);
        }
        fprintf(FinalFW_FP,"%s",line);
    }
    memset((void *)line, NULL, BUFFERLENGTH);
    memset((void *)empty_buffer, NULL, TOTAL_LINE_LEN);
    memset((void *)file_print_data, NULL, TOTAL_LINE_LEN);
    AppFP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\WWL_Sensor_V02_App\\bin\\WWL_Sensor_V02_App.a43","r");
    //FinalFW_FP=fopen("WWL_Sensor_V02_ISR_Merged.a43","a");
    FinalFW_FP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\OTA_Bin\\WWLSensor_V02.a43","a");
    //Reading address from FF80-FF8F
    while(fgets(line, BUFFERLENGTH, AppFP)!= '\n')
    {
        if(strstr(line,"00FF80"))
        {
            isr_address_found = true;
            memcpy(&store_address,&line[6],4);
            for(i=0; i<TOTAL_LINE_LEN; i++)
            {
                empty_buffer[i] = 'F';
            }
            strncat(file_print_data,constant_data,6);
            strncat(file_print_data,first_address,4);
            //memcpy(merge_line,line,TOTAL_LINE_LEN);
            for(j=0,i=10; i<30; i++,j++)
                {
                    if(line[i] == '\n')
                        break;
                    merge_line[j] = line[i];
                }
                merge_line_len = strlen(merge_line);
                merge_line[merge_line_len-1] = 'F';
                merge_line[merge_line_len-2] = 'F';
                strcat(file_print_data,merge_line);
                merge_line_len = strlen(file_print_data);
                j=TOTAL_LINE_LEN-merge_line_len;
                strncat(file_print_data,empty_buffer,j);
                //memset((void *)line, NULL, BUFFERLENGTH);
                //memset((void *)merge_line, NULL, TOTAL_LINE_LEN);
                /*fprintf(FinalFW_FP,"%s",file_print_data);
                fprintf(FinalFW_FP,"%s","F\n");
                fclose(AppFP);
                fclose(FinalFW_FP);*/
            isr_address_found = true;
        }
        if(isr_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            isr_address_compare = strcmp(&store_address,&first_address);
            if(isr_address_compare == 0)
                continue;
            address_first_byte = store_address[2];
            address_second_byte = store_address[3];
            while(file_print_data[8] != store_address[2])
            {
                if(new_address_found == false)
                {
                    /*if(merge_error_occur < 1)
                    {
                        if(strstr(file_print_data,"S20600FFC8884169"))
                        {
                            merge_error_occur++;
                            break;
                        }
                    }*/
                    //printf("%s\n",file_print_data);
                    //memset(&actual_data,NULL,43);
                    strncpy(actual_data,file_print_data, 40);
                    printf("%c\n",actual_data[41]);
                    fprintf(FinalFW_FP,"%s",actual_data);
                    fprintf(FinalFW_FP,"%s","FFFF\n");
                    //fprintf(FinalFW_FP,"%s",file_print_data);
                    //fprintf(FinalFW_FP,"%s","F\n");
                }
                new_address_found = true;
                if((file_print_data[8] < second_address[2]))
                {
                    memset(&file_print_data,NULL,TOTAL_LINE_LEN);
                    memcpy(&file_print_data,constant_data,6);
                    memcpy(&file_print_data[6],second_address,4);
                    memcpy(&file_print_data[10],empty_buffer,34);
                    memcpy(&final_file_buffer,&file_print_data,44-1);
                    fprintf(FinalFW_FP,"%s",final_file_buffer);
                    fprintf(FinalFW_FP,"%s","F\n");
                }
                else if((file_print_data[8] < third_address[2]))
                {
                    memset(&file_print_data,NULL,TOTAL_LINE_LEN);
                    memcpy(&file_print_data,constant_data,6);
                    memcpy(&file_print_data[6],third_address,4);
                    memcpy(&file_print_data[10],empty_buffer,34);
                    memcpy(&final_file_buffer,&file_print_data,44-1);
                    fprintf(FinalFW_FP,"%s",final_file_buffer);
                    fprintf(FinalFW_FP,"%s","F\n");
                }
                else if((file_print_data[8] < fourth_address[2]))
                {
                    memset(&file_print_data,NULL,TOTAL_LINE_LEN);
                    memcpy(&file_print_data,constant_data,6);
                    memcpy(&file_print_data[6],fourth_address,4);
                    memcpy(&file_print_data[10],empty_buffer,34);
                    memcpy(&final_file_buffer,&file_print_data,44-1);
                    fprintf(FinalFW_FP,"%s",final_file_buffer);
                    fprintf(FinalFW_FP,"%s","F\n");
                }
                else
                {
                    fclose(AppFP);
                    fclose(FinalFW_FP);
                    merge_data = true;
                    fifth_address_found = true;
                    break;
                }
            }
            if(merge_data == true)
                break;
                switch (address_second_byte)
                {
                    case '0':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[10],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '1':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[12],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '2':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[14],merge_line,strlen(merge_line));
                        //fprintf(FinalFW_FP,"%s",file_print_data);
                        //fprintf(FinalFW_FP,"%s","F\n");
                        //fclose(AppFP);
                        //fclose(FinalFW_FP);
                        //merge_data = true;
                        break;
                    case '3':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[16],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '4':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[18],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '5':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[20],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '6':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[22],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '7':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[24],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '8':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[26],&merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '9':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[28],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'A':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[30],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'B':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[32],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'C':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[34],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'D':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[36],merge_line,strlen(merge_line));
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'E':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        memcpy(&file_print_data[38],merge_line,strlen(merge_line));
                        //strcat(file_print_data,merge_line);
                        // Two lines are for writing the file the updated file
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    default:
                        break;
                }
        }
        if(merge_data == true)
            break;
    }


    isr_address_found = false;
    new_address_found = false;
    memset((void *)line, NULL, BUFFERLENGTH);
    memset((void *)empty_buffer, NULL, TOTAL_LINE_LEN);
    memset((void *)file_print_data, NULL, TOTAL_LINE_LEN);
    memset((void *)merge_line,NULL,TOTAL_LINE_LEN);
    memset(&file_print_data,'F',TOTAL_LINE_LEN);

    AppFP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\WWL_Sensor_V02_App\\bin\\WWL_Sensor_V02_App.a43","r");
    //FinalFW_FP=fopen("WWL_Sensor_V02_ISR_Merged.a43","a");
    FinalFW_FP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\OTA_Bin\\WWLSensor_V02.a43","a");
    //Reading address from FF80-FF8F
    while(fgets(line, BUFFERLENGTH, AppFP)!= '\n')
    {
        if(strstr(line,"00FF80"))
        {
            isr_address_found = true;
            fifth_address_found = false;
        }
        if(isr_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            if(store_address[2] == fifth_address[2])
                fifth_address_found = true;
                else if(fifth_address[2] < store_address[2])
                {
                    fprintf(FinalFW_FP,"%s",final_file_buffer);
                    fprintf(FinalFW_FP,"%s","F\n");
                    fclose(AppFP);
                    fclose(FinalFW_FP);
                    merge_data = true;
                    new_address_found = true;
                    fifth_address_found = true;
                    break;
                }
                else
                    continue;
        }
        else
            continue;
        if(isr_address_found = true && fifth_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            if(store_address[2] == line[8])
            {
                memcpy(&file_print_data,constant_data,6);
                memcpy(&file_print_data[6],&fifth_address,4);
                address_first_byte = store_address[2];
                address_second_byte = store_address[3];
                switch (address_second_byte)
                {
                    case '0':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[10],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '1':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[12],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '2':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[14],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        //fprintf(FinalFW_FP,"%s",file_print_data);
                        //fprintf(FinalFW_FP,"%s","F\n");
                        //fclose(AppFP);
                        //fclose(FinalFW_FP);
                        //merge_data = true;
                        break;
                    case '3':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[16],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '4':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[18],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '5':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[20],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '6':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[22],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '7':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[24],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '8':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_data = false;
                        merge_line_len = strlen(merge_line);;
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[26],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",final_file_buffer);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '9':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[28],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'A':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[30],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'B':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[32],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'C':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[34],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'D':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[36],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'E':
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        memcpy(&file_print_data[38],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        break;
                    default:
                        break;
                }
            }
        }
        if(merge_data == true)
            break;
    }


    //fetching for FFD0-FFDF
    merge_data = false;
    sixth_address_found = false;
    isr_address_found = false;
    new_address_found = false;
    memset((void *)line, NULL, BUFFERLENGTH);
    memset((void *)empty_buffer, NULL, TOTAL_LINE_LEN);
    memset((void *)file_print_data, NULL, TOTAL_LINE_LEN);
    memset((void *)merge_line,NULL,TOTAL_LINE_LEN);
    memset(&file_print_data,'F',TOTAL_LINE_LEN);

    AppFP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\WWL_Sensor_V02_App\\bin\\WWL_Sensor_V02_App.a43","r");
    //FinalFW_FP=fopen("WWL_Sensor_V02_ISR_Merged.a43","a");
    FinalFW_FP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\OTA_Bin\\WWLSensor_V02.a43","a");

    while(fgets(line, BUFFERLENGTH, AppFP)!= '\n')
    {
        if(strstr(line,"00FF80"))
        {
            isr_address_found = true;
            sixth_address_found = false;
        }
        if(isr_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            if(store_address[2] == sixth_address[2])
                sixth_address_found = true;
                else if(sixth_address[2] < store_address[2])
                {
                    fprintf(FinalFW_FP,"%s",final_file_buffer);
                    fprintf(FinalFW_FP,"%s","F\n");
                    fclose(AppFP);
                    fclose(FinalFW_FP);
                    merge_data = true;
                    new_address_found = true;
                    sixth_address_found = true;
                    break;
                }
                else
                    continue;
        }
        else
            continue;
        if(isr_address_found = true && sixth_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            if(store_address[2] == line[8])
            {
                memcpy(&file_print_data,constant_data,6);
                memcpy(&file_print_data[6],&sixth_address,4);
                address_first_byte = store_address[2];
                address_second_byte = store_address[3];
                switch (address_second_byte)
                {
                    case '0':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[10],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '1':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[12],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '2':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[14],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        //fprintf(FinalFW_FP,"%s",file_print_data);
                        //fprintf(FinalFW_FP,"%s","F\n");
                        //fclose(AppFP);
                        //fclose(FinalFW_FP);
                        //merge_data = true;
                        break;
                    case '3':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[16],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '4':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[18],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '5':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[20],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '6':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[22],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '7':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[24],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '8':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_data = false;
                        merge_line_len = strlen(merge_line);;
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[26],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",final_file_buffer);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '9':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[28],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'A':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[30],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'B':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[32],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'C':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[34],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'D':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[36],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'E':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        memcpy(&file_print_data[38],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        break;
                    default:
                        break;
                }
            }
        }
        if(merge_data == true)
            break;
    }

    //fetching for FFE0-FFEF
    merge_data = false;
    seventh_address_found = false;
    isr_address_found = false;
    new_address_found = false;
    memset((void *)line, NULL, BUFFERLENGTH);
    memset((void *)empty_buffer, NULL, TOTAL_LINE_LEN);
    memset((void *)file_print_data, NULL, TOTAL_LINE_LEN);
    memset((void *)merge_line,NULL,TOTAL_LINE_LEN);
    memset(&file_print_data,'F',TOTAL_LINE_LEN);

    AppFP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\WWL_Sensor_V02_App\\bin\\WWL_Sensor_V02_App.a43","r");
    //FinalFW_FP=fopen("WWL_Sensor_V02_ISR_Merged.a43","a");
    FinalFW_FP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\OTA_Bin\\WWLSensor_V02.a43","a");

    while(fgets(line, BUFFERLENGTH, AppFP)!= '\n')
    {
        if(strstr(line,"00FF80"))
        {
            isr_address_found = true;
            seventh_address_found = false;
        }
        if(isr_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            if(store_address[2] == seventh_address[2])
                seventh_address_found = true;
                else if(seventh_address[2] < store_address[2])
                {
                    fprintf(FinalFW_FP,"%s",final_file_buffer);
                    fprintf(FinalFW_FP,"%s","F\n");
                    fclose(AppFP);
                    fclose(FinalFW_FP);
                    merge_data = true;
                    new_address_found = true;
                    seventh_address_found = true;
                    break;
                }
                else
                    continue;
        }
        else
            continue;
        if(isr_address_found = true && seventh_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            if(store_address[2] == line[8])
            {
                memcpy(&file_print_data,constant_data,6);
                memcpy(&file_print_data[6],&seventh_address,4);
                address_first_byte = store_address[2];
                address_second_byte = store_address[3];
                switch (address_second_byte)
                {
                    case '0':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[10],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '1':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[12],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '2':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[14],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        //fprintf(FinalFW_FP,"%s",file_print_data);
                        //fprintf(FinalFW_FP,"%s","F\n");
                        //fclose(AppFP);
                        //fclose(FinalFW_FP);
                        //merge_data = true;
                        break;
                    case '3':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[16],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '4':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[18],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '5':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[20],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '6':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[22],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '7':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[24],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '8':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_data = false;
                        merge_line_len = strlen(merge_line);;
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[26],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",final_file_buffer);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '9':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[28],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'A':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[30],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'B':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[32],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'C':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[34],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'D':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[36],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'E':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        memcpy(&file_print_data[38],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        break;
                    default:
                        break;
                }
            }
        }
        if(merge_data == true)
            break;
    }

    //fetching for FFF0-FFFF
    merge_data = false;
    eighth_address_found = false;
    isr_address_found = false;
    new_address_found = false;
    memset((void *)line, NULL, BUFFERLENGTH);
    memset((void *)empty_buffer, NULL, TOTAL_LINE_LEN);
    memset((void *)file_print_data, NULL, TOTAL_LINE_LEN);
    memset((void *)merge_line,NULL,TOTAL_LINE_LEN);
    memset(&file_print_data,'F',TOTAL_LINE_LEN);

    AppFP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\WWL_Sensor_V02_App\\bin\\WWL_Sensor_V02_App.a43","r");
    //FinalFW_FP=fopen("WWL_Sensor_V02_ISR_Merged.a43","a");
    FinalFW_FP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\OTA_Bin\\WWLSensor_V02.a43","a");

    while(fgets(line, BUFFERLENGTH, AppFP)!= '\n')
    {
        if(strstr(line,"00FF80"))
        {
            isr_address_found = true;
            eighth_address_found = false;
        }
        if(isr_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            if(store_address[2] == eighth_address[2])
                eighth_address_found = true;
                else if(eighth_address[2] < store_address[2])
                {
                    fprintf(FinalFW_FP,"%s",final_file_buffer);
                    fprintf(FinalFW_FP,"%s","F\n");
                    fclose(AppFP);
                    fclose(FinalFW_FP);
                    merge_data = true;
                    new_address_found = true;
                    eighth_address_found = true;
                    break;
                }
                else
                    continue;
        }
        else
            continue;
        if(isr_address_found = true && eighth_address_found == true)
        {
            memcpy(&store_address,&line[6],4);
            if(store_address[2] == line[8])
            {
                memcpy(&file_print_data,constant_data,6);
                memcpy(&file_print_data[6],&eighth_address,4);
                address_first_byte = store_address[2];
                address_second_byte = store_address[3];
                switch (address_second_byte)
                {
                    case '0':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[10],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '1':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[12],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '2':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[14],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        //fprintf(FinalFW_FP,"%s",file_print_data);
                        //fprintf(FinalFW_FP,"%s","F\n");
                        //fclose(AppFP);
                        //fclose(FinalFW_FP);
                        //merge_data = true;
                        break;
                    case '3':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[16],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '4':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[18],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '5':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[20],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '6':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[22],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '7':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[24],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '8':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_data = false;
                        merge_line_len = strlen(merge_line);;
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[26],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",final_file_buffer);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case '9':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[28],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'A':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[30],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'B':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[32],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'C':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[34],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'D':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //strcat(file_print_data,merge_line);
                        memcpy(&file_print_data[36],&merge_line,strlen(merge_line));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        /*fprintf(FinalFW_FP,"%s",file_print_data);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;*/
                        break;
                    case 'E':
                        strncpy(merge_line,empty_buffer,TOTAL_LINE_LEN);
                        for(j=0,i=10; i<30; i++,j++)
                        {
                            if(line[i] == '\n')
                                break;
                            merge_line[j] = line[i];
                        }
                        merge_line_len = strlen(merge_line);
                        merge_line[merge_line_len-1] = 'F';
                        merge_line[merge_line_len-2] = 'F';
                        //memcpy(&file_print_data[38],&merge_line,strlen(merge_line));
                        memcpy(&file_print_data[38],&reset_vector_address,strlen(reset_vector_address));
                        memcpy(&final_file_buffer,&file_print_data,44-1);
                        fprintf(FinalFW_FP,"%s",final_file_buffer);
                        fprintf(FinalFW_FP,"%s","F\n");
                        fclose(AppFP);
                        fclose(FinalFW_FP);
                        merge_data = true;
                        new_address_found = true;
                        eighth_address_found = true;
                        break;
                    default:
                        break;
                }
            }
        }
        if(merge_data == true)
            break;
    }
    //Copying rest of the data
    isr_address_found = false;
    memset((void *)line, NULL, BUFFERLENGTH);
    memset((void *)merge_line, NULL, TOTAL_LINE_LEN);
    memset((void *)file_print_data, NULL, TOTAL_LINE_LEN);

    AppFP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\WWL_Sensor_V02_App\\bin\\WWL_Sensor_V02_App.a43","r");
    //FinalFW_FP=fopen("WWL_Sensor_V02_ISR_Merged.a43","a");
    FinalFW_FP=fopen("..\\..\\..\\..\\Implementation\\WWL_Sensor_V02_OTA\\OTA_Bin\\WWLSensor_V02.a43","a");
    while(fgets(line, BUFFERLENGTH, AppFP))
    {
        if(strstr(line,stopping_address))
        {
            //fprintf(FinalFW_FP,"%s",line);
            isr_address_found = true;
        }
        if(isr_address_found == true)
        {
            fprintf(FinalFW_FP,"%s",line);
            isr_address_found = true;
        }
    }
    fclose(AppFP);
    fclose(FinalFW_FP);
    isr_address_found = false;
    return 0;
}
