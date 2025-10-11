#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/Regex.h"

int main(){
    //Regex rx = Regex_Make("a \"@\" a \".\" a");
    //Regex rx = Regex_Make("[a-z,A-Z]+ \"@\" [a-z,A-Z]+ \".\" [com,net,de]");
    //Regex rx = Regex_Make("[A-D,P-Z]*E");
    //Regex rx = Regex_Make("[A-D,P-Z]{1,2}E");
    //Regex rx = Regex_Make("[c,C][o,O][l,L][o,O]\"u\"?[r,R]");
    //Regex rx = Regex_Make("(Hallo,\" \",Welt,\' \'-z?)+");
    Regex rx = Regex_Make("^(Hallo,\" \",Welt)+$");

    //Regex_Do(&rx,"AE BBE CZCE ALE PPE WWRT ABCE ABCDE");
    //Regex_Do(&rx,"Color color coluor colour colorucolouru");
    //Regex_Do(&rx,"Hallo Welt Hallo WeltHallo Welt  Hallo Welt Hallo  Welt Hallo Welt  Hallo Welt");
    Regex_Do(&rx,"Hallo Welt\nHallo WeltHallo WeltHallo Welt\nHallo Welt Hallo Welt\nHallo Welt\nHallo Welt");
    
    //Regex_DoFile(&rx,"./data/Cartag.txt");
    //Regex_DoFile(&rx,"./data/Emails.txt");

    Regex_Print(&rx);
    Regex_Free(&rx);
    return 0;
}