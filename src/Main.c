#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/Regex.h"

int main(){
    //Regex rx = Regex_Make("a \"@\" a \".\" a");
    //Regex rx = Regex_Make("[a-z,A-Z]* \"@\" [a-z,A-Z]* \".\" [a-z,A-Z]*");
    Regex rx = Regex_Make("A*N*");

    Regex_Do(&rx,"AAABBBBAABBBABAABBBBBABABABBBB");
    //Regex_DoFile(&rx,"./data/File.txt");

    Regex_Print(&rx);
    Regex_Free(&rx);
    return 0;
}