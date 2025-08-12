#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

#include <iostream>

using namespace std;




void permission_print(bool is_dir, bool is_verified, bool is_size_same, vector<bool> oldfile, vector<bool> newfile, vector<bool> dir)
{
    //prints all the permissions as per the given format.
    write(1, "Directory is created: ", strlen("Directory is created: "));
    is_dir ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

    write(1, "Whether file contents are correctly processed: ", strlen("Whether file contents are correctly processed: "));
    is_verified ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

    write(1, "Both Files Sizes are Same: ", strlen("Both Files Sizes are Same: "));
    is_size_same ? write(1, "Yes\n", 4) : write(1, "No\n", 3);
    
    //old file permissions
    string who;
    int ch_field = 0;
    for(int i = 0; i<9; i++)
    {
        if(ch_field == 0)
            who = "User";
        else if(ch_field == 1)
            who = "Group";
        else
            who = "Others";

        string read = who + " has read permissions on oldfile: ";
        write(1, read.c_str(), read.size());
        oldfile[i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        string write_ = who + " has write permissions on oldfile: ";
        write(1, write_.c_str(), write_.size());
        oldfile[++i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        string execute = who + " has execute permissions on oldfile: ";
        write(1, execute.c_str(), execute.size());
        oldfile[++i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        ch_field++;
    }

    //new file
    ch_field = 0;
    for(int i = 0; i<9; i++)
    {
        if(ch_field == 0)
            who = "User";
        else if(ch_field == 1)
            who = "Group";
        else
            who = "Others";

        string read = who + " has read permissions on newfile: ";
        write(1, read.c_str(), read.size());
        newfile[i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        string write_ = who + " has write permissions on newfile: ";
        write(1, write_.c_str(), write_.size());
        newfile[++i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        string execute = who + " has execute permissions on newfile: ";
        write(1, execute.c_str(), execute.size());
        newfile[++i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        ch_field++;
    }


    //dir
    ch_field = 0;
    for(int i = 0; i<9; i++)
    {
        if(ch_field == 0)
            who = "User";
        else if(ch_field == 1)
            who = "Group";
        else
            who = "Others";

        string read = who + " has read permissions on directory: ";
        write(1, read.c_str(), read.size());
        dir[i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        string write_ = who + " has write permissions on directory: ";
        write(1, write_.c_str(), write_ .size());
        dir[++i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        string execute = who + " has execute permissions on directory: ";
        write(1, execute.c_str(), execute.size());
        dir[++i] ? write(1, "Yes\n", 4) : write(1, "No\n", 3);

        ch_field++;
    }
}

vector<bool> permissions(struct stat file)
{
    //function to check and return all permissions for a given file.
    bool user_read = file.st_mode & S_IRUSR;
    bool user_write = file.st_mode & S_IWUSR;
    bool user_execute = file.st_mode & S_IXUSR;
    
    bool group_read = file.st_mode & S_IRGRP;
    bool group_write = file.st_mode & S_IWGRP;
    bool group_execute = file.st_mode & S_IXGRP;
    
    bool other_read = file.st_mode & S_IROTH;
    bool other_write = file.st_mode & S_IWOTH;
    bool other_execute = file.st_mode & S_IXOTH;
    
    vector<bool> all_permissions = {user_read, user_write, user_execute, group_read, group_write, group_execute, other_read, other_write, other_execute};
    return all_permissions;
    
}



bool reversal_logic_check(char arr1[], char arr2[], long long end)
{
    //main logic to check if the given two arrays contain the same characters just reversed from each other.
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
    //main logic to check if the given two arrays contain the same characters in the same order.
    //exclusive for the middle part of flag 2.
    int start = 0;
    while(start < end)
    {
        if(arr1[start] != arr2[start])
            return false;

        start++;
    }
    return true;
}




bool Block_reversal_check(int fd_op, int fd_ip, long long block_size)
{
    //logic to verify the output and input files for flag 0.
    // takes data from 2 different files in 2 different buffers block wise (from start i.e. 0) to check if those data are reverse of each other.
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

bool Full_reversal_check(int fd_op, int fd_ip)
{
    //logic to verify the output and input files for flag 1.
    // takes data from 2 different files in 2 different buffers block wise - one from start i.e. 0 and other from (n-i)th block from end to check if those data are reverse of each other.
    long long block_size = 1024*1024;
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

bool Partial_reversal_check(int fd_op, int fd_ip, long long add_arg1, long long add_arg2)
{
    //logic to verify the output and input files for flag 2.
    // takes data from 2 different files in 2 different buffers block wise but works in 3 different parts which are commented below.
    if(add_arg1>add_arg2)
    {
        perror("Wrong starting or ending index entered. start index can't be more than end Index");
        return false;
    }
    long long block_size = 1024*1024;
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
    //this part uses same logic as Full_reversal_check for the above given range.
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
    //this part just validates if the data is exactly same in same order. 
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
    //this part uses same logic as Full_reversal_check for the above given range.
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
    //checking if the no. of arguments are within the range.
    if(args < 5)
    {
        perror("Less arguments passed then expected.");
        return 0;
    }
    if(args > 7)
    {
        perror("More arguments passed then expected.");
        return 0;
    }
    
    //extracting all data from the input from the user like input file name, flag, block size, start and end index
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
    {//only 1 additional argument i.e. block size
        try{
            add_arg1=stoll(arguments[5]);
        }
        catch(invalid_argument e){
            perror("Block Size must be an Integer.");
            return 0;
        }
    }
    else if(args == 7)
    {//2 additional arguments i.e. start index and end index.
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

    //creating / opening output file.
    int fd_op = open(output_file_name, O_RDWR);
    if(fd_op == -1)
    {
        perror("Invalid File Name Entered.");
        return 0;
    }

    //creating / opening input file.
    int fd_ip = open(input_file_name, O_RDWR);
    if(fd_ip == -1)
    {
        perror("Invalid File Name Entered.");
        return 0;
    }
    

    //calling the functions as per flag requirements for file verification and storing values in boolean(true if verified else false).
    bool file_verification = false;
    if(flag == 0)
        file_verification = Block_reversal_check(fd_op, fd_ip, add_arg1);
    else if(flag == 1)
        file_verification = Full_reversal_check(fd_op, fd_ip);
    else if(flag == 2)
        file_verification = Partial_reversal_check(fd_op, fd_ip, add_arg1, add_arg2);
    else
    {
        perror("Wrong flag entered.");
    }

    //using the stat system call features to get the access of all data of oldFile(input of Q1), newfile(output of Q1), and directory of outputs.
    struct stat oldFile;
    struct stat newFile;
    struct stat dir;
    
    stat(input_file_name, &oldFile);
    stat(output_file_name, &newFile);
    
    //retrieving the permissions.
    bool is_dir = (stat(dir_name, &dir) == 0);
    bool sizes = (oldFile.st_size == newFile.st_size);
    vector<bool> ip_per = permissions(oldFile);
    vector<bool> op_per = permissions(newFile);
    vector<bool> dir_per = permissions(dir);
    
    //final printing of all the outputs in required format.
    permission_print(is_dir, file_verification, sizes, ip_per, op_per, dir_per);

    
    //closing the newfile as well as old file.
    close(fd_op);
    close(fd_ip);
    return 0;
}