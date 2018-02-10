/*
Cameron Elwood
Unversity of Victoria
CSC 360
V00152812
Assignment 1
*/


#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#define MAXLENGTH 35

/*
struct pidVal
this is an object with 3 members:
	1: a character array called name, for the name of the process
	2: a integer id, for the pid of the process
	3: an integer active, which is a 1 or 0 based on if the process is active or not

struct pidDataStruct
this is an object with 2 members:
	1: an integer num, which counts the number of values in the data structure
	2: an pidVal array called pidVals, which is of length n (max 35)

struct pidValStruct
this is an instance of the data struct pidDataStruct

The names are abit confusing, might mix later

*/
struct pidVal{
		char * name;
		int id;
		int active;
};

struct pidDataStruct{
	pid_t last;
	int num;
	struct pidVal pidVals[MAXLENGTH];
};

struct pidDataStruct pidValStruct;



/*
function getNum
returns an integer, the location of the current pid
takes in an int
this function, using a pid, will get the process tied to it.
*/
int getNum(int pid){
	for(int i = 0; i < pidValStruct.num; i++){
		if(pidValStruct.pidVals[i].id == pid){
			return i;
		}
	}
	return -1;
}

/*
function getProcess
returns a character array
takes in an int
this function, using a pid, will get the process tied to it.
*/
char * getProcess(int pid){
	for(int i = 0; i < pidValStruct.num; i++){
		if(pidValStruct.pidVals[i].id == pid){
			return pidValStruct.pidVals[i].name;
		}
	}
	return "NotFound";
}

/*
function getPid
returns an integer
takes in a character array
this function, using a processes name, returns the pid tied to it.
*/
int getPid(char * process){
	for(int i = 0; i < pidValStruct.num; i++){
		if(strcmp(pidValStruct.pidVals[i].name, process) == 0){
			return pidValStruct.pidVals[i].id;
		}
	}
	return -1;
}

/*
function startPID
returns a character array as the error statment, Null if there are no errors
takes in the PID to be started
*/
char * startPID(int pidToStart){
	int idNum = getNum(pidToStart);
	if(idNum == -1){
		return "PID Number Not Found";
	}

	int check = kill(pidToStart, SIGCONT);
	if(check < 0){
		return "Was Not Started";
	}


	pidValStruct.pidVals[idNum].active = 1;
	return NULL;

}

/*
function stopPID
returns a character array as the error statment, Null if there are no errors
takes in the PID to be stopped
*/
char * stopPID(int pidToStop){
	int idNum = getNum(pidToStop);
	if(idNum == -1){
		return "PID Number Not Found";
	}

	int check = kill(pidToStop, SIGSTOP);
	if(check < 0){
		return "Was Not Stopped";
	}


	pidValStruct.pidVals[idNum].active = 2;
	return NULL;

}
/*
function EvaluatePID
returns a character array as the error statment, Null if there are no errors
takes in the pid to be evaluated

*/
char * EvaluatePID(int pidToEvaluate){
	int idNum = getNum(pidToEvaluate);
	if(idNum == -1){
		return "PID Number Not Found";
	}

	FILE *file;
	char buffer[255];
	char * values[9];

	char path[30];
	sprintf(path, "/proc/%d/stat", pidToEvaluate);

	//parse the information from the stat file
	file = fopen(path, "r");
	int count = 1;
	int val = 0;
	if(file){
		while(fscanf(file, "%s", buffer) != EOF){
			if(count == 2 | count ==3 | count ==14 | count ==15 | count ==24){
				//printf("number%d: value: %s\n", count, buffer);
				values[val] = malloc(sizeof(char)*(strlen(buffer)+3));
				strncpy(values[val], buffer, strlen(buffer)+2);
				val++;
			}
			count++;
		}
		fclose(file);
	}else{
		return "error with file stat";
	}

	//prase remaining information from status file
	sprintf(path, "/proc/%d/status", pidToEvaluate);
	file = fopen(path, "r");
	if(file){
		while(fgets(buffer, 240, file) != NULL){
			if(strstr(buffer, "voluntary_ctxt_switches") != NULL){
				//printf("value: %s\n",buffer);
				values[val] = malloc(sizeof(char)*(strlen(buffer)+3));
				strncpy(values[val], buffer, strlen(buffer)+2);
				val++;
			}
		}
		fclose(file);
	}else{
		return "error with file stat";
	}
	values[val] = NULL;

	float time = (float)atoi(values[2])/sysconf(_SC_CLK_TCK);
	float time2 = (float)atoi(values[3])/sysconf(_SC_CLK_TCK);
	int counter = 2;
	int lengthStr = strlen(values[5]);
	char val1 = ' ';
	while(val1 == ' '){
		if(counter > (lengthStr-10)){
			break;
		}
		val1 = values[5][lengthStr-counter];
		counter++;
		
	}
	char val2 = ' ';
	counter = 2;
	lengthStr = strlen(values[6]);
	while(val2 == ' ' ){
		if(counter > (lengthStr-10)){
			break;
		}
		val2 = values[6][lengthStr-counter];
		counter++;
		
	}

	printf("FileName: %s\nState: %s\nUtime: %f\nStime: %f\nRSS: %s\nVoluntary_ctxt_switches: %c\nNonvoluntary_ctxt_switches: %c\n", values[0], values[1], time, time2, values[4],val1,val2);

	for(int i = 0; i < val; i++){
		free(values[i]);
	}
	return NULL;
}

/*
function killIt
takes in a pid to kill
returns a char array
changes the ddata structure to reflect the killed process
*/
char * killIt(int pidToKill){

	int check = getNum(pidToKill);
	if(check < 0){
		return "Does Not Exist";
	}

	int checkTwo = kill(pidToKill, SIGTERM);
	if(checkTwo < 0){
		return "Was Not Killed";
	}

	pidValStruct.last = pidValStruct.pidVals[check].id;

	char * holdName = pidValStruct.pidVals[pidValStruct.num-1].name;
	int holdID = pidValStruct.pidVals[pidValStruct.num-1].id;
	int holdActive = pidValStruct.pidVals[pidValStruct.num-1].active;

	pidValStruct.pidVals[check].id = holdID;
	pidValStruct.pidVals[check].name = holdName;
	pidValStruct.pidVals[check].active = holdActive;

	pidValStruct.pidVals[pidValStruct.num-1].id = 0;
	pidValStruct.pidVals[pidValStruct.num-1].name = NULL;
	pidValStruct.pidVals[pidValStruct.num-1].active = 0;
	pidValStruct.num--;

	printf("PID: %d Has been terminated.\n", pidToKill);

	return NULL;
}


/*
function printList
returns no value
prints the current processes in the file manager, and lists out the number of current processes.
*/
int printList(){
	for(int i =0; i< pidValStruct.num; i++){
		printf("%d: %s\n", pidValStruct.pidVals[i].id, pidValStruct.pidVals[i].name);
	}
	printf("Total background jobs: %d\n", pidValStruct.num);

}
/*
function runBG
returns a character array which is used for error handling.
paramaters: 
*/
char * runBG(char * processPath, char *args[]){
	pid_t child;
	//check process path
	if(access(processPath, F_OK) == -1){
		return "invalid file";
	}
	if(access(processPath, X_OK) == -1){
		return "invalid permissions";
	}

	child = fork();
	if(child < 0){
		perror("fork\n"); /* display error message */
        return NULL; 
	}else if(child > 0){
		pidValStruct.pidVals[pidValStruct.num].id = child;
		char * name;
		if(processPath[0] == '.'){
			name = processPath + 2;
		}else{
			name = processPath;
		}
		pidValStruct.pidVals[pidValStruct.num].name = name; 
		pidValStruct.pidVals[pidValStruct.num].active = 1;
		pidValStruct.num++;
		return NULL;
		
	}else{
		//printf("process path: %s\n", processPath);
		if(execvp(processPath, args)<0){
			perror("Error in execvp\n");
		}
	}
}
/*
function identifyCommand
takes in a character array, which is input from the user
returns a char*, signifying which command to use, or null if its an invalid command
*/

char * identifyCommand(char * input){
	char * returnStatment = NULL;
	char * token = strtok(input, " ");
	if(token == NULL){
			printf("Not Enough Arguments\n");
			return NULL;
		}

	if(strcmp(token, "bg") == 0){
		//getting the executable set up for running 
		if(pidValStruct.num == 35){
			printf("too many processes, capped at 35.\n Please Terminate processes before adding more.\n");
			return NULL;
		}
		char * p = strtok(NULL, " ");
		if(p == NULL){
			printf("Not Enough Arguments\n");
			return NULL;
		}
		char * path = malloc(sizeof(char)*(strlen(p)+4));
		if(p[0] != '/'){
			strncpy(path, "./", 3);
			strncat(path, p, (strlen(p)+2));
		}else{
			strncpy(path, p, strlen(p)+2);
		}
		int inputLength = strlen(input);
		char * args[inputLength+3];

		args[0] = p;
		int counter = 1;
		token = strtok(NULL, " ");
		while(token != NULL){
			args[counter] = token;
			//printf("arg%d: %s\n", counter,  args[counter]);
			token = strtok(NULL, " ");
			counter++;
		}
		args[counter] = "&";
		args[counter+1] = NULL;
		returnStatment = runBG(path, args);
		if(returnStatment != NULL){
			printf("error: %s %s\n", path, returnStatment);
			free(path);
			return "cnf";
		}
	}else if(strcmp(token, "bglist") == 0){
		printList();
	}else if(strcmp(token, "bgkill")==0){
		char * pidT = strtok(NULL, " ");
		if(pidT == NULL){
			printf("Not Enough Arguments\n");
			return NULL;
		}
		int pidToKill = atoi(pidT);
		returnStatment = killIt(pidToKill);
		if(returnStatment != NULL){
			printf("Error: Process %d %s\n", pidToKill, returnStatment);
		}
	}else if(strcmp(token, "pstat") == 0){
		char * pidE= strtok(NULL, " ");
		if(pidE == NULL){
			printf("Not Enough Arguments\n");
			return NULL;
		}
		int pidToEvaluate = atoi(pidE);
		returnStatment = EvaluatePID(pidToEvaluate);
		if(returnStatment != NULL){
			printf("Error: Process %d %s\n", pidToEvaluate, returnStatment);
		}

	}else if(strcmp(token, "bgstop") == 0){
		char * pidS= strtok(NULL, " ");
		if(pidS == NULL){
			printf("Not Enough Arguments\n");
			return NULL;
		}
		int pidToStop = atoi(pidS);
		returnStatment = stopPID(pidToStop);
		if(returnStatment != NULL){
			printf("Error: Process %d %s\n", pidToStop, returnStatment);
		}

	}else if(strcmp(token, "bgstart") == 0){
		char * pidS= strtok(NULL, " ");
		if(pidS == NULL){
			printf("Not Enough Arguments\n");
			return NULL;
		}
		int pidToStart = atoi(pidS);
		returnStatment = startPID(pidToStart);
		if(returnStatment != NULL){
			printf("Error: Process %d %s\n", pidToStart, returnStatment);
		}

	}else{
		printf("%s: command not found\n", token);
	}
}




int main(){
	pidValStruct.num = 0;
	pidValStruct.last = -1;

	while(1){
		
	    char *input = NULL ;
	    //need to do waitpid here could store the value of the destroyed process in the data structure
	    //and refference it from here
	    if(pidValStruct.last != -1){
			waitpid(pidValStruct.last, 0, WNOHANG);
			pidValStruct.last = -1;
	    }
	    char *prompt = "PMan:  >";
	    input = readline(prompt);
	    if(strcmp(input, "")==0){
	    	free(input);
	    	continue;
	    }

	    char * function = identifyCommand(input);
	    free(input);
	  
    }
}