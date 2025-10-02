#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/Regex.h"

int main(){
    Regex rx = Regex_New("[a-z,A-Z]* \"@\" [a-z,A-Z]* \".\" [a-z,A-Z]*");

    Regex_Do(&rx," 1");
    //Regex_DoFile(&rx,"01");

    Regex_Print(&rx);
    Regex_Free(&rx);
    return 0;
}