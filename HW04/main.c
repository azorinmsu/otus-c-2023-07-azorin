#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "weather.h"

int main(int argc, char** argv)
{
  if (argc != 2) {
    perror("The name of city must be passed");
    exit(1);
  }

  char* location = argv[1];

  requestWeatherForLocation(location);

  return EXIT_SUCCESS;
}
