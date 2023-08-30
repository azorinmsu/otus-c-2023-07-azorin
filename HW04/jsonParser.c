
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>
#include <stdio.h>
#include "jsonParser.h"
#include <json-c/json.h>

struct json_object* findNodeByKey(struct json_object* jobj, char* key) {
  struct json_object* tmp;

  json_object_object_get_ex(jobj, key, &tmp);

  return tmp;
}

void parseResponse(char* res) {
  struct json_object* root = json_tokener_parse(res);

  struct json_object* weather = json_object_object_get(root, "weather");
  int size = json_object_array_length(weather);

  for (int i = 0; i < size; ++i) {
    json_object* element = json_object_array_get_idx(weather, i);
    
    json_object* minTempC = json_object_object_get(element, "mintempC");
    json_object* maxTempC = json_object_object_get(element, "maxtempC");
    json_object* date = json_object_object_get(element, "date");
    printf("Weather for date=%s: minTempC=%s and maxTempC=%s\n", 
           json_object_get_string(date), json_object_get_string(minTempC), json_object_get_string(maxTempC));
    
    struct json_object* hourly = json_object_object_get(element, "hourly");
    int hourlySize = json_object_array_length(hourly);

    for (int j = 0; j < hourlySize; ++j) {
      json_object* weatherForTime = json_object_array_get_idx(hourly, j);

      json_object* time = json_object_object_get(weatherForTime, "time");
      json_object* winddirDegree = json_object_object_get(weatherForTime, "winddirDegree");
      json_object* windspeedKmph = json_object_object_get(weatherForTime, "windspeedKmph");
      json_object* weatherDesc = json_object_object_get(weatherForTime, "weatherDesc");
      json_object* weatherDescIdx = json_object_array_get_idx(weatherDesc, 0);
      json_object* weatherDescValue = json_object_object_get(weatherDescIdx, "value");

      printf("For time %s weather is %s and winddirDegree=%s with windspeedKmph=%s\n", 
             json_object_get_string(time), json_object_get_string(weatherDescValue), 
             json_object_get_string(winddirDegree), json_object_get_string(windspeedKmph));
    }

    printf("\n");
  }


}
