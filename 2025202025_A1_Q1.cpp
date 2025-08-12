#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>


const char* dir_name = "Assignment1/";

using namespace std;

void reversal_logic(char buffer[], long long left_pt, long long right_pt)
{
    //this function performs reversal of any give array with start and end index inplace with help of swapping logic.
    while(left_pt <= right_pt)
    {
        char temp = buffer[left_pt];
        buffer[left_pt] = buffer[right_pt];
        buffer[right_pt] = temp;
        left_pt++;
        right_pt--;
    }
}

void Block_reversal(int fd, long long block_size, const char* file_name)
{
    //this function performs and contains the full logic of flag 0 i.e. block wise reversal.
    if(block_size == 0)
    {
        perror("Wrong Block size entered.");
        return;
    }

    //initializing all necessary variables.
    char buffer[block_size];
    long long end_pt = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    long long no_of_bytes_read = 0;
    long long bytes_remaining = end_pt;
    char progress_bar[44];

    //creating / opening output file.
    int fd_op = open((string("Assignment1/0_")+file_name).c_str(), O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0600);
    if(fd_op == -1)
    {
        perror("Error in creating output file.");
        return;
    }
    
    //logic of block reversal and processbar.
    while(bytes_remaining > 0)
    {
        //calculating the percentage of file that is processed for process bar. and printing / overwriting processbar.
        long long percentage = ((end_pt - bytes_remaining)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        if (write(1, progress_bar, strlen(progress_bar)) == -1) {
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }
        fflush(stdout);

        //reading block by block from start to EOF of the file.
        bytes_remaining -= block_size;
        no_of_bytes_read = read(fd, buffer, block_size);
        if(no_of_bytes_read == -1){
            perror("unable to perform read function.");
            close(fd_op);
        }
        reversal_logic(buffer, 0LL , no_of_bytes_read-1);
        if(write(fd_op, buffer, no_of_bytes_read)){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }

        //just for the edge case of last iteration we need to print the progressbar for the last iteration as if we don't do it here then it will not be printed.
        if(bytes_remaining<=0)
        {
            percentage = ((end_pt - 0)*100)/end_pt;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            if(write(1, progress_bar, strlen(progress_bar))){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }
            fflush(stdout);
            if(write(1, "\n", 1)){
                perror("unable to perform write function.");
                close(fd_op);
                return;
            }
        }
    }
   
    // closing the output file.
    close(fd_op);   
}


void Full_reversal(int fd, const char* file_name)
{
    //this function performs and contains the full logic of flag 1 i.e. Full file reversal.

    //initializing all necessary variables.
    long long block_size = 1024*1024;
    char buffer[block_size];
    long long end_pt = lseek(fd, 0, SEEK_END);
    long long bytes_remaining = end_pt;
    long long no_of_bytes_read = 0;
    char progress_bar[44];

    //creating / opening output file.
    int fd_op = open((string("Assignment1/1_")+file_name).c_str(), O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0600);
    if(fd_op == -1)
    {
        perror("Error in creating output file.");
        return;
    }
    
    //logic of full file reversal and processbar.
    while(bytes_remaining>0)
    {
        //calculating the percentage of file that is processed for process bar. and printing / overwriting processbar.
        long long percentage = ((end_pt - bytes_remaining)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        if(write(1, progress_bar, strlen(progress_bar))){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }
        fflush(stdout);
        
        //assuming there are n blocks in the file of the defined block size above.
        //we are reversing the (n-i)th block in every iteration i.e. from the end block by block.
        bytes_remaining -= block_size;
        lseek(fd, max(bytes_remaining, 0LL), SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+bytes_remaining));  
        if(no_of_bytes_read == -1){
            perror("unable to perform read function.");
            close(fd_op);
        }
        reversal_logic(buffer, 0LL, no_of_bytes_read-1);
        if(write(fd_op, buffer, no_of_bytes_read)){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }     

        if(bytes_remaining<=0)
        {
            percentage = ((end_pt-0)*100)/end_pt;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            if(write(1, progress_bar, strlen(progress_bar))){
                perror("unable to perform write function.");
                close(fd_op);
                return;
            }
            fflush(stdout);
            if(write(1, "\n", 1)){
                perror("unable to perform write function.");
                close(fd_op);
                return;
            }
        }
    }


    close(fd_op);
}


void Partial_reversal(int fd, long long add_arg1, long long add_arg2, const char* file_name)
{
    //this function performs and contains the full logic of flag 1 i.e. Full file reversal.
    if(add_arg1>add_arg2)
    {
        perror("Wrong starting or ending index entered. start index can't be more than end Index");
        return;
    }

    //initializing all necessary variables.
    long long block_size = 1024*1024;
    char buffer[block_size];
    long long no_of_bytes_read = 0;
    long long part1 = add_arg1;
    char progress_bar[44];
    long long end_pt = lseek(fd, 0, SEEK_END);
    
    if(add_arg1 < 0 ||  add_arg2>end_pt )
    {//condition when the input indices are out of bound.
        perror("Wrong starting or ending index entered. Index is Out Of Bound.");
        return;
    }

    //creating / opening output file.
    int fd_op = open((string("Assignment1/2_")+file_name).c_str(), O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0600);
    if(fd_op == -1)
    {
        perror("Error in creating output file.");
        return;
    }
    

    //and there are 3 diff parts which need to be processed 
    //1st is revarsel from 0 to add_arg1-1 
    //logic is same as full reversal.
    while(part1>0)
    {
        //calculating the percentage of file that is processed for process bar. and printing / overwriting processbar.
        long long percentage = ((add_arg1-part1)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        if(write(1, progress_bar, strlen(progress_bar))){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }
        fflush(stdout);
        
        part1 -= block_size;
        lseek(fd, max(part1, 0LL), SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part1));
        if(no_of_bytes_read == -1){
            perror("unable to perform read function.");
            close(fd_op);
        }
        reversal_logic(buffer, 0LL, no_of_bytes_read-1);
        if(write(fd_op, buffer, no_of_bytes_read)){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }

        if(part1<0)
        {
            percentage = ((add_arg1)*100)/end_pt;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            if(write(1, progress_bar, strlen(progress_bar))){
                perror("unable to perform write function.");
                close(fd_op);
                return;
            }
            fflush(stdout);

        }
    }


    //2nd is the middle part from add_arg1 to add_arg2 which should remain unchanged:
    lseek(fd, add_arg1, SEEK_SET);
    no_of_bytes_read = 0;
    long long part2 = add_arg2-add_arg1 +1;
    while(part2>0)
    {
        //calculating the percentage of file that is processed for process bar. and printing / overwriting processbar.
        long long percentage = ((add_arg2+1 - part2)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        if(write(1, progress_bar, strlen(progress_bar))){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }
        fflush(stdout);

        part2 -= block_size;
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part2));
        if(no_of_bytes_read == -1){
            perror("unable to perform read function.");
            close(fd_op);
        }
        if(write(fd_op, buffer, no_of_bytes_read)){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }

        if(part2<0)
        {
            percentage = ((add_arg2)*100)/end_pt;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            if(write(1, progress_bar, strlen(progress_bar))){
                perror("unable to perform write function.");
                close(fd_op);
                return;
            }
            fflush(stdout);
            
        }
    }
    

    //3rd is reversal from add_arg2+1 to end_pt
    //logic is same as full reversal.
    long long part3 = (end_pt-1)-add_arg2;
    no_of_bytes_read = 0;
    while(part3>0)
    {
        //calculating the percentage of file that is processed for process bar. and printing / overwriting processbar.
        long long percentage = ((end_pt-1 - part3)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        if(write(1, progress_bar, strlen(progress_bar))){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }
        fflush(stdout);
        
        part3 -= block_size;
        lseek(fd, add_arg2+max(part3, 0LL)+1, SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part3));
        if(no_of_bytes_read == -1){
            perror("unable to perform read function.");
            close(fd_op);
        }
        reversal_logic(buffer, 0LL, no_of_bytes_read-1);
        if(write(fd_op, buffer, no_of_bytes_read)){
            perror("unable to perform write function.");
            close(fd_op);
            return;
        }

        if(part3<0)
        {
            percentage = ((end_pt-0)*100)/end_pt;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            if(write(1, progress_bar, strlen(progress_bar))){
                perror("unable to perform write function.");
                close(fd_op);
                return;
            }
            fflush(stdout);
            if(write(1, "\n", 1)){
                perror("unable to perform write function.");
                close(fd_op);
                return;
            }
        }
    }
    

    close(fd_op);
}


int main(int args,const char* arguments[])
{
    //checking if the no. of arguments are within the range.
    if(args <3)
    {
        perror("Less arguments passed then expected.");
        return 0;
    }
    else if(args >5)
    {
        perror("More arguments passed then expected.");
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
    if(!(flag == 0 || flag == 1 || flag == 2))
    {
        perror("Invalid flag entered.");
        return 0;
    }

    long long add_arg1;
    long long add_arg2;
    if(args == 4)
    {//only 1 additional argument i.e. block size
        try{
            add_arg1=stoll(arguments[3]);
        }
        catch(invalid_argument e){
            perror("Block Size must be an Integer.");
            return 0;
        }
    }
    else if(args == 5)
    {//2 additional arguments i.e. start index and end index.
        try{
            add_arg1=stoll(arguments[3]);
        }
        catch(invalid_argument e){
            perror("Invalid Starting Index");
            return 0;
        }

        try{
            add_arg2=stoll(arguments[4]);
        }
        catch(invalid_argument e){
            perror("Invalid End Index");
            return 0;
        }
    }

    //creating / opening output file.
    int fd = open(file_name, O_RDWR);
    if(fd == -1)
    {
        perror("Invalid File Name Entered.");
        return 0;
    }


    //creating the directory.
    mkdir(dir_name, 0700);
    struct stat dir;
    if(stat(dir_name, &dir) != 0)
    {
        perror("There was an error while creating directory for outputs.");
        return 0;
    }

    //calling the functions as per flag requirements for file reversal.
    if(flag == 0)
        Block_reversal(fd, add_arg1, file_name);
    else if(flag == 1)
        Full_reversal(fd, file_name);
    else if(flag == 2)
        Partial_reversal(fd, add_arg1, add_arg2, file_name);
    else
    {
        perror("Wrong flag entered.");
    }

    //closing the input file.
    close(fd);
    return 0;
}