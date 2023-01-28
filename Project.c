#include<stdio.h>
#include<string.h>
#include <dirent.h>
#include <errno.h>

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
int direxists(char* dname){
    DIR* dir = opendir(dname);
    if (dir) {
        closedir(dir);
        return 1;
    } else if (ENOENT == errno) {
        return 0;
    }
}

int main(){
    char command[10000];
    while(1){
        char tmp[1000],tmp1[1000],tmp2[1000],address[1000];
        scanf("%[^\n]%*c", command);
        sscanf(command,"%s %[^\n]%*c",tmp,tmp1);
        if(!(strcmp(tmp,"createfile"))){
            sscanf(tmp1,"%s %[^\n]%*c",tmp2,address);
            if(strcmp(tmp2,"--file")){
                printf("Invalid input\n");
                continue;
            }else{
                if(address[0]=='"'){
                    if(address[1]=='/' && address[2]=='r' && address[3]=='o' && address[4]=='o' && address[5]=='t'){
                        char checks[1000],tmp3[1000];
                        int check;
                        strcpy(checks,"./root");
                        if(strlen(address)==6 || address[6]!='/' || address[strlen(address)-1]!='"') { printf("Invalid address\n"); continue;}
                        strcpy(tmp3,"/");
                        for(int i=7;i<strlen(address)-1;i++){
                            if(address[i]=='/'){
                                strcat(checks,tmp3);
                                mkdir(checks);
                                strcpy(tmp3,"/");
                            }else{
                                strncat(tmp3,&address[i], 1);
                                if(i+2==strlen(address)){
                                    strcat(checks,tmp3);
                                    if(exists(checks)) { printf("File already exists\n"); continue;}
                                    else{
                                        FILE * file;
                                        fopen(checks,"w");
                                    }
                                }
                            }
                        }
                    }else {printf("Invalid address\n"); continue;}
                }else{
                    if(address[0]=='/' && address[1]=='r' && address[2]=='o' && address[3]=='o' && address[4]=='t'){
                        char checks[1000],tmp3[1000];
                        int check;
                        strcpy(checks,"./root");
                        if(strlen(address)==5 || address[5]!='/') { printf("Invalid address\n"); continue;}
                        strcpy(tmp3,"/");
                        for(int i=6;i<strlen(address);i++){
                            if(address[i]=='/'){
                                strcat(checks,tmp3);
                                mkdir(checks);
                                strcpy(tmp3,"/");
                            }else{
                                strncat(tmp3,&address[i], 1);
                                if(i+1==strlen(address)){
                                    strcat(checks,tmp3);
                                    if(exists(checks)) { printf("File already exists\n"); continue;}
                                    else{
                                        FILE * file;
                                        fopen(checks,"w");
                                    }
                                }
                            }
                        }
                    }else{
                        printf("Invalid address\n");
                        continue;
                    }
                }
            }
        }
        else if(!(strcmp(tmp,"insertstr"))){
            sscanf(tmp1,"%s %[^\n]%*c",tmp,tmp1);
            if(strcmp(tmp,"--file")){
                printf("Invalid input\n");
                continue;
            }
            if(tmp1[0]=='"'){
                int i=1;
                strcpy(tmp2,"");
                while(tmp1[i]!='"'){
                    tmp2[i-1]=tmp1[i];
                    i++;
                }
                if(tmp2[0]=='/' && tmp2[1]=='r' && tmp2[2]=='o' && tmp2[3]=='o' && tmp2[4]=='t' && tmp2[5]=='/'){
                    for(int j=strlen(tmp2)-1;j>=0;j--){
                        if(tmp2[j]=='/') {i=j; break;}
                    }
                    char tmpp[1000];
                    strncpy(tmpp,tmp2,i);
                    strcpy(tmp,".");
                    strcat(tmp,tmpp);
                    if(!direxists(tmp)) {printf("Invalid address\n"); continue;}
                    else{
                       strcpy(tmp,".");
                       strcat(tmp,tmp2);
                       if(!exists(tmp)) {printf("This file doesn't exist\n"); continue;}
                       else{

                       }
                    }

                }else{
                    printf("Invalid address\n");
                    continue;
                }
            }else if(tmp1[0]=='/'){
                int i=1;
                strcpy(tmp2,"");
                while(tmp1[i]!=' '){
                    tmp2[i]=tmp1[i];
                    i++;
                }
                tmp2[0]='/';
                if(tmp2[0]=='/' && tmp2[1]=='r' && tmp2[2]=='o' && tmp2[3]=='o' && tmp2[4]=='t' && tmp2[5]=='/'){
                    for(int j=strlen(tmp2)-1;j>=0;j--){
                        if(tmp2[j]=='/') {i=j; break;}
                    }
                    char tmpp[1000];
                    strncpy(tmpp,tmp2,i);
                    strcpy(tmp,".");
                    strcat(tmp,tmpp);
                    if(!direxists(tmp)) {printf("Invalid address\n"); continue;}
                    else{
                       strcpy(tmp,".");
                       strcat(tmp,tmp2);
                       if(!exists(tmp)) {printf("This file doesn't exist\n"); continue;}
                       else{

                       }
                    }

                }else{
                    printf("Invalid address\n");
                    continue;
                }
            }else {printf("Invalid address"); continue;}
        }
    }
}
