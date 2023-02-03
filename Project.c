#include<stdio.h>
#include<string.h>
#include <dirent.h>
#include <errno.h>

char tmp[1000],tmp1[1000],tmp2[1000],address[1000],null[1000]={'\0'},word[10000];
char line_string[100];
char character_string[100];
long long line;
long long character;
#define N 1000
/*
    tmp1 is main command
    tmp2 is what we need
*/

int command_counter=0;
int check_if_ended[2]={0};

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
        command_counter+=(i+1);
        if(*(command+i)=='\0') {command_counter=0; return 2;}
        if(address[1]!='/' || address[2]!='r' || address[3]!='o' || address[4]!='o' || address[5]!='t' || address[6]!='/') return 0;
        return 1;
    }else{
        int i=0;
        int check_if=0;
        strcpy(address,".");
        char whole_line[10000]={'\0'};char just_a_word[10000]={'\0'};
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
        if(address[1]!='/' || address[2]!='r' || address[3]!='o' || address[4]!='o' || address[5]!='t' || address[6]!='/') return 0;
        return 1;
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
    char s[10000]={'\0'};char t[10000]={'\0'};char tempo[10000]={'\0'};char pos[10000]={'\0'};
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
    char s[10000]={'\0'};char t[10000]={'\0'};char tempo[10000]={'\0'};char pos[10000]={'\0'};
    int end_of_word=0;
    strcpy(t,".");
    strcpy(word,null);
    strcpy(s,command);
    while(strcmp(t,"--str")){
        sscanf(s,"%s %[^\n]%*c",t,s);
    }
    if(s[0]!='"'){
        for(int i=0;;i++){
            if(s[i]==' ' || s[i]=='\0'){
                end_of_word=i;
                break;
            }
        }
        strncat(word,s,end_of_word);
    }
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
    char address_file[10000]={'\0'};
    int i=0;
    for(i=strlen(address)-1;i>=0;i--){
        if(address[i]=='/') break;
    }
    strncat(address_file,address,i);
    if(directory_exists(address_file)) return 1;
    return 0;
}

int exist(char* command){
    if(!findAddress(command,0)) {
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

int get_address(char* command,int index){
    check_if_ended[1]=0;
    int i=0;
    int check_if=0;
    strcpy(address,".");
    for(i=index;i<strlen(command);i++){
        if(check_if==2){
            if(command[i]=='"') break;
            strncat(address,command+i,1);
        }else if(check_if==1){
            if(command[i]==' ') break;
            strncat(address,command+i,1);
        }else if(command[i]=='/'){
            check_if=1;
            strncat(address,command+i,1);
        }else if(command[i]=='"') check_if=2;
    }
    if(check_if==2) i++;
    if(i==strlen(command)){
        check_if_ended[0]=0;
        check_if_ended[1]=1;
    }else{
        check_if_ended[0]=i+1;
    }
    if(address[1]!='/' || address[2]!='r' || address[3]!='o' || address[4]!='o' || address[5]!='t' || address[6]!='/') return 0;
    if(!file_exists(address)) return 0;
    return 1;
}

void createcopy(char* address){
    char fake_address[N];
    strcpy(fake_address,"./temp");
    strncat(fake_address,address+6,strlen(address)-6);

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
                char checks[1000]={'\0'};char tmp3[1000]={'\0'};
                int check=0;
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
                                file=fopen(checks,"w");
                                fclose(file);
                            }
                        }
                    }
                }
            }else {printf("Invalid address\n"); return;}
        }else{
            if(address[0]=='/' && address[1]=='r' && address[2]=='o' && address[3]=='o' && address[4]=='t'){
                char checks[1000]={'\0'};char tmp3[1000]={'\0'};
                int check=0;
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
                                file=fopen(checks,"w");
                                fclose(file);
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
        char s='\0';
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
    char s='\0';
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
    char whole_line[10000]={'\0'}; char just_a_word[10000]={'\0'};
    strcpy(whole_line,command); strcpy(just_a_word,".");
    while(strcmp(just_a_word,"-size")){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
    }
    char removesize[100]={'\0'};
    char removemode[10]={'\0'};
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
    if(check_if_exist==0) {printf("Out of bounds\n");fclose(help);fclose(helphelp);fclose(fp); return;}
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
    char whole_line[10000]={'\0'}; char just_a_word[10000]={'\0'};
    strcpy(whole_line,command); strcpy(just_a_word,".");
    while(strcmp(just_a_word,"-size")){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
    }
    char copysize[100]={'\0'};
    char copymode[10]={'\0'};
    sscanf(whole_line,"%s %s",copysize,copymode);
    long long copy_size=string_to_ll(copysize);
    FILE *fp,*clipboard,*help;
    fp=fopen(address,"r");
    clipboard=fopen("./clipboard.txt","w");
    long long current_line=1;
    long long current_character=0;
    long long counter=0;
    long long initial_counter=0;
    char s='\0';
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
    char s='\0';
    char t='\0';
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
    if(!check_if_exist){printf("Out of bounds\n"); fclose(clipboard);fclose(fp);return;}
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
}

void findstr(char* command ,char* word){
    if(!exist(command)) return;
    findAddress(command,0);
    int check[4]={0,0,0,0};
    int i=0;
    char whole_line[N]={'\0'};
    char just_a_word[N]={'\0'};
    strcpy(whole_line,command);
    strcpy(just_a_word,".");
    long long at_mode=0;
    int chch=1;
    while((strcmp(whole_line,null)) && chch){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
        if(!(strcmp(just_a_word,whole_line))){
            chch=0;
        }
        if(!(strcmp(just_a_word,"-at"))){
            check[0]=1;
            i++;
            at_mode=string_to_ll(whole_line);
        }
        if(!(strcmp(just_a_word,"-byword"))){
            check[1]=1;
            i++;
        }
        if(!(strcmp(just_a_word,"-count"))){
            check[2]=1;
            i++;
        }
        if(!(strcmp(just_a_word,"-all"))){
            check[3]=1;
            i++;
        }
    }
    int sum=i;
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
    fclose(fp);
    fp=fopen(address,"r");
    fclose(help);
    int counter=0;
    help=fopen("./help.txt","r");
    char body[100000]={'\0'};
    char updated_word[100000]={'\0'};
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
    strcpy(whole_line,body);
    strcpy(body,null);
    strncat(body,whole_line,strlen(whole_line)-2);
    if(whole_line[strlen(whole_line)-2]!='\n' && whole_line[strlen(whole_line)-2]!=EOF) strncat(body,whole_line+strlen(whole_line)-2,1);
    if(whole_line[strlen(whole_line)-1]!='\n' && whole_line[strlen(whole_line)-1]!=EOF) strncat(body,whole_line+strlen(whole_line)-1,1);
    int space_counter=0;
    strcpy(just_a_word,null);
    char beforestar[N]={'\0'};
    char afterstar[N]={'\0'};
    char star[N]={'\0'};
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
    int head[N]={0};
    int tail[N]={0};
    int search_before[N]={0};
    int search_after[N]={0};
    if(starmode!='n'){
        findWord_instring(body,beforestar,0,search_before);
        findWord_instring(body,afterstar,1,search_after);
        int space[]={0};
        counter=1;
        space[0]=0;
        for(int i=0;i<strlen(body);i++){
            if(body[i]==' '){
                space[counter]=i;
                counter++;
            }
        }
        space[counter]=strlen(body)-1;
        int rcloseb[N]={0};
        int lclosea[N]={0};
        for(int i=0;i<counter;i++){
            int flag=0;
            int j=0;
            for(j=0;j<space[i+1]-space[i];j++){
                if(search_before[space[i]+j]==1) flag=1;
                if(flag) break;
            }
            if(flag==0 && i==counter-1 && search_before[strlen(body)-1]==1) {flag=1;}
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
            if(flag==0 && i==1 && search_after[0]==1){flag=1;}
            if(flag) lclosea[i]=space[i]-j;
            else lclosea[i]=-1;
        }
        for(int i=0;i<N;i++){
            head[i]=-1;
            tail[i]=-1;
        }
        for(int i=0;i<counter;i++){
            if(((lclosea[i+1]==strlen(body)-1 && rcloseb[i]==strlen(body)-1) || (lclosea[i+1]==0 && rcloseb[i]==0) || lclosea[i+1]>rcloseb[i]) && lclosea[i+1]!=-1 && rcloseb[i]!=-1){
                if((!strcmp(beforestar,null)) && i==0) head[i]=0;
                else head[i]=rcloseb[i]-strlen(beforestar)+1;
                if((!(strcmp(afterstar,null))) && i==counter-1) tail[i]=strlen(body)-1;
                else tail[i]=lclosea[i+1]+strlen(afterstar)-1;
            }
        }
    }else{
        findWord_instring(body,beforestar,0,search_before);
        for(int i=0;i<N;i++){
            head[i]=-1;
            tail[i]=-1;
        }
        counter=0;
        for(int i=0;i<strlen(body);i++){
            if(search_before[i]==1){
                head[counter]=i-strlen(beforestar)+1;
                tail[counter]=i;
                counter++;
            }
        }
    }
    int vajeh_counter=0;
    int vajeh[N]={'\0'};
    for(int i=0;i<strlen(body);i++){
        if((i==0 || body[i-1]==' ') && (body[i]!=' ' && body[i]!=EOF && body[i]!='\0')){
            vajeh_counter++;
        }
        vajeh[i]=vajeh_counter-1;
    }

    int find_counter=0;
    for(int i=0;i<strlen(body);i++){
        if(head[i]!=-1){
            find_counter++;
        }
    }
    if(sum>=3 || (sum==2 && (check[1]!=1 || check[2]==1))){
        printf("Invalid option");
    }else{
        int checker=0;
        int countt=0;
        if(sum==0){
            for(int i=0;i<counter;i++){
                if(head[i]!=-1){printf("%d\n",head[i]);checker=1;break;}
            }
            if(!checker) printf("-1\n");
        }else if(sum==1){
            if(check[0]==1){
                if(at_mode>find_counter) printf("-1\n");
                else{
                    for(int i=0;i<counter;i++){
                        if(head[i]!=-1){
                            countt++;
                        }
                        if(countt==at_mode) printf("%d\n",head[i]);
                    }
                }
            }else if(check[1]==1){
                for(int i=0;i<counter;i++){
                    if(head[i]!=-1){printf("%d\n",vajeh[head[i]]);checker=1;break;}
                }
                if(!checker) printf("-1\n");
            }else if(check[2]==1){
                if(find_counter==0) printf("-1\n");
                else printf("%d\n",find_counter);
            }else{
                for(int i=0;i<counter;i++){
                    if(head[i]!=-1 && checker==0){
                        printf("%d",head[i]);
                        checker=1;
                    }else if(head[i]!=-1){
                        printf(" ,%d",head[i]);
                    }
                }
                if(find_counter==0) printf("-1");
                printf("\n");
            }
        }else{
            if(check[3]==1){
                for(int i=0;i<counter;i++){
                    if(head[i]!=-1 && checker==0){
                        printf("%d",vajeh[head[i]]);
                        checker=1;
                    }else if(head[i]!=-1){
                        printf(" ,%d",vajeh[head[i]]);
                    }
                }
                if(find_counter==0) printf("-1");
                printf("\n");
            }else{
                if(at_mode>find_counter) printf("-1\n");
                else{
                    for(int i=0;i<counter;i++){
                        if(head[i]!=-1){
                            countt++;
                        }
                        if(countt==at_mode) printf("%d\n",vajeh[head[i]]);
                    }
                }
            }
        }
    }
    fclose(help);
    fclose(fp);
    free(just_a_word);
    free(whole_line);
    free(search_after);
    free(search_before);
    free(beforestar);
    free(afterstar);
    free(head);
    free(tail);
    free(vajeh);
}

void replacestr(char* command){
    if(exist(command)) return;
    char whole_line1[N]={'\0'}; char just_a_word1[N]={'\0'};
    char word2[N]={'\0'};
    char word1[N]={'\0'};
    strcpy(whole_line1,command);
    strcpy(just_a_word1,".");
    while((strcmp(just_a_word1,"--str1"))){
        sscanf(whole_line1,"%s %[^\n]%*c",just_a_word1,whole_line1);
    }
    int i=0;
    strcpy(word,null);
    if(whole_line1[0]=='"'){
        i=1;
        while(whole_line1[i]!='"'){
            strncat(word,whole_line1+i,1);
            i++;
        }
    }else{
        while(whole_line1[i]!=' ' && whole_line1[i]!='\0' && whole_line1[i]!='\n'){
            strncat(word,whole_line1+i,1);
            i++;
        }
    }
    strcpy(word1,word);
    strcpy(whole_line1,command);
    strcpy(just_a_word1,".");
    while((strcmp(just_a_word1,"--str2"))){
        sscanf(whole_line1,"%s %[^\n]%*c",just_a_word1,whole_line1);
    }
    i=0;
    if(whole_line1[0]=='"'){
        i=1;
        while(whole_line1[i]!='"'){
            strncat(word2,whole_line1+i,1);
            i++;
        }
    }else{
        while(whole_line1[i]!=' ' && whole_line1[i]!='\0' && whole_line1[i]!='\n'){
            strncat(word2,whole_line1+i,1);
            i++;
        }
    }
    findAddress(command,0);
    int check[4]={0,0,0,0};
    i=0;
    char whole_line[N]={'\0'};
    char just_a_word[N]={'\0'};
    strcpy(whole_line,command);
    strcpy(just_a_word,".");
    long long at_mode=0;
    int chch=1;
    while((strcmp(whole_line,null)) && chch){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
        if(!(strcmp(just_a_word,whole_line))){
            chch=0;
        }
        if(!(strcmp(just_a_word,"-at"))){
            check[0]=1;
            i++;
            at_mode=string_to_ll(whole_line);
        }
        if(!(strcmp(just_a_word,"-byword"))){
            check[1]=1;
            i++;
        }
        if(!(strcmp(just_a_word,"-count"))){
            check[2]=1;
            i++;
        }
        if(!(strcmp(just_a_word,"-all"))){
            check[3]=1;
            i++;
        }
    }
    int sum=i;
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
    fclose(fp);
    fp=fopen(address,"r");
    fclose(help);
    int counter=0;
    help=fopen("./help.txt","r");
    char body[N]={'\0'};
    char updated_word[N]={'\0'};
    int j=0;
    char s='\0';
    while((s=fgetc(fp))!=EOF){
        body[j]=s;
        j++;
    }
    j=0;
    while((s=fgetc(help))!=EOF){
        updated_word[j]=s;
        j++;
    }
    strcpy(whole_line,body);
    strcpy(body,null);
    strncat(body,whole_line,strlen(whole_line)-2);
    if(whole_line[strlen(whole_line)-2]!='\n' && whole_line[strlen(whole_line)-2]!=EOF) strncat(body,whole_line+strlen(whole_line)-2,1);
    if(whole_line[strlen(whole_line)-1]!='\n' && whole_line[strlen(whole_line)-1]!=EOF) strncat(body,whole_line+strlen(whole_line)-1,1);
    int space_counter=0;
    strcpy(just_a_word,null);
    char beforestar[N]={'\0'};
    char afterstar[N]={'\0'};
    char star[N]={'\0'};
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
    int head[N]={0};
    int tail[N]={0};
    int search_before[N]={0};
    int search_after[N]={0};
    if(starmode!='n'){
        findWord_instring(body,beforestar,0,search_before);
        findWord_instring(body,afterstar,1,search_after);
        int space[N]={0};
        counter=1;
        space[0]=0;
        for(int i=0;i<strlen(body);i++){
            if(body[i]==' '){
                space[counter]=i;
                counter++;
            }
        }
        space[counter]=strlen(body)-1;
        int rcloseb[N]={0};
        int lclosea[N]={0};
        for(int i=0;i<counter;i++){
            int flag=0;
            int j=0;
            for(j=0;j<space[i+1]-space[i];j++){
                if(search_before[space[i]+j]==1) flag=1;
                if(flag) break;
            }
            if(flag==0 && i==counter-1 && search_before[strlen(body)-1]==1) {flag=1;}
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
            if(flag==0 && i==1 && search_after[0]==1){flag=1;}
            if(flag) lclosea[i]=space[i]-j;
            else lclosea[i]=-1;
        }
        for(int i=0;i<N;i++){
            head[i]=-1;
            tail[i]=-1;
        }
        for(int i=0;i<counter;i++){
            if(((lclosea[i+1]==strlen(body)-1 && rcloseb[i]==strlen(body)-1) || (lclosea[i+1]==0 && rcloseb[i]==0) || lclosea[i+1]>rcloseb[i]) && lclosea[i+1]!=-1 && rcloseb[i]!=-1){
                if((!strcmp(beforestar,null)) && i==0) head[i]=0;
                else head[i]=rcloseb[i]-strlen(beforestar)+1;
                if((!(strcmp(afterstar,null))) && i==counter-1) tail[i]=strlen(body)-1;
                else tail[i]=lclosea[i+1]+strlen(afterstar)-1;
            }
        }
    }else{
        findWord_instring(body,beforestar,0,search_before);
        for(int i=0;i<N;i++){
            head[i]=-1;
            tail[i]=-1;
        }
        counter=0;
        for(int i=0;i<strlen(body);i++){
            if(search_before[i]==1){
                head[counter]=i-strlen(beforestar)+1;
                tail[counter]=i;
                counter++;
            }
        }
    }
    int vajeh_counter=0;
    int vajeh[N]={'\0'};
    for(int i=0;i<strlen(body);i++){
        if((i==0 || body[i-1]==' ') && (body[i]!=' ' && body[i]!=EOF && body[i]!='\0')){
            vajeh_counter++;
        }
        vajeh[i]=vajeh_counter-1;
    }

    int find_counter=0;
    for(int i=0;i<strlen(body);i++){
        if(head[i]!=-1){
            find_counter++;
        }
    }
    fclose(help);
    fclose(fp);
    int head_updated[N]={0};
    int tail_updated[N]={0};
    int exist_head[N]={0};
    int reverse_head[N]={0};
    int counter_updated=0;
    for(int i=0;i<counter;i++){
        if(head[i]!=-1){
            head_updated[counter_updated]=head[i];
            tail_updated[counter_updated]=tail[i];
            exist_head[head[i]]=1;
            counter_updated++;
            reverse_head[head[i]]=counter_updated-1;
        }
    }
    char javab[N]={'\0'};
    if(sum>=2){
        printf("Invalid option\n");
    }else{
        int checker=0;
        int countt=0;
        if(sum==0){
            if(counter_updated){
                for(int j=0;j<strlen(body);j++){
                    if(j==head_updated[0]){
                        strcat(javab,word2);
                        j=tail_updated[0];
                    }else{
                        strncat(javab,body+j,1);
                    }
                }
                checker=1;
            }
            if(!checker) printf("-1\n");
            else{
                printf("Successfully replaced\n");
                fp=fopen(address,"w");
                for(int i=0;i<strlen(javab);i++){
                    fputc(javab[i],fp);
                }
                fclose(fp);
            }

        }else if(sum==1){
            if(check[0]==1){
                if(at_mode<=counter_updated){
                    checker=1;
                    for(int j=0;j<strlen(body);j++){
                        if(j==head_updated[at_mode-1]){
                            strcat(javab,word2);
                            j=tail_updated[at_mode-1];
                        }else{
                            strncat(javab,body+j,1);
                        }
                    }
                }
                if(!checker) printf("-1\n");
                else{
                    printf("Successfully replaced\n");
                    fp=fopen(address,"w");
                    for(int i=0;i<strlen(javab);i++){
                        fputc(javab[i],fp);
                    }
                    fclose(fp);
                }
            }else if(check[1]==1){
                printf("Invalid option\n");
            }else if(check[2]==1){
                printf("Invalid option");
            }else{
                for(int i=0;i<strlen(body);i++){
                    if(exist_head[i]){
                        strcat(javab,word2);
                        i=tail_updated[reverse_head[i]];
                    }
                    else{
                        strncat(javab,body+i,1);
                    }
                }
                if(counter_updated==0) printf("-1\n");
                else{
                    printf("Successfully replaced\n");
                    fp=fopen(address,"w");
                    for(int i=0;i<strlen(javab);i++){
                        fputc(javab[i],fp);
                    }
                    fclose(fp);
                }
            }
        }
    }
    fclose(help);
    fclose(fp);
    free(just_a_word);
    free(whole_line);
    free(search_after);
    free(search_before);
    free(beforestar);
    free(afterstar);
    free(head);
    free(tail);
    free(vajeh);
    free(head_updated);
    free(tail_updated);
    free(exist_head);
    free(reverse_head);
}

void grepstr(char* command){
    findWord(command);
    char whole_line[N];
    char just_a_word[N];
    strcpy(whole_line,command);
    strcpy(just_a_word,".");
    int checkmode[10]={0};
    while(strcmp(just_a_word,"--files")){
        sscanf(whole_line,"%s %[^\n]%*c",just_a_word,whole_line);
        if(!strcmp(just_a_word,"-l")) checkmode[0]=1;
        if(!strcmp(just_a_word,"-c")) checkmode[1]=1;
    }
    int warning=0;
    int counter=0;
    FILE *fp;
    int t=1;
    int check_if=1;
    int check=0;
    char s='\0';
    char current_line[N]={'\0'};
    int file_counter=0;
    strcpy(command,whole_line);
    while(check_if_ended[1]==0){
        t=get_address(command,check_if_ended[0]);
        if(!t) warning=1;
        else{
            fp=fopen(address,"r");
            int check_file=0;
            while((s=fgetc(fp))!=EOF){
                if(s=='\n'){
                    check=0;
                    for(int i=0;i<=strlen(current_line)-strlen(word);i++){
                        check_if=1;
                        for(int j=0;j<strlen(word);j++){
                            if(current_line[i+j]!=word[j]) {check_if=0; break;}
                        }
                        if(check_if) check=1;
                    }
                    if(check){
                        if(checkmode[0]==1){
                            check_file=1;
                        }else if(checkmode[1]==1){
                            counter++;
                        }else{
                            printf("%s: %s\n",address,current_line);
                        }
                    }
                    strcpy(current_line,null);
                }else{
                    strncat(current_line,&s,1);
                }
            }
            if(checkmode[0]==1 && check_file==1) printf("%s\n",address);
            fclose(fp);
        }
        file_counter++;
    }
    if(checkmode[1]==1) printf("%d\n",counter);
}

void compare(char* command){
    FILE *fp1,*fp2;
    char addresses[N]={'\0'};
    char address1[N]={'\0'};
    char address2[N]={'\0'};
    char tempo[N]={'\0'};
    sscanf(command,"%s %[^\n]%*c",tempo,addresses);
    get_address(addresses,check_if_ended[0]);
    strcpy(address1,address);
    get_address(addresses,check_if_ended[0]);
    strcpy(address2,address);
    if(!file_exists(address1)){
        if(address_of_file(address1)) printf("Invalid address\n");
        else printf("File does not exist\n");
        return;
    }
    if(!file_exists(address2)){
        if(address_of_file(address2)) printf("Invalid address\n");
        else printf("File does not exist\n");
        return;
    }
    fp1=fopen(address1,"r");
    fp2=fopen(address2,"r");
    char s='\0';
    char t='\0';
    int current_line=1;
    char line1[N]={'\0'};
    char line2[N]={'\0'};
    int check1=1;
    int check2=1;
    int check_line=1;
    int total1=0;
    int total2=0;
    while((s=fgetc(fp1))!=EOF){
        if(s=='\n') total1++;
    }
    while((t=fgetc(fp2))!=EOF){
        if(t=='\n') total2++;
    }
    fclose(fp1);
    fclose(fp2);
    fp1=fopen(address1,"r");
    fp2=fopen(address2,"r");
    s='\0'; t='\0';
    while(current_line<=total1 || current_line<=total2){
        if(s!=EOF) {s=fgetc(fp1); strncat(line1,&s,1);}
        if(t!=EOF) {t=fgetc(fp2); strncat(line2,&t,1);}
        while(s!='\n' && s!=EOF){
            s=fgetc(fp1);
            if(s!='\n' && s!=EOF) strncat(line1,&s,1);
        }
        while(t!='\n' && t!=EOF){
            t=fgetc(fp2);
            if(t!='\n' && t!=EOF) strncat(line2,&t,1);
        }
        if(s!=EOF && t!=EOF){
            if((strcmp(line1,line2))){
                printf("========== #%d ==========\n%s\n%s\n",current_line,line1,line2);
            }
        }
        if(current_line==total1+1){
            printf(">>>>>>>> #%d - #%d >>>>>>>>\n%s",total1+1,total2,line2);
        }else if(current_line==total2+1){
            printf("<<<<<<<< #%d - #%d <<<<<<<<\n%s",total2+1,total1,line1);
        }else if(current_line>total1){
            printf("\n%s",line2);
        }else if(current_line>total2){
            printf("\n%s",line1);
        }
        strcpy(line1,null);
        strcpy(line2,null);
        current_line++;
    }
    fclose(fp1);
    fclose(fp2);
    free(line1);
    free(line2);
}

void closingpair(char* command){
    if(!(exist(command))) return;
    findAddress(command,1);
    FILE *fp,*help;
    fp=fopen(address,"r");
    help=fopen("./help.txt","w");
    char s='\0';
    char body[N]={'\0'};
    while((s=fgetc(fp))!=EOF){
        strncat(body,&s,1);
    }
    int state[N]={0};
    int copystate[N]={0};
    int matching[N]={0};
    int matching0[N]={0};
    int counter=0;
    for(int i=0;i<strlen(body);i++){
        if(body[i]=='{') {state[i]=-1; copystate[i]=-1; matching0[counter]=i; matching[counter]=-1; counter++;}
        else if(body[i]=='}') {state[i]=1; copystate[i]=1; matching0[counter]=i; matching[counter]=1; counter++;}
        else {state[i]=0; copystate[i]=0;}
    }
    int PairOF[N]={0};
    for(int i=0;i<counter;i++){
        PairOF[i]=-1;
    }
    int y=1;
    int check_iff=1;
    int akolad_counter=counter;
    while(akolad_counter!=0 && y==1 && check_iff==1){
        int check_if=-1;
        check_iff=-1;
        for(int i=0;i<counter;i++){
            if(matching[i]==1 && check_if==-1){
                y=0;
                check_if=1;
                break;
            }else if(matching[i]==-1){
                check_if=i;
            }else if(matching[i]==1){
                PairOF[i]=check_if;
                PairOF[check_if]=i;
                matching[i]=0;
                matching[check_if]=0;
                akolad_counter-=2;
                check_iff=1;
                break;
            }
        }
    }
    int RealPairOf[N]={0};
    for(int i=0;i<strlen(body);i++){
        RealPairOf[i]=-1;
    }
    for(int i=0;i<counter;i++){
        if(PairOF[i]!=-1){
            RealPairOf[matching0[i]]=matching0[PairOF[i]];
        }
    }
    int which_line[N]={0};
    int initial_line=1;
    for(int i=0;i<strlen(body);i++){
        which_line[i]=initial_line;
        if(body[i]=='\n') initial_line++;
    }
    int which_state[N]={0};
    int which_type[N]={0};
    for(int i=0;i<strlen(body);i++){
        if(RealPairOf[i]!=-1){
            if(RealPairOf[i]>i) which_type[i]=3;
            else which_type[i]=4;
        }
        else if(body[i]=='\n'){
            which_type[i]=1;
        }else if(body[i]==' ' || body[i]=='\t') which_type[i]=0;
        else which_type[i]=2;
    }
    int space[N]={0};
    int jj=0;
    while(which_type[jj]==0){
        space[jj]=1;
        jj++;
    }
    for(int i=0;i<strlen(body);i++){
        which_state[i]=0;
        if(RealPairOf[i]==-1) {
            which_state[i]=-1;
            if(which_type[i]==1){
                jj=1;
                while(which_type[i+jj]==0 && jj<strlen(body)){
                    space[i+jj]=1;
                    jj++;
                }
                jj=1;
                while(which_type[i-jj]==0 && i-jj>=0){
                    space[i-jj]=1;
                    jj++;
                }
            }
        }
        else if(which_type[i]==4){
            int j=1;
            while(i+j<strlen(body)){
                if(which_type[i+j]!=0) {break;}
                else {space[i+j]=1;}
                j++;
            }
            if(i+j==strlen(body) || which_type[i+j]==1 || which_type[i+j]==4){

            }else which_state[i]++;
            j=1;
            while(i-j>=0){
                if(which_type[i-j]!=0) {break;}
                else {space[i-j]=1;}
                j++;
            }
            if(which_type[i-j]==3 || which_type[i-j]==1){

            }else which_state[i]+=2;
        }else{
            int j=1;
            while(i+j<strlen(body)){
                if(which_type[i+j]!=0) {break;}
                else {space[i+j]=1;}
                j++;
            }
            if(which_type[i+j]==1){;}
            else if(which_type[i+j]==4){which_state[i]-=5;}
            else {which_state[i]++;}
            j=1;
            while(i-j>=0){
                if(which_type[i-j]!=0) {break;}
                else {space[i-j]=1;}
                j++;
            }
            if(i>=j && which_type[i-j]==2) which_state[i]+=2;
        }
    }
    int tab_counter=0;
    fclose(fp);
    fp=fopen(address,"w");
    for(int i=0;i<strlen(body);i++){
        if(which_type[i]==3) tab_counter++;
        if(which_type[i]==4) tab_counter--;
        if(which_state[i]==-1){
            if(space[i]==1) continue;
            fputc(body[i],fp);
            if(which_type[i]==1){
                for(int k=0;k<tab_counter;k++){
                    fputc('\t',fp);
                }
            }
        }else{
            if(which_state[i]>=2 && which_type[i]==4){
                fputc('\n',fp);
            }
            if(which_type[i]==4){
                for(int k=0;k<tab_counter;k++){
                    fputc('\t',fp);
                }
            }
            if(which_type[i]==3 && which_state[i]>=2){
                fputc(' ',fp);
            }
            fputc(body[i],fp);
            if(which_state[i]==1 || which_state[i]==3){
                fputc('\n',fp);
                for(int k=0;k<tab_counter;k++){
                    fputc('\t',fp);
                }
            }else if(which_state[i]==-3 || which_state[i]==-5) fputc('\n',fp);
        }
    }

    fclose(help);
    fclose(fp);
}

int main(){
    char command[10000];
    while(1){
        check_if_ended[0]=0;
        check_if_ended[1]=0;
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
            findWord(command);
            findstr(command,word);
        }else if(!(strcmp(tmp,"grep"))){
            grepstr(command);
        }else if(!(strcmp(tmp,"replacestr"))){
            replacestr(command);
        }else if(!(strcmp(tmp,"compare"))){
            compare(command);
        }else if(!(strcmp(tmp,"auto-indent"))){
            closingpair(command);
        }else if(!(strcmp(tmo,"tree"))){

        }
    }
}
