// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str1[] = "howard.edu"; 
    char fixed_str2[] = "gobison.org";
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe 1 Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe 2 Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        write(fd1[1], input_str, strlen(input_str)+1);
        close(fd1[1]);

        wait(NULL);

        char recv_str[100];
        close(fd2[1]);
        read(fd2[0], recv_str, 100);
        close(fd2[0]);

        strcat(recv_str, fixed_str2);
        printf("Final concatenated string %s\n", recv_str);
    } 
  
    // child process 
    else
    {    
        // Read a string using first pipe 
        char concat_str[100]; 
        close(fd1[1]);  // Close writing end of first pipes 
        read(fd1[0], concat_str, 100); 
        close(fd1[0]);

        strcat(concat_str, fixed_str1);
        printf("Concatenated string: %s\n", concat_str);

        char second_input[100];
        printf("Enter a string to concatenate (P2 to P1): ");
        scanf("%s", second_input);

        close(fd2[0]);
        write(fd2[1], concat_str, strlen(concat_str)+1);
        close(fd2[1]);
    } 
    return 0;
} 