// ImagePreview.cpp : Defines the entry point for the application.
//

#include "ImagePreview.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp> // for Fit, canvas, operator|, border, Element
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/event.hpp>

int main(int argc, char *argv[])
{
  using namespace ftxui;
  if (argc < 2)
  {
    std::cerr << "error: requires path to an image as an argument" << std::endl;
    return -1;
  }
  Console console{};
  Image thumb;
  Image image{};
  auto c = Canvas(console.width(), console.height());
  auto screen = ScreenInteractive::FitComponent();
  try
  {
    image.open(argv[1]);
    float imgRatio = static_cast<float>(image.width()) / image.height();
    thumb = image; // copy ...
    int width = static_cast<int>(console.height() * imgRatio);
    thumb.resize(width, console.height());

    int offX = (c.width() - thumb.width()) / 2;
    int offY = (c.height() - thumb.height()) / 2;

    unsigned char *pixel{nullptr};
    unsigned char rgb[3] = {0, 0, 0};
    float alpha = 0.0f;

    auto renderer = Renderer([&]()
                             { 

    for (int y = 0; y < thumb.height(); ++y)
    {
      for (int x = 0; x < thumb.width(); ++x)
      {
        alpha = 1.0f;    
        pixel = thumb.data() + ((y*thumb.width() + x)*thumb.channels());
        if(thumb.channels() == 4){
          if (pixel[3] == 0){
            continue; // do not draw transparent pixels ...
          }
          alpha = static_cast<float>(pixel[3]) / 255.0f;
        }
        for (int c = 0; c < 3 && c < thumb.channels(); ++c){
          rgb[c] = pixel[c] * alpha;
        }
        c.DrawBlock(x+offX,y+offY, true, Color(rgb[0], rgb[1], rgb[2]));
      }
    }
    return canvas(&c) | border; });

    auto canvasWithExit = CatchEvent(renderer, [&](Event event)
                                     {
        if (event == Event::Character('q')) {
          screen.ExitLoopClosure()();
          return true;
        }
        return false; });

    screen.Loop(canvasWithExit);
  }
  catch (const std::exception &e)
  {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
