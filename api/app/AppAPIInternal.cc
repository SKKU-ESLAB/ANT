#include <string.h>

#include "AppAPIInternal.h"

// 0 = Invalid
// 1 = Noti page
// 2 = Config page
// 3 = Config event driven page
int check_page_name(const char* name){
  char tempString[15];

  strncpy(tempString, name, 14);
  tempString[14] = '\0';

  if(!strcmp(tempString, "{\"noti\":\"noti\"")) {
    return 1;
  } else if(!strcmp(tempString, "{\"conf\":\"conf\"")) {
    return 2;
  } else if(!strcmp(tempString, "{\"conf_eventd\"")) {
    return 3;
  } else {
    return 0;
  }
}

bool check_key_overlap(const char* json, const char* name){
  int position = 0;

  char key[512];
  memset(key, '\0', 512);

  for(unsigned int i=0; i< strlen(json); i++) {

    if ( json[i] == '{' ) {
      continue;
    } else if ( json[i] == '}' ) {
      if(!strcmp(key, name)){
        return false;
      }
    } else if ( json[i] == ':' ) {
      if( json[i+1] == '"')
        position++;
    } else if ( json[i] == ',' ) {
      if( json[i+1] == '"'){
        position++;
        if(!strcmp(key, name)){
          return false;
        }
        else{
          memset(key, '\0', 512);
        }
      }
    } else if ( json[i] == '"' ) {
      continue;
    } else {
      char tmp[2];
      tmp[1] = '\0';
      tmp[0] = json[i];			

      if(!(position % 2)) // even num -> key
        strcat(key, tmp);
      else{}				// odd num -> value
    }
  }
  return true;
}
