#include<stdio.h>
#include<string.h>
#include <dirent.h>
#include <errno.h>

char tmp[1000],tmp1[1000],tmp2[1000],address[1000],null[1000],word[10000];
char line_string[100];
char character_string[100];
long long line;
long long character;

/*
    tmp1 is main command
    tmp2 is what we need
*/

int command_counter=0;
int* pointer_command=&command_counter;

int file_exists(const char*);
int directory_exists(char*);
int findAddress(char*,int);
void findPos(char*);
void findWord(char*);
long long string_to_ll(char*);
int exist(char*);

int file_exists(const char *fname){
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int directory_exists(char* dname){
    DIR* dir = opendir(dname);
    if (dir) {
        closedir(dir);
        return 1;
    } else if (ENOENT == errno) {
        return 0;
    }
}

int findAddress(char* command,int findmode){
    if(findmode){
        int i=0;
        int check_if=0;
        strcpy(address,".");
        while(*(command+i)!='\0'){
            if(check_if==2){
                if(*(command+i)=='"') break;
                strncat(address,command+i,1);
            }else if(check_if==1){
                if(*(command+i)==' ') break;
                strncat(address,command+i,1);
            }else if(*(command+i)=='/'){
                check_if=1;
                strncat(address,command+i,1);
            }else if(*(command+i)=='"') check_if=2;
            i++;
        }
        *pointer_command+=(i+1);
        if(*(command+i)=='\0') *pointer_command=0;
        if(address[1]!='/' || address[2]!='r' || address[3]!='o' || address[4]!='o' || address[5]!='t' || address[6]!='/') return 0;
        return 1;
    }else{
        int i=0;
        int check_if=0;
        strcpy(address,".");
        char whole_line[10000],just_a_word[10000];
        strcpy(whole_line,command);
        strcpy(just_a_word,".");
        while((strcmp(just_a_word,"--file"))){
            sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
        }
        if(whole_line[0]=='"'){
            i=1;
            while(whole_line[i]!='"' && whole_line[i]!='\0'){
                strncat(address,whole_line+i,1);
                i++;
            }
        }else if(whole_line[0]=='/'){
            while(whole_line[i]!=' ' && whole_line[i]!= '\n' && whole_line[i]!='\0'){
                strncat(address,whole_line+i,1);
                i++;
            }
        }
    }
}

long long string_to_ll(char* s){
    long long ans=0;
    for(int i=0;i<strlen(s);i++){
        ans*=10;
        ans+=(s[i]-'0');
    }
    return ans;
}

void findPos(char* command){
    char s[10000],t[10000],tempo[10000],pos[10000];
    strcpy(line_string,null);
    strcpy(character_string,null);
    strcpy(t,".");
    strcpy(s,command);
    while(strcmp(t,"--pos")){
        sscanf(s,"%s %[^\n]%*c",t,s);
    }
    sscanf(s,"%s %[^\n]%*c",pos,tempo);
    int check=0;
    for(int i=0;i<strlen(pos);i++){
        if(pos[i]==':') { check=1; continue;}
        if(check==0) strncat(line_string,&pos[i],1);
        else{strncat(character_string,&pos[i],1);}
    }
    *(&character)=string_to_ll(character_string);
    *(&line)=string_to_ll(line_string);

}

void findWord(char* command){
    char s[10000],t[10000],tempo[10000],pos[10000];
    int end_of_word=0;
    strcpy(t,".");
    strcpy(word,null);
    strcpy(s,command);
    while(strcmp(t,"--str")){
        sscanf(s,"%s %[^\n]%*c",t,s);
    }
    sscanf(s,"%s %[^\n]%*c",pos,tempo);
    if(pos[0]!='"') strcpy(word,pos);
    else{
        for(int i=1;;i++){
            if(s[i]=='"' && s[i-1]!='\\'){
                end_of_word=i;
                break;
            }
        }
        strncat(word,s+1,end_of_word-1);
    }
}

int address_of_file(char* address){
    char address_file[10000];
    int i;
    for(i=strlen(address)-1;i>=0;i--){
        if(address[i]=='/') break;
    }
    strncat(address_file,address,i);
    if(directory_exists(address_file)) return 1;
    return 0;
}

int exist(char* command){
    if(!findAddress(command,1)) {
        printf("Invalid address\n");
        return 0;
    }else{
        if(!file_exists(address)){
            if(address_of_file(address)){
                printf("File does not exist\n");
                return 0;
            }else{
                printf("Invalid Address\n");
                return 0;
            }
        }
    }
    return 1;
}

int findWord_instring(char* kalame,char* word,int ba,int search[]){
    if(ba==1){
        if(!(strcmp(word,null))){for(int i=0;i<strlen(kalame);i++){search[i]=1;} return 0;}
        int check_if=0;
        int check_all=-1;
        for(int i=0;i<=strlen(kalame)-strlen(word);i++){
            check_if=1;
            for(int j=0;j<strlen(word);j++){
                if(kalame[i+j]!=word[j]) {check_if=0;}
            }
            search[i]=check_if;
            if(check_all==-1 && check_if) check_all=i;
        }
        return check_all;
    }else{
        if(!(strcmp(word,null))){for(int i=0;i<strlen(kalame);i++){search[i]=1;} return 0;}
        int check_if=0;
        int check_all=-1;
        for(int i=0;i<=strlen(kalame)-strlen(word);i++){
            check_if=1;
            for(int j=0;j<strlen(word);j++){
                if(kalame[i+j]!=word[j]) {check_if=0;}
            }
            search[i+strlen(word)-1]=check_if;
            if(check_all==-1 && check_if) check_all=i;
        }
        return check_all;
    }
}

void createfile(char* command){
    sscanf(tmp1,"%s %[^\n]%*c",tmp2,address);
    if(strcmp(tmp2,"--file")){
        printf("Invalid input\n");
        printf("\n%s",tmp1);
        return;
    }else{
        if(address[0]=='"'){
            if(address[1]=='/' && address[2]=='r' && address[3]=='o' && address[4]=='o' && address[5]=='t'){
                char checks[1000],tmp3[1000];
                int check;
                strcpy(checks,"./root");
                if(strlen(address)==6 || address[6]!='/' || address[strlen(address)-1]!='"') { printf("Invalid address\n"); return;}
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
                            if(file_exists(checks)) { printf("File already exists\n"); return;}
                            else{
                                FILE * file;
                                fopen(checks,"w");
                            }
                        }
                    }
                }
            }else {printf("Invalid address\n"); return;}
        }else{
            if(address[0]=='/' && address[1]=='r' && address[2]=='o' && address[3]=='o' && address[4]=='t'){
                char checks[1000],tmp3[1000];
                int check;
                strcpy(checks,"./root");
                if(strlen(address)==5 || address[5]!='/') { printf("Invalid address\n"); return;}
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
                            if(file_exists(checks)) { printf("File already exists\n"); return;}
                            else{
                                FILE * file;
                                fopen(checks,"w");
                            }
                        }
                    }
                }
            }else{
                printf("Invalid address\n");
                return;
            }
        }
    }
}

void insertstr(char* command){
    if(!findAddress(command,0)) {
        printf("Invalid address\n");
        return;
    }else{
        FILE* fp;
        findAddress(command,0);
        long long current_line=1;
        long long current_character=0;
        if(!file_exists(address)){
            if(address_of_file(address)){
                printf("File does not exist\n");
                return;
            }else{
                printf("Invalid Address\n");
                return;
            }
        }
        fp=fopen(address,"r");
        findPos(command);
        findWord(command);
        char s;
        FILE* help;
        help=fopen("./help.txt","w");
        while((s=fgetc(fp))!=EOF){
            fputc(s,help);
        }
        fclose(help);
        fclose(fp);
        help=fopen("./help.txt","r");
        fp=fopen(address,"w");
        int check_if_added=0;
        while((s=fgetc(help))!=EOF){
            if(current_line==line && current_character==character){
                check_if_added=1;
                for(int i=0;i<strlen(word);i++){
                    if(word[i]=='\\' && i<strlen(word)-1 && word[i+1]=='n'){
                        if(i==0 || word[i-1]!='\\') {
                            fputc('\n',fp);
                            i++;
                        }
                    }else if(word[i]=='\\' && i<strlen(word)-1 && word[i+1]=='"'){
                        fputc('"',fp);
                        i++;
                    }else{
                        fputc(word[i],fp);
                    }
                }
            }
            if(s=='\n') {current_line++; current_character=0;}
            else current_character++;
            fputc(s,fp);

        }
        if(!check_if_added) printf("Out of bounds\n");
        fclose(fp);
        fclose(help);
        remove("./help.txt");
    }
}

void cat(char* command){
    if(!exist(command)) return;
    FILE *fp;
    fp=fopen(address,"r+");
    char s;
    while((s=fgetc(fp))!=EOF){
    printf("%c",s);
    }
    fclose(fp);
}

void removestr(char* command){
    if(!exist(command)) return;
    findAddress(command,1);
    findPos(command);
    long long current_line=1;
    long long current_character=0;
    char s='\0';
    FILE *fp,*help,*helphelp;
    fp=fopen(address,"r+");
    help=fopen("./help.txt","w");
    fclose(help);
    helphelp=fopen("./helphelp.txt","w");
    fclose(helphelp);
    help=fopen("./help.txt","r+");
    helphelp=fopen("./helphelp.txt","r+");
    int check_if_removed=0;
    long long before_char=0;
    int check_if_exist=0;
    long long total_char=0;
    char whole_line[10000]; char just_a_word[10000];
    strcpy(whole_line,command); strcpy(just_a_word,".");
    while(strcmp(just_a_word,"-size")){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
    }
    char removesize[100];
    char removemode[10];
    sscanf(whole_line,"%s %s",removesize,removemode);
    while((s=fgetc(fp))!=EOF){
        total_char++;
        if(!check_if_exist){
            fputc(s,help);
            before_char=total_char;
        }
        char t;
        if(current_character==character && current_line==line) check_if_exist=1;
        if(total_char==before_char && check_if_exist==1){
            if(!strcmp(removemode,"-b")){
                fclose(help);
                help=fopen("./help.txt","r+");
                for(int i=0;i<before_char-string_to_ll(removesize);i++){
                    t=fgetc(help);
                    fputc(t,helphelp);
                }
            }else{
                fclose(help);
                help=fopen("./help.txt","r+");
                for(int i=0;i<before_char-1;i++){
                    t=fgetc(help);
                    fputc(t,helphelp);
                }
                for(int i=before_char;i<before_char+string_to_ll(removesize)-1;i++){
                    t=fgetc(fp);
                    if(t==EOF) break;
                }
            }
        }else if(check_if_exist==1){
            fputc(s,helphelp);
        }
        if(s=='\n'){ current_line++; current_character=0;}
        else current_character++;
        if(t==EOF) break;

    }
    if(check_if_exist==0) {printf("Out of bounds\n"); return;}
    fclose(fp);
    fclose(helphelp);
    helphelp=fopen("./helphelp.txt","r");
    fp=fopen(address,"w");
    while((s=fgetc(helphelp))!=EOF){
        fputc(s,fp);
    }
    fclose(helphelp);
    fclose(fp);
    fclose(help);
    remove("./help.txt");
    remove("./helphelp.txt");
}

void copystr(char* command){
    if(!exist(command)) return;
    findAddress(command,1);
    findPos(command);
    char whole_line[10000]; char just_a_word[10000];
    strcpy(whole_line,command); strcpy(just_a_word,".");
    while(strcmp(just_a_word,"-size")){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
    }
    char copysize[100];
    char copymode[10];
    sscanf(whole_line,"%s %s",copysize,copymode);
    long long copy_size=string_to_ll(copysize);
    FILE *fp,*clipboard,*help;
    fp=fopen(address,"r");
    clipboard=fopen("./clipboard.txt","w");
    long long current_line=1;
    long long current_character=0;
    long long counter=0;
    long long initial_counter=0;
    char s;
    int check_if_existed=0;
    help=fopen("./help.txt","w");
    fclose(help);
    help=fopen("./help.txt","r+");
    while((s=fgetc(fp))!=EOF){
        char t;
        if(!check_if_existed) {initial_counter++;fputc(s,help);}
        if(((current_line==line && current_character>=character) || current_line>line)){
            check_if_existed=1;
            if(counter<copy_size && (!strcmp(copymode,"-f"))){
                fputc(s,clipboard);
                counter++;
            }
        }
        if(s=='\n'){current_character=0;current_line++;}
        else current_character++;
    }
    fclose(help);
    help=fopen("./help.txt","r");
    if((!(strcmp(copymode,"-b")))){
        for(long long i=0;i<initial_counter;i++){
            s=fgetc(help);
            if(i>initial_counter-copy_size-1){
                fputc(s,clipboard);
            }
        }
    }
    if((counter<copy_size && (!strcmp(copymode,"-f"))) || check_if_existed==0 || (initial_counter<copy_size && (!strcmp(copymode,"-r")))) {printf("Out of bounds\n"); return;}
    fclose(fp);
    fclose(help);
    fclose(clipboard);
    remove("./help.txt");
}

void cutstr(char* command){
    copystr(command);
    removestr(command);
}

void pastestr(char* command){
    if(!(exist(command))) return;
    FILE *fp,*clipboard,*help;
    findAddress(command,1);
    findPos(command);
    fp=fopen(address,"r+");
    clipboard=fopen("./clipboard.txt","r+");
    help=fopen("./help.txt","w");
    long long current_line=1;
    long long current_character=0;
    char s,t;
    int check_if_exist=0;
    while((s=fgetc(fp))!=EOF){
        if(current_line==line && current_character==character){
            check_if_exist=1;
            while((t=fgetc(clipboard))!=EOF){
                fputc(t,help);
            }
        }
        putc(s,help);
        if(s=='\n'){current_character=0;current_line++;}
        else current_character++;
    }
    if(!check_if_exist){printf("Out of bounds\n"); return;}
    fclose(help);
    help=fopen("./help.txt","r");
    fclose(fp);
    fp=fopen(address,"w");
    while((s=fgetc(help))!=EOF){
        fputc(s,fp);
    }
    fclose(fp);
    fclose(clipboard);
    fclose(help);
    remove(help);
}

void findstr(char* command){
    findAddress(command,0);
    findWord(command);
    int check[4]={0,0,0,0};
    int i=0;
    char whole_line[10000],just_a_word[10000];
    strcpy(whole_line,command);
    strcpy(just_a_word,".");
    while(!(strcmp(whole_line,null))){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
        if(!(strcmp(just_a_word,"-at"))){
            check[0]=1;
            i++;
            break;
        }
        if(!(strcmp(just_a_word,"-byword"))){
            check[1]=1;
            i++;
            break;
        }
        if(!(strcmp(just_a_word,"-count"))){
            check[2]=1;
            i++;
            break;
        }
        if(!(strcmp(just_a_word,"-all"))){
            check[3]=1;
            i++;
            break;
        }
    }
    FILE *fp,*help;
    int check_if=0;
    help=fopen("./help.txt","w");
    int check_space=0;
    for(int i=0;i<strlen(word);i++){
        if(word[i]=='\\' && i<strlen(word)-1 && word[i+1]=='n'){
            if(i==0 || word[i-1]!='\\') {
                fputc('\n',help);
                i++;
            }
            check_space=1;
        }else if(word[i]=='\\' && i<strlen(word)-1 && word[i+1]=='"'){
            fputc('"',help);
            i++;
            check_space=1;
        }else if(word[i]=='\\' && i<strlen(word)-1 && word[i+1]=='*'){
            fputc('*',help);
            i++;
            check_space=1;
        }else if(word[i]=='*' && (i==0 || word[i-1]==' ')){
            check_if=1;
            check_space=1;
        }else if(word[i]=='*' && (i==strlen(word)-1 || word[i+1]==' ')){
            check_space=1;
        }else if(word[i]==' ' && check_space==1){
            if(word[i-1]=='*') fputc('f',help);
            else if(check_if==1) fputc('b',help);
            else fputc('n',help);
            fputc(' ',help);
            check_if=0;
        }else{
            if(word[i]!=' ') check_space=1;
            fputc(word[i],help);
        }
    }
    if(word[strlen(word)-1]=='*') fputc('f',help);
    else if(check_if==1) fputc('b',help);
    else fputc('n',help);
    fp=fopen(address,"r");
    fclose(help);
    int counter=0;
    help=fopen("./help.txt","r");
    char body[100000];
    char updated_word[100000];
    int j=0;
    char s;
    while((s=fgetc(fp))!=EOF){
        body[j]=s;
        j++;
    }
    j=0;
    while((s=fgetc(help))!=EOF){
        updated_word[j]=s;
        j++;
    }
    int space_counter=0;
    strcpy(just_a_word,null);
    char beforestar[10000];
    char afterstar[10000];
    char star[10000];
    check_if=0;
    char starmode='n';
    for(int j=0;j<strlen(updated_word);j++){
        if(j!=strlen(updated_word)-1 && updated_word[j+1]!=' ' && updated_word[j]!=' '){
            strncat(just_a_word,updated_word+j,1);
        }else if(updated_word[j]!=' '){
            if(updated_word[j]!='n') {strcpy(star,just_a_word); check_if=1; starmode=updated_word[j];}
            else if(check_if==0) strcat(beforestar,just_a_word);
            else strcat(afterstar,just_a_word);
        }else if(updated_word[j]==' '){
            if(check_if==0) strncat(beforestar,updated_word+j,1);
            else strncat(afterstar,updated_word+j,1);
            strcpy(just_a_word,null);
        }
    }
    if(starmode=='f'){
        strcat(beforestar,star);
    }else if(starmode=='b'){
        strcat(star,afterstar);
        strcpy(afterstar,star);
    }
    int search_before[10000]={0};
    int search_after[10000]={0};
    if(starmode!='n'){
        findWord_instring(body,beforestar,0,search_before);
        findWord_instring(body,afterstar,1,search_after);
    }
    int space[10000]={0};
    counter=1;
    space[0]=0;
    for(int i=0;i<strlen(body);i++){
        if(body[i]==' '){
            space[counter]=i;
            counter++;
        }
    }
    space[counter]=strlen(body)-1;
    int rcloseb[10000]={0};
    int lclosea[10000]={0};
    for(int i=0;i<counter;i++){
        int flag=0;
        int j=0;
        for(j=0;j<space[i+1]-space[i];j++){
            if(search_before[space[i]+j]==1) flag=1;
            if(flag) break;
        }
        if(flag) rcloseb[i]=space[i]+j;
        else rcloseb[i]=-1;
    }
    for(int i=1;i<=counter;i++){
        int flag=0;
        int j=0;
        for(j=0;j<space[i]-space[i-1];j++){
            if(search_after[space[i]-j]==1) flag=1;
            if(flag) break;
        }
        if(flag) lclosea[i]=space[i]-j;
        else lclosea[i]=-1;
    }
    /*for(int i=0;i<counter;i++){
        printf("%d.%d ",rcloseb[i],lclosea[i+1]);
    }*/
    for(int i=0;i<counter;i++){
        if(lclosea[i+1]>rcloseb[i] && lclosea[i+1]!=-1 && rcloseb[i]!=-1){
            printf("%s",beforestar);
            if((!strcmp(beforestar,null)) && i==0) printf("%c",body[0]);
            for(int p=1;p<(lclosea[i+1]-rcloseb[i]);p++){
                printf("%c",body[p+rcloseb[i]]);
            }
            if((!(strcmp(afterstar,null))) && i==counter-1) printf("%c",body[strlen(body)-1]);
            printf("%s",afterstar);
            printf("\n");
        }
    }
    fclose(help);
    fclose(fp);
}

/*void grep(char* command){
    findWord(command);
    char whole_line[10000],just_a_word[10000];
    strcpy(whole_line,command);
    strcpy(just_a_word,".");
    while(strcmp(just_a_word,"--files")){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
    }
    FILE *fp,*help;
    help=fopen("./help.txt","w");
    for(int i=0;i<strlen(word);i++){
        if(word[i]=='\\' && i<strlen(word)-1 && word[i+1]=='n'){
            if(i==0 || word[i-1]!='\\') {
                fputc('\n',help);
                i++;
            }
        }else if(word[i]=='\\' && i<strlen(word)-1 && word[i+1]=='"'){
            fputc('"',help);
            i++;
        }else{
            fputc(word[i],help);
        }
    }
    fclose(help);
}*/

int main(){
    char command[10000];
    while(1){
        pointer_command=0;
        scanf("%[^\n]%*c", command);
        sscanf(command,"%s %[^\n]%*c",tmp,tmp1);
        if(!(strcmp(tmp,"createfile"))){
            createfile(command);
        }
        else if(!(strcmp(tmp,"insertstr"))){
            insertstr(command);
        }else if(!strcmp(tmp,"cat")){
            cat(command);
        }else if(!strcmp(tmp,"removestr")){
            removestr(command);
        }else if(!strcmp(tmp,"copystr")){
            copystr(command);
        }else if(!strcmp(tmp,"cutstr")){
            cutstr(command);
        }else if(!strcmp(tmp,"pastestr")){
            pastestr(command);
        }else if(!strcmp(tmp,"findstr")){
            findstr(command);
        }else if(!(strcmp(tmp,"grep"))){
            //grep(command);
        }
    }
}
