#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>

using namespace std;

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
            add_arg1=stoi(arguments[5]);
        }
        catch(invalid_argument e){
            perror("Invalid Block Size");
            return 0;
        }
    }
    else if(args == 7)
    {
        try{
            add_arg1=stoi(arguments[5]);
        }
        catch(invalid_argument e){
            perror("Invalid Starting Index");
            return 0;
        }

        try{
            add_arg2=stoi(arguments[6]);
        }
        catch(invalid_argument e){
            perror("Invalid End Index");
            return 0;
        }
    }

    int fd_op = open((string(dir_name)+"/"+string(output_file_name)).c_str(), O_RDWR);

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
    




    close(fd_op);
    close(fd_ip);
    return 0;
}