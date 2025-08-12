#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>


const char* dir_name = "Assignment1/";

using namespace std;

void revarsal_logic(char buffer[], long long left_pt, long long right_pt)
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

void Block_revarsal(int fd, long long block_size, const char* file_name)
{
    if(block_size == 0)
    {
        perror("Wrong Block size entered.");
        return;
    }
    char buffer[block_size];
    long long end_pt = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    long long no_of_bytes_read = 0;

    long long bytes_remaining = end_pt;
    int fd_op = open((string("Assignment1/0_")+file_name).c_str(), O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0600);
    if(fd_op == -1)
    {
        perror("Invalid Path Name For The Output File.");
        return;
    }
    char progress_bar[30];
    while(bytes_remaining > 0)
    {
        long long percentage = ((end_pt - bytes_remaining)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        write(1, progress_bar, strlen(progress_bar));
        fflush(stdout);
        usleep(100);
        bytes_remaining -= block_size;
        no_of_bytes_read = read(fd, buffer, block_size);
        revarsal_logic(buffer, 0LL , no_of_bytes_read-1);
        write(fd_op, buffer, no_of_bytes_read);
        if(bytes_remaining<=0)
        {
            percentage = 100;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            write(1, progress_bar, strlen(progress_bar));
            fflush(stdout);
            write(1, "\n", 1);
        }
    }
   
    // processing percentage bar will come here.
    close(fd_op);
}


void Full_revarsal(int fd, const char* file_name)
{
    long long block_size = 1024*1024;
    char buffer[block_size];
    long long end_pt = lseek(fd, 0, SEEK_END);
    int fd_op = open((string("Assignment1/1_")+file_name).c_str(), O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0600);
    if(fd_op == -1)
    {
        perror("Invalid Path Name For The Output File.");
        return;
    }
    long long bytes_remaining = end_pt;
    long long no_of_bytes_read = 0;
    char progress_bar[30];
    while(bytes_remaining>0)
    {
        long long percentage = ((end_pt - bytes_remaining)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        write(1, progress_bar, strlen(progress_bar));
        fflush(stdout);
        usleep(100);
        bytes_remaining -= block_size;
        lseek(fd, max(bytes_remaining, 0LL), SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+bytes_remaining));
        revarsal_logic(buffer, 0LL, no_of_bytes_read-1);
        write(fd_op, buffer, no_of_bytes_read);

        if(bytes_remaining<=0)
        {
            percentage = 100;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            write(1, progress_bar, strlen(progress_bar));
            fflush(stdout);
            write(1, "\n", 1);
        }
    }


    //processing bar will come here.
    close(fd_op);
}


void Partial_revarsal(int fd, long long add_arg1, long long add_arg2, const char* file_name)
{
    if(add_arg1>add_arg2)
    {
        perror("Wrong starting or ending index entered. start index can't be more than end Index");
        return;
    }
    long long block_size = 1024*1024;
    char buffer[block_size];
    long long end_pt = lseek(fd, 0, SEEK_END);
    int fd_op = open((string("Assignment1/2_")+file_name).c_str(), O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0600);
    if(fd_op == -1)
    {
        perror("Invalid Path Name For The Output File.");
        return;
    }
    long long no_of_bytes_read = 0;

    //and there are 3 diff parts which need to be processed 
    //1st is revarsal from 0 to add_arg1-1 
    long long part1 = add_arg1;
    char progress_bar[30];
    while(part1>0)
    {
        long long percentage = ((add_arg1-part1)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        write(1, progress_bar, strlen(progress_bar));
        fflush(stdout);
        usleep(100000);
        part1 -= block_size;
        lseek(fd, max(part1, 0LL), SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part1));
        revarsal_logic(buffer, 0LL, no_of_bytes_read-1);
        write(fd_op, buffer, no_of_bytes_read);
        if(part1<0)
        {
            percentage = ((add_arg1)*100)/end_pt;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            write(1, progress_bar, strlen(progress_bar));
            fflush(stdout);

        }
    }


    //2nd is the middle part from add_arg1 to add_arg2 which should remain unchanged:
    lseek(fd, add_arg1, SEEK_SET);
    no_of_bytes_read = 0;
    long long part2 = add_arg2-add_arg1 +1;
    while(part2>0)
    {
        long long percentage = ((add_arg2+1 - part2)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        write(1, progress_bar, strlen(progress_bar));
        fflush(stdout);
        usleep(100);
        part2 -= block_size;
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part2));
        write(fd_op, buffer, no_of_bytes_read);
        if(part2<0)
        {
            percentage = ((add_arg2)*100)/end_pt;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            write(1, progress_bar, strlen(progress_bar));
            fflush(stdout);
            
        }
    }
    

    //3rd is revarsal from add_arg2+1 to end_pt
    long long part3 = (end_pt-1)-add_arg2;
    no_of_bytes_read = 0;
    while(part3>0)
    {
        long long percentage = ((end_pt-1 - part3)*100)/end_pt;
        snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
        write(1, progress_bar, strlen(progress_bar));
        fflush(stdout);
        usleep(100);
        part3 -= block_size;
        lseek(fd, add_arg2+max(part3, 0LL)+1, SEEK_SET);
        no_of_bytes_read = read(fd, buffer, min(block_size, block_size+part3));
        revarsal_logic(buffer, 0LL, no_of_bytes_read-1);
        write(fd_op, buffer, no_of_bytes_read);
        if(part3<0)
        {
            percentage = 100;
            snprintf(progress_bar, sizeof(progress_bar), "\rProgress : ( %lld%% / 100)", percentage);
            write(1, progress_bar, strlen(progress_bar));
            fflush(stdout);
            write(1, "\n", 1);
        }
    }
    

    //processing bar will come here
    close(fd_op);
}


int main(int args,const char* arguments[])
{
    if(args <3 || args >5)
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
    if(!(flag == 0 || flag == 1 || flag == 2))
    {
        perror("Invalid flag entered.");
        return 0;
    }
    long long add_arg1;
    long long add_arg2;

    if(args == 4)
    {
        try{
            add_arg1=stoll(arguments[3]);
        }
        catch(invalid_argument e){
            perror("Invalid Block Size");
            return 0;
        }
    }
    else if(args == 5)
    {
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

    int fd = open(file_name, O_RDWR);

    if(fd == -1)
    {
        perror("Invalid File Name Entered.");
        return 0;
    }


    //creating the directory.
    int dir = mkdir(dir_name, 0700);
    

    if(flag == 0)
        Block_revarsal(fd, add_arg1, file_name);
    else if(flag == 1)
        Full_revarsal(fd, file_name);
    else if(flag == 2)
        Partial_revarsal(fd, add_arg1, add_arg2, file_name);
    else
    {
        perror("Wrong flag entered.");
        close(fd);
    }

    return 0;
}