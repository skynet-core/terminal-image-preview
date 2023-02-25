#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <exception>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif // _WIN32

#if defined(__unix__) || defined(__linux__)
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO
#endif

class Console
{
  int width_{};
  int height_{};

public:
  inline Console()
  {
#if defined _WIN32 || defined _WIN64
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width_ = csbi.dwSize.X;
    height_ = csbi.dwSize.Y;
#elif defined(__unix__) || defined(__linux__)
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0)
    {
      throw std::runtime_error{"ioctl: failed to get console size"};
    }
    width_ = size.ws_col;
    height_ = size.ws_row;
#else
    static_assert(false, "unsupported platform");
#endif // _WIN32 || defined _WIN64
  }

  inline int width() const
  {
    return width_;
  }

  inline int height() const
  {
    return height_;
  }
};

#endif
