#include <ncurses.h>
#include <stdlib.h>
#include<stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#define N 1000

char null[N]={'\0'};
char mohtaviat[N]={'\0'};
FILE *fp;
char address[N]={'\0'};
char WhatIsOnScreen[30][200];
char temporary[N]={'\0'};

char tmp[1000],tmp1[1000],tmp2[1000],address[1000],word[10000];
char line_string[100];
char character_string[100];
long long line;
long long character;
int SizeOfLine[N];
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
    char fake_address[N]={'\0'};
    strcpy(fake_address,"./temp");
    strncat(fake_address,address+6,strlen(address)-6);
    char current_address[N]={'\0'};
    strcpy(current_address,fake_address);
    int i=0;
    char number[N]={'\0'};
    sprintf(number,"(%d)",i);
    strcat(current_address,number);
    while(file_exists(current_address)){
        i++;
        strcpy(current_address,fake_address);
        sprintf(number,"(%d)",i);
        strcat(current_address,number);
    }
    strcpy(current_address,null);
    for(int i=0;i<strlen(fake_address);i++){
        if(fake_address[i]=='/'){
            mkdir(current_address,0777);
        }
        strncat(current_address,fake_address+i,1);
    }
    FILE *fp,*fp1;
    sprintf(number,"(%d)",i);
    strcat(current_address,number);
    fp=fopen(current_address,"w");
    fp1=fopen(address,"r");
    char s='\0';
    while((s=fgetc(fp1))!=EOF){
        fputc(s,fp);
    }
    fclose(fp);
    fclose(fp1);
}

void createfile(char* command){
	sscanf(command,"%s %[^\n]%*c",command,tmp1);

    sscanf(tmp1,"%s %[^\n]%*c",tmp2,address);
    if(strcmp(tmp2,"--file")){
        printf("Invalid input\n");
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
                        //mkdir(checks,0777);
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
                        mkdir(checks,0777);
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
        createcopy(address);
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



void PrintToFile(WINDOW *MAIN){
	fp=fopen(address,"r");
	char s='\0';
	wclear(MAIN);
	wmove(MAIN,0,0);
	strcpy(mohtaviat,null);
	int cnt=0;
	while((s=fgetc(fp))!=EOF){
		wprintw(MAIN,"%c",s);
		mohtaviat[cnt]=s;
	}
	wrefresh(MAIN);
	fclose(fp);
}

void PrintMain(WINDOW *MAINBOX,int BeginLine,int BeginChar){
	int ThisLine=0;
	int ThisChar=0;
	int SizeOfMohtaviat=strlen(mohtaviat);
	wclear(MAINBOX);
	wrefresh(MAINBOX);
	for(int i=0;i<SizeOfMohtaviat;i++){
		if(ThisLine<BeginLine){
			if(mohtaviat[i]=='\n'){
				if(ThisLine<30 && ThisChar<200){
					WhatIsOnScreen[ThisLine][ThisChar]='\0';
				}
				ThisLine++;
				ThisChar=0;
			}else{
				if(ThisLine<30 && ThisChar<200){
					WhatIsOnScreen[ThisLine][ThisChar]=mohtaviat[i];
				}
				ThisChar++;
			}
		}else if(ThisLine<=(BeginLine+LINES-3)){
			if(mohtaviat[i]=='\n'){
				if(ThisLine<30 && ThisChar<200){
					WhatIsOnScreen[ThisLine][ThisChar]='\0';
				}
				ThisChar=0;
				ThisLine++;
				if(ThisLine<(BeginLine+LINES-3)){
					char JustNewLine='\n';
					wprintw(MAINBOX,"%c",JustNewLine);
				}
			}else if(ThisChar<(BeginChar+COLS-4) && ThisChar>=BeginChar){
				if(ThisLine<30 && ThisChar<200){
					WhatIsOnScreen[ThisLine][ThisChar]=mohtaviat[i];
				}
				char JustNewLine='\n';
				wprintw(MAINBOX,"%c",JustNewLine);
				wrefresh(MAINBOX);
				ThisChar++;
			}else{
				if(ThisLine<30 && ThisChar<200){
					WhatIsOnScreen[ThisLine][ThisChar]=mohtaviat[i];
				}
				ThisChar++;
			}
		}
	}
	for(int i=ThisLine+1;i<30;i++){
		WhatIsOnScreen[i][0]='\0';
	}
	wrefresh(MAINBOX);
}

void PrintSideBar(WINDOW *SIDEBAR,int BeginLine){
	wclear(SIDEBAR);
	for(int i=0;i<LINES-2;i++){
		mvwprintw(SIDEBAR,i,0,"%4d",i+BeginLine+1);
	}
	wrefresh(SIDEBAR);
}

void PrintMode(WINDOW *MODEBOX,char c){
	wclear(MODEBOX);
	if(c=='n'){
		mvwprintw(MODEBOX,0,1,"NORMAL");
	}else if(c=='v'){
		mvwprintw(MODEBOX,0,1,"VISUAL");
	}else if(c=='i'){
		mvwprintw(MODEBOX,0,1,"INSERT");
	}
	wrefresh(MODEBOX);
}

void PrintName(WINDOW *NAMEBOX,char *address,int save){
	wclear(NAMEBOX);
	int SIZEOFADDRESS=strlen(address);
	mvwprintw(NAMEBOX,0,1,"%s",address);
	if(save) mvwprintw(NAMEBOX,0,SIZEOFADDRESS+2,"+");
	wrefresh(NAMEBOX);
}

void highlight(WINDOW *BOX,int inity,int initx,int finy,int finx,int MODE){
	attron(COLOR_PAIR(MODE));
	int RealInitx=initx; int RealInity=inity; int RealFinx=finx; int RealFiny=finy;
	if(inity>finy || (inity=finy && initx>=finx)){
		RealFinx=initx; RealFiny=inity; RealInitx=finx; RealInity=finy;
	}
	for(int i=RealInity;i<=RealFiny;i++){
		int index=0;
		if(i==RealInity) index=RealInitx;
		for(int j=index;j<COLS;j++){
			if(WhatIsOnScreen[i][j]=='\0') break;
			mvwprintw(BOX,i,j,"%c",WhatIsOnScreen[i][j]);
		}
	}
	attroff(COLOR_PAIR(MODE));
}

void copyScr(WINDOW *MAINBOX,int inity,int initx,int finy,int finx){
	int RealInitx=initx; int RealInity=inity; int RealFinx=finx; int RealFiny=finy;
	if(inity>finy || (inity=finy && initx>=finx)){
		RealFinx=initx; RealFiny=inity; RealInitx=finx; RealInity=finy;
	}
	for(int i=RealInity;i<=RealFiny;i++){
		int index=0;
		if(i==RealInity) index=RealInitx;
		strcpy(temporary,null);
		char NEWLINE='\n';
		for(int j=index;j<COLS;j++){
			if(WhatIsOnScreen[i][j]=='\0'){
				strncat(temporary,&NEWLINE,1);
			}else if(j==COLS-1){
				strncat(temporary,&WhatIsOnScreen[i][j],1);
				strncat(temporary,&NEWLINE,1);
			}else strncat(temporary,&WhatIsOnScreen[i][j],1);
		}
	}
}

void RemoveScr(WINDOW *MAINBOX,int inity,int initx,int finy,int finx){
	char NewScr[LINES][COLS];
	int RealInitx=initx; int RealInity=inity; int RealFinx=finx; int RealFiny=finy;
	if(inity>finy || (inity=finy && initx>=finx)){
		RealFinx=initx; RealFiny=inity; RealInitx=finx; RealInity=finy;
	}
	for(int i=0;i<LINES;i++){
		for(int j=0;j<COLS;j++){
			NewScr[i][j]='\0';
		}
	}
	wmove(MAINBOX,0,0);
	int Realx=0; int Realy=0;
	for(int i=0;i<LINES;i++){
		int checks=0;
		for(int j=0;j<COLS;j++){
			if(((RealInity==i && RealInitx<=j) || RealInity<i) && ((RealFiny==i && RealFinx>=j) || RealFiny>j)){

			}else{
				checks=1;
				NewScr[Realy][Realx]=WhatIsOnScreen[j][i];
				mvwprintw(MAINBOX,Realy,Realx,"%c",WhatIsOnScreen[j][i]);
				Realx++;
			}
		}
		if(checks==1) {Realy++;Realx=1;}
	}
	for(int i=0;i<LINES;i++){
		strcpy(WhatIsOnScreen[i],NewScr[i]);
	}
}

int main(){
	initscr();
	start_color();
	cbreak();
	keypad(stdscr,TRUE);
	noecho();
	int starty=0;
	int startx=0;
	int height=LINES-2;
	int width=2;
	//WINDOW *win= newwin(3,4,1,2);
	init_pair(1,COLOR_BLACK,COLOR_YELLOW);
	init_pair(2,COLOR_BLACK,COLOR_BLUE);
	init_pair(3,COLOR_WHITE,COLOR_RED);
	//CreateWindow(win,3,4,1,2);
	//wrefresh(win);
	//wbkgd(win,COLOR_PAIR(1));
	refresh();
	WINDOW *SIDEBAR=newwin(LINES-2,4,0,0);
	WINDOW *MAINBOX=newwin(LINES-2,COLS-4,0,4);
	WINDOW *MODEBOX=newwin(1,12,LINES-2,0);
	WINDOW *COMMANDBOX=newwin(1,COLS,LINES-1,0);
	WINDOW *NAMEBOX=newwin(1,COLS-12,LINES-2,12);
	wrefresh(MAINBOX); wrefresh(MODEBOX); wrefresh(COMMANDBOX); wrefresh(NAMEBOX);
	wbkgd(MODEBOX,COLOR_PAIR(2));
	wbkgd(SIDEBAR,COLOR_PAIR(1));
	wrefresh(MODEBOX); wrefresh(SIDEBAR);
	for(int i=0;i<30;i++){
		for(int j=0;j<200;j++){
			WhatIsOnScreen[i][j]='\0';
		}
	}
	char OurMode='n'; //n: normal / i:insert / v:visual
	strcpy(address,"./root/dir/salam.txt");
	int save=1;
	int cnt=0;
	for(int i=0;i<9;i++){
		for(int j=0;j<SizeOfLine[i];j++){
			mohtaviat[cnt]='a';
			cnt++;
		}
		if(i!=50) mohtaviat[cnt]='\n';
		cnt++;
	}
	int BeginLine=0;
	int BeginChar=0;
	int MaxLine=0;
	while(1){
		OurMode='n';
		PrintMode(MODEBOX,OurMode);
		PrintSideBar(SIDEBAR,BeginLine);
		PrintName(NAMEBOX,address,save);
		char command[N]={'\0'};
		wmove(COMMANDBOX,0,0);
		char ch='\0';
		wrefresh(MAINBOX);
		ch=wgetch(COMMANDBOX);
		wclear(COMMANDBOX);
		wrefresh(COMMANDBOX);
		wrefresh(COMMANDBOX);
		wmove(COMMANDBOX,0,0);
		while(ch!='\n'){
			wprintw(COMMANDBOX,"%c",ch);
			wrefresh(COMMANDBOX);
			strncat(command,&ch,1);
			if(ch==27){
				wrefresh(MAINBOX);
				break;
			}
			if(ch=='i' && strlen(command)==1){
				break;
			}
			if(ch=='v' && strlen(command)==1){
				break;
			}
			if(ch=='=' && strlen(command)==1){
				break;
			}
			ch=wgetch(COMMANDBOX);
			wrefresh(COMMANDBOX);
		}
		wrefresh(MAINBOX);
		if(ch!='\n'){
			wclear(COMMANDBOX);
			wrefresh(COMMANDBOX);
			wrefresh(MAINBOX);
			if(ch==27){
				wrefresh(MAINBOX);
				wclear(COMMANDBOX);
				wmove(COMMANDBOX,0,0);
				wrefresh(MAINBOX);
				ch=wgetch(MAINBOX);
				while(ch!=27){
					wrefresh(MAINBOX);
					int x=0;
					int y=0;
					getyx(MAINBOX,y,x);
					if(ch=='l'){
						if(SizeOfLine[BeginLine+y]<=x){
						}else{
							if(x==COLS-5){
								BeginChar++;
								wclear(MAINBOX);
								PrintMain(MAINBOX,BeginLine,BeginChar);
								PrintSideBar(MAINBOX,BeginLine);
								wmove(MAINBOX,y,x);
							}else wmove(MAINBOX,y,x+1);
						}
					}else if(ch=='h'){
						if(x==0 && BeginChar==0){
						}else{
							if(x==0){
								BeginChar--;
								wclear(MAINBOX);
								PrintMain(MAINBOX,BeginLine,BeginChar);
								PrintSideBar(MAINBOX,BeginLine);
								wmove(MAINBOX,y,x);
							}else wmove(MAINBOX,y,x-1);
						}
					}else if(ch=='k'){
						if(y==0 && BeginLine==0){
						}else{
							if(y==0){
								BeginLine--;
								wclear(MAINBOX);
								PrintMain(MAINBOX,BeginLine,BeginChar);
								wclear(SIDEBAR);
								PrintSideBar(SIDEBAR,BeginLine);
								if(x+BeginChar>SizeOfLine[BeginLine+y-1]){
									wmove(MAINBOX,y,SizeOfLine[BeginLine+y-1]-BeginChar);
								}else{
									wmove(MAINBOX,y,x);
								}
							}else{
								if(x+BeginChar>SizeOfLine[BeginLine+y-1]){
									wmove(MAINBOX,y-1,SizeOfLine[BeginLine+y-1]-BeginChar);
								}else{
									wmove(MAINBOX,y-1,x);
								}
								PrintSideBar(SIDEBAR,BeginLine);
							}

						}
					}else if(ch=='j'){
						if(y>=LINES-4){
							BeginLine++;
							wclear(MAINBOX);
							PrintMain(MAINBOX,BeginLine,BeginChar);
							wclear(SIDEBAR);
							PrintSideBar(SIDEBAR,BeginLine);
							if(x+BeginChar>SizeOfLine[BeginLine+y+1]){
								wmove(MAINBOX,y,SizeOfLine[BeginLine+y+1]-BeginChar);
							}else{
								wmove(MAINBOX,y,x);
							}
						}else{
							if(x+BeginChar>SizeOfLine[BeginLine+y+1]){
								wmove(MAINBOX,y+1,SizeOfLine[BeginLine+y+1]-BeginChar);
							}else{
								wmove(MAINBOX,y+1,x);
							}
							PrintSideBar(SIDEBAR,BeginLine);
						}
					}
					wrefresh(SIDEBAR);
					wrefresh(MAINBOX);
					ch=getch();
				}
			}else if(ch=='i'){
				OurMode='i';
				PrintMode(MODEBOX,OurMode);
				wrefresh(MAINBOX);
				ch=wgetch(MAINBOX);
				char previous='\0';
				char preprevious='\0';
				int initialx=0; int initialy=0;
				getyx(MAIN,initialy,initialx);
				while(ch!=27){
					int x=0;
					int y=0;
					if(ch=='\\'){
						int x=0;
						int y=0;
						preprevious=previous;
						previous=ch;
						ch=wgetch(MAINBOX);
						if(ch==27) {getyx(MAINBOX,y,x); wprintw(MAINBOX,"%c",previous); WhatIsOnScreen[y][x]=previous; break;}
						else if(ch=='"') {
							getyx(MAINBOX,y,x);
							WhatIsOnScreen[y][x]=ch;
							wprintw(MAINBOX,"%c",ch);
							ch=wgetch(MAINBOX);
							continue;
						}else{
							getyx(MAINBOX,y,x);
							WhatIsOnScreen[y][x]=previous;
							wprintw(MAINBOX,"%c",previous);
						}
					}
					getyx(MAINBOX,y,x);
					WhatIsOnScreen[y][x]=ch;
					wprintw(MAINBOX,"%c",ch);
					preprevious=previous;
					previous=ch;
					wrefresh(MAINBOX);
					ch=wgetch(MAINBOX);
				}
			}else if(ch=='v'){
				int x=0;
				int y=0;
				getyx(MAINBOX,y,x);
				ch=wgetch(MAINBOX);
				while(ch!=27){
					wrefresh(MAINBOX);
					int x=0;
					int y=0;
					getyx(MAINBOX,y,x);
					if(ch=='l'){
						if(SizeOfLine[BeginLine+y]<=x){
						}else{
							if(x==COLS-5){
								BeginChar++;
								wclear(MAINBOX);
								PrintMain(MAINBOX,BeginLine,BeginChar);
								PrintSideBar(MAINBOX,BeginLine);
								wmove(MAINBOX,y,x);
							}else wmove(MAINBOX,y,x+1);
						}
					}else if(ch=='h'){
						if(x==0 && BeginChar==0){
						}else{
							if(x==0){
								BeginChar--;
								wclear(MAINBOX);
								PrintMain(MAINBOX,BeginLine,BeginChar);
								PrintSideBar(MAINBOX,BeginLine);
								wmove(MAINBOX,y,x);
							}else wmove(MAINBOX,y,x-1);
						}
					}else if(ch=='k'){
						if(y==0 && BeginLine==0){
						}else{
							if(y==0){
								BeginLine--;
								wclear(MAINBOX);
								PrintMain(MAINBOX,BeginLine,BeginChar);
								wclear(SIDEBAR);
								PrintSideBar(SIDEBAR,BeginLine);
								if(x+BeginChar>SizeOfLine[BeginLine+y-1]){
									wmove(MAINBOX,y,SizeOfLine[BeginLine+y-1]-BeginChar);
								}else{
									wmove(MAINBOX,y,x);
								}
							}else{
								if(x+BeginChar>SizeOfLine[BeginLine+y-1]){
									wmove(MAINBOX,y-1,SizeOfLine[BeginLine+y-1]-BeginChar);
								}else{
									wmove(MAINBOX,y-1,x);
								}
								PrintSideBar(SIDEBAR,BeginLine);
							}

						}
					}else if(ch=='j'){
						if(y>=LINES-4){
							BeginLine++;
							wclear(MAINBOX);
							PrintMain(MAINBOX,BeginLine,BeginChar);
							wclear(SIDEBAR);
							PrintSideBar(SIDEBAR,BeginLine);
							if(x+BeginChar>SizeOfLine[BeginLine+y+1]){
								wmove(MAINBOX,y,SizeOfLine[BeginLine+y+1]-BeginChar);
							}else{
								wmove(MAINBOX,y,x);
							}
						}else{
							if(x+BeginChar>SizeOfLine[BeginLine+y+1]){
								wmove(MAINBOX,y+1,SizeOfLine[BeginLine+y+1]-BeginChar);
							}else{
								wmove(MAINBOX,y+1,x);
							}
							PrintSideBar(SIDEBAR,BeginLine);
						}
					}
					wrefresh(SIDEBAR);
					wrefresh(MAINBOX);
					ch=getch();
				}
			}else if(ch=='='){

			}
			wclear(COMMANDBOX);
			wrefresh(COMMANDBOX);
		}else{
			sscanf(command,"%s %[^\n]%*c",temporary,address);
			if(!(strcmp(command,"save"))){

				if(!(strcmp(address,null))){
					mvwprintw(COMMANDBOX,0,0,"Please Choose an address");
					wrefresh(COMMANDBOX);	
				}else{
					char FAKECOMMAND[2*N]={'\0'};
					wrefresh(COMMANDBOX);		
					strcpy(FAKECOMMAND,null);
					FILE *fp2;
					fp2=fopen(address,"w");
					wprintw(COMMANDBOX,"SAAAAAAAAL");
					for(int i=0;i<9;i++){
						for(int j=0;j<SizeOfLine[i];j++){
							chtype Salam=mvwinch(MAINBOX,i,j);
							char c= (char) (Salam & A_CHARTEXT);
							wmove(MAINBOX,4,4);
							wprintw(MAINBOX,"%c",c);
							wrefresh(MAINBOX);
							fputc(c,fp2);
						}
					}
					fclose(fp2);
					//wclear(COMMANDBOX);
				}
			}else if(!(strcmp(temporary,":open"))){
				sscanf(command,"%s %[^\n]%*c",temporary,address);
				PrintToFile(MAINBOX);
			}
		}
	}
	endwin();			
	return 0;
}