#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

unsigned int countSpaces(const std::string &s);                    // counts white spaces in a string
char **convert(const std::string &s, const std::string &procname); // converts from a string to a array of strings using space seperation

int main(int argc, char *argv[])
{
    std::string input;
    std::string tmp;
    std::string procName;
    std::size_t pos;
    pid_t cpid;
    pid_t wait;
    int status;

    std::vector<pid_t> jobs;
    char **cargs = nullptr;

    while (1)
    {
        std::getline(std::cin, input);

        if (input.compare("jobs") == 0)
        {
            for (unsigned int i = 0; i < jobs.size(); ++i)
            {
                std::cout << i + 1 << "\t" << jobs[i] << "\n";
            }
            continue;
        }

        pos = input.find(" ");

        if (pos != std::string::npos)
        {
            procName = input.substr(0, pos);
            cargs = convert(input.substr(pos + 1), procName);
        }
        else
        {
            procName = input;
            cargs = new char *[2];
            cargs[0] = new char[procName.length()];
            strncpy(cargs[0], procName.c_str(), procName.length());
            cargs[1] = (char *)0;
        }

        cpid = fork();

        if (cpid == 0)
        {
            int x;
            if (x = execve(procName.c_str(), cargs, NULL))
            {
                perror("Execve Error");
                exit(-1);
            };
        }
        else
        {
            if (waitpid(cpid, &status, WUNTRACED | WCONTINUED) == -1)
            {
                perror("waitpid");
                exit(-1);
            }

            if (WIFEXITED(status) && WEXITSTATUS(status) != 255)
            {
                jobs.push_back(cpid);
                std::cout << "The PID of " << procName << " is " << cpid << "\n";
            }
        }
    }

    return 0;
}

unsigned int countSpaces(const std::string &s)
{
    unsigned int count = 0;
    for (unsigned int i = 0; i < s.length(); ++i)
    {
        if (s[i] == ' ')
            ++count;
    }
    return count;
}

char **convert(const std::string &s, const std::string &procname)
{
    unsigned int count = countSpaces(s);
    std::stringstream ss(s);
    std::string tmp;
    char **c = new char *[count + 2];

    c[count + 1] = (char *)0;
    c[0] = new char[procname.length()];
    strncpy(c[0], procname.c_str(), procname.length());

    count = 1;
    while (ss.good())
    {
        ss >> tmp;

        c[count] = new char[tmp.length()];
        strncpy(c[count], tmp.c_str(), tmp.length());
        ++count;
    }

    return c;
}