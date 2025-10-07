#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/Regex.h"

int main(){
    //Regex rx = Regex_Make("a \"@\" a \".\" a");
    //Regex rx = Regex_Make("[a-z,A-Z]* \"@\" [a-z,A-Z]* \".\" [a-z,A-Z]*");
    Regex rx = Regex_Make("Hello World");

    Regex_Do(&rx,"HelloWorld Hello World Helloworld");
    //Regex_DoFile(&rx,"./data/File.txt");

    Regex_Print(&rx);
    Regex_Free(&rx);
    return 0;
}