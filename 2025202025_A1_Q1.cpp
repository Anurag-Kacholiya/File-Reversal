#include<unistd.h>
#include<fcntl.h>
#include<vector>
#include<string.h>
#include<iostream>
#include <sys/stat.h>
#include <sys/types.h>


const char* dir_name = "Assignment1/";

using namespace std;

void revarsal_logic(char buffer[], int left_pt, int right_pt)
{
    while(left_pt <= right_pt)
    {
       
        char temp = buffer[left_pt];
        buffer[left_pt] = buffer[right_pt];
        buffer[right_pt] = temp;
        left_pt++;
        right_pt--;
    }
}

void Block_revarsal(int fd, int block_size)
{
    if(block_size == 0)
    {
        perror("Wrong Block size entered.");
        return;
    }
    char buffer[block_size];
    int start_pt = lseek(fd, 0, SEEK_CUR);
    int end_pt = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    int size;


    int fd_op = open("Assignment1/0_input.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd_op == -1)
    {
        perror("Invalid Path Name For The Output File.");
        return;
    }
    int byte_sum = 0;

    while(byte_sum < end_pt)
    {
        int no_of_bytes_read = read(fd, buffer, block_size);
        revarsal_logic(buffer, 0 , no_of_bytes_read-1);
        write(fd_op, buffer, no_of_bytes_read);
        byte_sum += no_of_bytes_read;
    }
   
    // processing percentage bar will come here.
    close(fd_op);
}


void Full_revarsal(int fd)
{
    int block_size = 4;//1024*1024;
    char buffer[block_size];
    int end_pt = lseek(fd, 0, SEEK_END);
    int fd_op = open("Assignment1/1_input.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    // if(fd_op == -1)
    // {
    //     perror("Invalid Path Name For The Output File.");
    //     return;
    // }
    int size;
    int file_size = end_pt;
    int no_of_bytes_read = 0;
    while(file_size>0)
    {
        file_size = file_size-block_size;
        lseek(fd, max(file_size, 0), SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+file_size));
        revarsal_logic(buffer, 0, no_of_bytes_read-1);
        write(fd_op, buffer, no_of_bytes_read);
    }


    //processing bar will come here.
    close(fd_op);
}


void Partial_revarsal(int fd, int add_arg1, int add_arg2)
{
    if(add_arg1>add_arg2)
    {
        perror("Wrong starting or ending index entered. start index can't be more than end Index");
        return;
    }
    int block_size = 4;//1024*1024;
    char buffer[block_size];
    int end_pt = lseek(fd, 0, SEEK_END);
    int fd_op = open("Assignment1/2_input.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd_op == -1)
    {
        perror("Invalid Path Name For The Output File.");
        return;
    }
    int no_of_bytes_read = 0;

    //and there are 3 diff parts which need to be processed 
    //1st is revarsal from 0 to add_arg1-1 
    int part1 = add_arg1;
    while(part1>0)
    {
        part1 = part1-block_size;
        lseek(fd, max(part1, 0), SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part1));
        revarsal_logic(buffer, 0, no_of_bytes_read-1);
        write(fd_op, buffer, no_of_bytes_read);
    }


    //2nd is the middle part from add_arg1 to add_arg2 which should remain unchanged:
    int end_mid = lseek(fd, add_arg2, SEEK_SET);
    int start_mid = lseek(fd, add_arg1, SEEK_SET);
    no_of_bytes_read = 0;
    int part2 = end_mid-start_mid +1;
    while(part2>0)
    {
        part2 -= block_size;
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part2));
        write(fd_op, buffer, no_of_bytes_read);
    }
    

    //3rd is revarsal from add_arg2+1 to end_pt
    int part3 = (end_pt-1)-add_arg2;
    no_of_bytes_read = 0;
    while(part3>0)
    {
        part3 = part3-block_size;
        lseek(fd, add_arg2+max(part3, 0)+1, SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part3));
        revarsal_logic(buffer, 0, no_of_bytes_read-1);
        write(fd_op, buffer, no_of_bytes_read);
    }
    

    //processing bar will come here
    close(fd_op);
}


int main(int args,const char* arguments[])
{
    if(args <3)
    {
        perror("Less arguments passed then expected.");
        return 0;
    }
    //extracting all data from the input from the user like input file name, flag, block size, start and end index
    const char* file_name=arguments[1];
    int flag;
    try{
        flag = stoi(arguments[2]);
    }
    catch(invalid_argument e){
        perror("Invalid Flag Entered");
        return 0;
    }
    int add_arg1;
    int add_arg2;

    if(args == 4)
    {
        try{
            add_arg1=stoi(arguments[3]);
        }
        catch(invalid_argument e){
            perror("Invalid Block Size");
            return 0;
        }
    }
    else if(args == 5)
    {
        try{
            add_arg1=stoi(arguments[3]);
        }
        catch(invalid_argument e){
            perror("Invalid Starting Index");
            return 0;
        }

        try{
            add_arg2=stoi(arguments[4]);
        }
        catch(invalid_argument e){
            perror("Invalid End Index");
            return 0;
        }
    }

    int fd = open(file_name, O_RDWR);

    if(fd == -1)
    {
        perror("Invalid File Name Entered.");
        return 0;
    }


    //creating the directory.
    int dir = mkdir(dir_name, 0700);
    // if(dir != 0)
    // {
    //     perror("Directory Was Not Created");
    // }
    

    if(flag == 0)
        Block_revarsal(fd, add_arg1);
    else if(flag == 1)
        Full_revarsal(fd);
    else if(flag == 2)
        Partial_revarsal(fd, add_arg1, add_arg2);
    else
    {
        perror("Wrong flag entered.");
        close(fd);
    }

    return 0;
}