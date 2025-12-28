#include "process.hpp"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

ChildProcess::ChildProcess(const std::string &command) {

  if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1) {
    throw std::runtime_error("Pipe failed");
  }
  std::cout << "afetr pipe " << command << std::endl;

  pid = fork();
  std::cout << "after fork: pid=" << pid << "\n";
  if (pid == 0) {
    // child process
    dup2(stdin_pipe[0], STDIN_FILENO);
    dup2(stdout_pipe[1], STDOUT_FILENO);

    close(stdin_pipe[0]);
    close(stdin_pipe[1]);
    close(stdout_pipe[0]);
    close(stdout_pipe[1]);

    execlp("sh", "sh", "-c", command.c_str(), nullptr);
    exit(1);
  } else if (pid > 0) {
    // parent process
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
  } else {
    throw std::runtime_error("Fork failed");
  }
}

ChildProcess::~ChildProcess() {
  if (pid > 0) {
    close(stdin_pipe[1]);
    close(stdout_pipe[0]);
    waitpid(pid, nullptr, 0);
  }
}

void ChildProcess::write(const std::string &message) {
  ::write(stdin_pipe[1], message.c_str(), message.size());
  // ::write(stdin_pipe[1], "\n", 1);
}

inline char ChildProcess::getchar() {
  char ch;
  ssize_t bytes = read(stdout_pipe[0], &ch, 1);

  if (bytes == 1) {
    return ch;
  } else if (bytes == 0) {
    return EOF;
  } else {
    // read failed
    assert(false);
  }
}

std::string ChildProcess::readline() {
  std::string line;
  while (true) {
    char ch = this->getchar();
    if (ch == '\n')
      break;
    line.push_back(ch);
  }
  return line;
}
