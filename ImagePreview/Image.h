#ifndef IMAGE_H_
#define IMAGE_H_

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include <string>
#include <exception>
#include <utility>
#include <iostream>
#include <cstdlib>
#include <cstring>

class read_error : public std::exception
{
  std::string what_;

public:
  read_error(const char *str) : what_{str} {}
  inline const char *what() const noexcept override
  {
    return what_.c_str();
  }
};

class write_error : public std::exception
{
  std::string what_;

public:
  write_error(const char *str) : what_{str} {}
  inline const char *what() const noexcept override
  {
    return what_.c_str();
  }
};

class resize_error : public std::exception
{
  std::string what_;

public:
  resize_error(const char *str) : what_{str} {}
  inline const char *what() const noexcept override
  {
    return what_.c_str();
  }
};

class Image
{
  std::string file_{};
  unsigned char *data_{nullptr};
  int width_{0};
  int height_{0};
  int channels_{0};

public:
  inline Image() {}

  inline Image(const Image &other)
  {
    Image tmp{other.copy()};
    swap(tmp);
  }

  Image &operator=(const Image &other)
  {
    if (this == &other)
    {
      return *this;
    }
    Image tmp{other.copy()};
    swap(tmp);
    return *this;
  }

  inline Image copy() const
  {
    Image tmp{};
    std::size_t size = width_ * height_ * channels_;
    tmp.data_ = static_cast<unsigned char *>(std::malloc(size));
    if (!tmp.data_)
    {
      throw std::runtime_error{"out of memory"};
    }
    std::memcpy(tmp.data_, data_, size);
    tmp.width_ = width_;
    tmp.height_ = height_;
    tmp.channels_ = channels_;
    return tmp;
  }

  inline Image(Image &&other) noexcept
  {
    if (this == &other)
    {
      return;
    }
    this->swap(other);
  }

  inline Image &operator=(Image &&other) noexcept
  {
    if (this == &other)
    {
      return *this;
    }
    this->swap(other);
    return *this;
  }

  inline ~Image()
  {
    if (data_)
    {
      stbi_image_free(data_);
      data_ = nullptr;
    }
  }
  inline void open(const std::string_view name)
  {
    if (data_)
    {
      stbi_image_free(data_);
      data_ = nullptr;
    }
    file_ = static_cast<std::string>(name);
    data_ = stbi_load(file_.c_str(), &width_, &height_, &channels_, 0);
    if (!data_)
    {
      throw read_error{stbi_failure_reason()};
    }
  }

  inline unsigned char *data() const
  {
    return data_;
  }

  inline int width() const
  {
    return width_;
  }

  inline int height() const
  {
    return height_;
  }

  inline int channels() const
  {
    return channels_;
  }

  inline void swap(Image &img) noexcept
  {
    std::swap(file_, img.file_);
    std::swap(data_, img.data_);
    std::swap(width_, img.width_);
    std::swap(height_, img.height_);
    std::swap(channels_, img.channels_);
  }

  inline void save(const std::string_view dest)
  {
    auto file = static_cast<std::string>(dest);
    if (!stbi_write_png(file.c_str(), width_, height_, channels_, data_, width_ * channels_))
    {
      throw write_error{"failed to save image"};
    }
  }

  inline void resize(int width, int height, int channels = 4)
  {

    if (!stbir_resize_uint8_generic(data_, width_, height_, width_ * channels_,
                                    data_, width, height, width * channels,
                                    channels, channels == 4 ? 3 : 0, 0,
                                    STBIR_EDGE_ZERO, STBIR_FILTER_DEFAULT, STBIR_COLORSPACE_LINEAR,
                                    nullptr))
    {
      std::cout << "resize error" << std::endl;
      throw resize_error{"failed to resize image"};
    };

    width_ = width;
    height_ = height;
    channels_ = channels;
  }

  friend void swap(Image &left, Image &right) noexcept;
};

inline void swap(Image &left, Image &right) noexcept
{
  left.swap(right);
}

#endif