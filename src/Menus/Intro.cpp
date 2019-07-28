#include <map>
#include "Utility/Utility.h"

using namespace std;

void intro()
{
  Image *image = rosalila()->graphics->getImage(std::string(assets_directory) + "menu/studio.png");
  Image *image2 = rosalila()->graphics->getImage(std::string(assets_directory) + "menu/game_ilustration.png");
  int current_fade_in = 0;

  /*
    rosalila()->graphics->notification_handler.notifications.push_back(
        new Notification(getLoadingImage(), rosalila()->graphics->screen_width/2-getLoadingImage()->getWidth()/2,
                            rosalila()->graphics->screen_height,
                            rosalila()->graphics->screen_height-getLoadingImage()->getHeight(),
                            999999));
*/
  int frames = 0;
  if (!rosalila()->api_integrator->isUsingApi())
  {
    rosalila()->graphics->notification_handler.interruptCurrentNotification();
  }
  while (true)
  {
    if (rosalila()->api_integrator->getState() != "loading" && frames > 180)
    {
      rosalila()->sound->playSound("Menu.confirm", 1, 0, 0);
      rosalila()->graphics->notification_handler.interruptCurrentNotification();
      break;
    }
    if (rosalila()->receiver->isPressed(0, "a") && rosalila()->api_integrator->getState() != "loading")
    {
      rosalila()->sound->playSound("Menu.right", 0, 0, 0);
      rosalila()->graphics->notification_handler.interruptCurrentNotification();
      break;
    }

    if (rosalila()->receiver->isPressed(0, "back"))
    {
      exit(0);
    }

    image->color_filter.red = current_fade_in;
    image->color_filter.green = current_fade_in;
    image->color_filter.blue = current_fade_in;
    rosalila()->graphics->drawImage(image, 0, 0);

    /*
        if(rosalila()->api_integrator->isUsingApi())
        {
          rosalila()->graphics->drawText(rosalila()->utility->toString(rosalila()->api_integrator->getLeaderboardDownloadPercentage())+"%", 20, 20, false, false);
        }
        */

    current_fade_in += 2;
    if (current_fade_in > 255)
      current_fade_in = 255;

    frames++;

    rosalila()->update();
  }

  int current_background_transparency = 0;

  frames = 0;

  while (true)
  {
    if (frames > 180)
    {
      break;
    }
    if (rosalila()->receiver->isPressed(0, "a"))
    {
      rosalila()->sound->playSound("Menu.right", 0, 0, 0);
      break;
    }

    if (rosalila()->receiver->isPressed(0, "back"))
    {
      exit(0);
    }

    image2->color_filter.alpha = current_background_transparency;
    rosalila()->graphics->drawImage(image2, 0, 0);

    current_background_transparency += 2;
    if (current_background_transparency > 255)
      current_background_transparency = 255;

    frames++;

    rosalila()->update();
  }
}
