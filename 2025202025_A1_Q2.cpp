#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>
#include <iostream>

using namespace std;

bool reversal_logic_check(char arr1[], char arr2[], long long end)
{
    int start = 0;
    while(start < end)
    {
        if(arr1[start] != arr2[end-start])
            return false;

        start++;
    }
    return true;
}

bool every_byte_same_check(char arr1[], char arr2[], long long end)
{
    int start = 0;
    while(start < end)
    {
        if(arr1[start] != arr2[start])
            return false;

        start++;
    }
    return true;
}




bool Block_revarsal_check(int fd_op, int fd_ip, long long block_size)
{
    char buffer_input[block_size];
    char buffer_output[block_size];
    long long total_bytes_op = lseek(fd_op, 0, SEEK_END);
    long long total_bytes_ip = lseek(fd_ip, 0, SEEK_END);
    if(total_bytes_ip != total_bytes_op)
        return false;
    lseek(fd_op, 0, SEEK_SET);
    lseek(fd_ip, 0, SEEK_SET);
    long long no_of_bytes_read = 0;
    long long bytes_remaining = total_bytes_ip;
    bool is_verified_correctly = true;
    while(bytes_remaining > 0 && is_verified_correctly)
    {
        bytes_remaining -= block_size;
        no_of_bytes_read = read(fd_op, buffer_output, block_size);
        read(fd_ip, buffer_input, block_size);
        is_verified_correctly = reversal_logic_check(buffer_output, buffer_input, no_of_bytes_read-1);
    }
    return is_verified_correctly;
}

bool Full_revarsal_check(int fd_op, int fd_ip)
{
    long long block_size = 4;//1024*1024;
    char buffer_input[block_size];
    char buffer_output[block_size];
    long long total_bytes_op = lseek(fd_op, 0, SEEK_END);
    long long total_bytes_ip = lseek(fd_ip, 0, SEEK_END);
    if(total_bytes_ip != total_bytes_op)
    {
        cout<<"size dont match.";
        return false;
    }
    long long bytes_remaining = total_bytes_op;
    long long no_of_bytes_read = 0;
    bool is_verified_correctly = true;
    lseek(fd_ip, 0, SEEK_SET);
    while(bytes_remaining > 0 && is_verified_correctly)
    {
        bytes_remaining -= block_size;
        lseek(fd_op, max(bytes_remaining, 0LL), SEEK_SET);
        no_of_bytes_read = read(fd_op, buffer_output, min(block_size, block_size+bytes_remaining));
        read(fd_ip, buffer_input, min(block_size, block_size+bytes_remaining));
        is_verified_correctly = reversal_logic_check(buffer_output, buffer_input, no_of_bytes_read-1);
    }
    
    return is_verified_correctly;
}

bool Partial_revarsal_check(int fd_op, int fd_ip, long long add_arg1, long long add_arg2)
{
    if(add_arg1>add_arg2)
    {
        perror("Wrong starting or ending index entered. start index can't be more than end Index");
        return false;
    }
    long long block_size = 4;//1024*1024;
    char buffer_input[block_size];
    char buffer_output[block_size];
    long long total_bytes_op = lseek(fd_op, 0, SEEK_END);
    long long total_bytes_ip = lseek(fd_ip, 0, SEEK_END);
    if(total_bytes_ip != total_bytes_op)
    {
        cout<<"size dont match.";
        return false;
    }
    long long no_of_bytes_read = 0;

    bool is_verified_correctly = true;
    lseek(fd_ip, 0, SEEK_SET);

    //1st part : from 0 to start index -1
    long long part1 = add_arg1;
    while(part1 > 0 && is_verified_correctly)
    {
        part1 -= block_size;
        lseek(fd_op, max(part1, 0LL), SEEK_SET);
        no_of_bytes_read = read(fd_op, buffer_output, min(block_size, block_size+part1));
        read(fd_ip, buffer_input, min(block_size, block_size+part1));
        is_verified_correctly = reversal_logic_check(buffer_output, buffer_input, no_of_bytes_read-1);
    }
    
    //2nd part : from start index to end index
    lseek(fd_op, add_arg1, SEEK_SET);
    lseek(fd_ip, add_arg1, SEEK_SET);
    no_of_bytes_read = 0;
    long long part2 = add_arg2-add_arg1 +1;
    while(part2>0 && is_verified_correctly)
    {
        part2 -= block_size;
        no_of_bytes_read = read(fd_op, buffer_output, min(block_size, block_size+part2));
        read(fd_ip, buffer_input, min(block_size, block_size+part2));
        is_verified_correctly = every_byte_same_check(buffer_output, buffer_input, no_of_bytes_read-1);
    }

    //3rd part : from end index to EOF
    long long part3 = (total_bytes_op-1)-add_arg2;
    no_of_bytes_read = 0;
    while(part3>0 && is_verified_correctly)
    {
        part3 -= block_size;
        lseek(fd_op, add_arg2+max(part3, 0LL)+1, SEEK_SET);
        no_of_bytes_read = read(fd_op, buffer_output, min(block_size, block_size+part3));
        read(fd_ip, buffer_input, min(block_size, block_size+part3));
        is_verified_correctly = reversal_logic_check(buffer_output, buffer_input, no_of_bytes_read-1);
    }

    return is_verified_correctly;
}


int main(int args, const char* arguments[])
{
    if(args < 5 || args > 7)
    {
        perror("Less arguments passed then expected.");
        return 0;
    }
    const char* output_file_name = arguments[1];
    const char* input_file_name = arguments[2];
    const char* dir_name = arguments[3];
    int flag;
    try{
        flag = stoi(arguments[4]);
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

    if(args == 6)
    {
        try{
            add_arg1=stoll(arguments[5]);
        }
        catch(invalid_argument e){
            perror("Invalid Block Size");
            return 0;
        }
    }
    else if(args == 7)
    {
        try{
            add_arg1=stoll(arguments[5]);
        }
        catch(invalid_argument e){
            perror("Invalid Starting Index");
            return 0;
        }

        try{
            add_arg2=stoll(arguments[6]);
        }
        catch(invalid_argument e){
            perror("Invalid End Index");
            return 0;
        }
    }

    int fd_op = open(output_file_name, O_RDWR);

    if(fd_op == -1)
    {
        perror("Invalid File Name Entered.");
        return 0;
    }

    int fd_ip = open(input_file_name, O_RDWR);

    if(fd_ip == -1)
    {
        perror("Invalid File Name Entered.");
        return 0;
    }
    

    bool file_verification = false;

    if(flag == 0)
        file_verification = Block_revarsal_check(fd_op, fd_ip, add_arg1);
    else if(flag == 1)
        file_verification = Full_revarsal_check(fd_op, fd_ip);
    else if(flag == 2)
        file_verification = Partial_revarsal_check(fd_op, fd_ip, add_arg1, add_arg2);
    else
    {
        perror("Wrong flag entered.");
    }


    cout<<"are the files correctly verified : ";
    if(file_verification == true)
        cout<<"yes"<<endl;
    else
        cout<<"no"<<endl;


    close(fd_op);
    close(fd_ip);
    return 0;
}