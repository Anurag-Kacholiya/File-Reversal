## AOS - Assignment 1
# File Reversal in Different Modes 
language - C++

# Submitted by
Name - Anurag Kacholiya
Roll No. - 2025202025 
(M.tech - CSIS)
__________________________________________________________________
__________________________________________________________________

# Overview
This project implements file content reversal using system calls in C++ with multiple modes, as well as file and permission verification. The assignment consists of two parts:

**Q1**: Reverse file contents in one of three modes:

**Block-wise reversal : (flag 0)** – reverse each block of fixed size individually.
**Full file reversal : (flag 1)** – reverse the entire file as a whole.
**Partial range reversal : (flag 2)** – reverse portions before and after a specified range, leaving the middle part intact.

**Q2**: Verify the generated output file and directory for:

Correct reversal pattern according to the flag used
Matching file sizes
Correct permissions for all newFile, oldFile and Directory.
__________________________________________________________________

# Execution instructions
**Q1**
code compilation : g++ 2025202025_A1_Q1.cpp

execution format for command line arguments : ./a.out <input file name> <flag> [<blockSize>|<start> <end>]

**Flag 0 :**
**command :** ./a.out <input file> 0 <block size>
**Output file for flag 0 :**  Assignment1/0_<input file name>

**Flag 1 :**
**command :** ./a.out <input file> 1
**Output file for flag 1 :**  Assignment1/1_<input file name>

**Flag 2 :**
**command :** ./a.out <input file> 2 <start index> <end index>
**Output file for flag 2 :**  Assignment1/2_<input file name>
__________________________________________________________________

**Q2**
code compilation : g++ 2025202025_A1_Q2.cpp

execution format for command line arguments : ./a.out <newfilepath> <oldfilepath> <directory> <flag> [<blockSize>|<start> <end>]

**Flag 0 :**
**command :** ./a.out Assignment1/<newFile name> <oldFile name> Assignment1 0 <block size>

**Flag 1 :**
**command :** ./a.out Assignment1/<newFile name> <oldFile name> Assignment1 1

**Flag 2 :**
**command :** ./a.out Assignment1/<newFile name> <oldFile name> Assignment1 2 <start index> <end index>

**Output Format for Q2 :**
Directory is created: Yes
Whether file contents are correctly processed: Yes
Both Files Sizes are Same: Yes
User has read permissions on newfile: Yes
User has write permission on newfile: Yes
User has execute permission on newfile: No
Group has read permissions on newfile: No
Group has write permission on newfile: No
Group has execute permission on newfile: No
Others has read permissions on newfile: No
Others has write permission on newfile: No
Others has execute permission on newfile: No

For each: new file, old file, and directory (total 30 lines).
__________________________________________________________________

# Code Workflow
**Question - 1 : File Reversal with Multiple Modes**
2025202025_A1_Q1.cpp is the file for the Question 1.
following are the functions defined in the program:
1) int main(int args, char const* arguments[]) - default start of c++ program with command line arguments.
2) void reversal_logic(char buffer[], long long left_pt, long long right_pt)
3) void Block_reversal(int fd, long long block_size, const char* file_name)
4) void Full_reversal(int fd, const char* file_name)
5) void Partial_reversal(int fd, long long add_arg1, long long add_arg2, const char* file_name)

Workflow is as follows : 
**In main function :**
1) Arguments parsing - which extracts command-line arguments to identify input file, mode (flag), and any extra parameters.
2) Directory Creation - Creates Assignment1 directory with 700 permissions (user read/write/execute) for outputs.
3) Mode Selection - Based on the flag and calling respective function.
                    i.e. if flag = 0 -> Block_reversal(fd, block_size, file_name);
                         if flag = 1 -> Full_reversal(fd, file_name);
                         if flag = 2 -> Partial_reversal(fd, add_arg1, add_arg2, file_name);

**In Block_reversal function :**
0) output file creation as per required format with permissions.
1) Read file in fixed-size blocks.
2) Reverse each block individually. -> calling reversal_logic(buffer, left_pt, right_pt);
3) Write to output file sequentially.
4) Progress Bar.
5) Repeate until EOF.

**In Full_reversal function :**
0) output file creation as per required format with permissions.
1) Seek to the end of the file.
2) Read chunks backward.
3) Reverse each chunk. -> calling reversal_logic(buffer, left_pt, right_pt);
4) Write to output file.
5) Progress Bar.
6) Repeate until start of file.

**In Partial_reversal function :**
0) output file creation as per required format with permissions.
Process three regions:
1) Start(0) → start_index - 1 (reversed) - same reversal logic as Full_reversal.
2) start_index → end_index (unchanged) - just copying data from input file to output file in chunks.
3) end_index + 1 → EOF (reversed) - same reversal logic as Full_reversal.
______________________________________________________________

**Question - 2 : File and Permission Verification**
2025202025_A1_Q2.cpp is the file for the Question 1.
following are the functions defined in the program:
1) int main(int args, const char* arguments[]) - default start of c++ program with command line arguments.
2) void permission_print(bool is_dir, bool is_verified, bool is_size_same, vector<bool> oldfile, vector<bool> newfile, vector<bool> dir)
3) vector<bool> permissions(struct stat file)
4) bool reversal_logic_check(char arr1[], char arr2[], long long end)
5) bool every_byte_same_check(char arr1[], char arr2[], long long end)
6) bool Block_reversal_check(int fd_op, int fd_ip, long long block_size)
7) bool Full_reversal_check(int fd_op, int fd_ip)
8) bool Partial_reversal_check(int fd_op, int fd_ip, long long add_arg1, long long add_arg2)


Workflow is as follows : 
**In main function :**
1) Arguments parsing - which extracts command-line arguments to identify input file, mode (flag), and any extra parameters.
2) Directory Verification - Uses stat() to check if the given directory exists or not.
3) Permission Check - Checks read, write, execute permission bits for user, group and others of newFile, oldFile and directory.
4) File Size Match Check – Compares sizes of input and output files to detect obvious mismatches.
5) File verification - Based on the flag and calling respective function.
                       i.e. if flag = 0 -> Block_reversal_check(fd, block_size, file_name);
                            if flag = 1 -> Full_reversal_check(fd, file_name);
                            if flag = 2 -> Partial_reversal_check(fd, add_arg1, add_arg2, file_name);
6) Final Verification Result – Prints whether verification succeeded or failed.

**In Block_reversal_check function :**
0) Position file pointers to the beginning.
1) Read both files in fixed-size blocks.
2) For each block from the output file, verify that it is the reversed form of the corresponding block in the input file using reversal_logic_check().
3) Repeat until EOF.

**In Full_reversal_check function :**
0) Determine file size using lseek().
1) Compare first block of the input file with the reversed last block of the output file, then move inward.
2) Use reversal_logic_check() for each block pair.
3) ontinue until all data has been compared.

**In Partial_reversal_check function :**
Process three regions:
1) Start region (0 → start_index - 1) – Verify reversed content matches original start segment using reversal_logic_check().
2) Middle region (start_index → end_index) – Verify content matches exactly between input and output.
3) End region (end_index + 1 → EOF) – Verify reversed content matches corresponding end segment from input.

**In permissions function :**
1) checks all permissions i.e. for user, group and other for oldFile, newFile and directory(which ever it is called for).
2) stores value in a vector<bool> of size 9 and returns it.
______________________________________________________________