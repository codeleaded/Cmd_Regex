#include "./YanisWindow.h"

void Setup(YanisWindow* yw){
    YanisWindow_Clear(yw,0xFF000000);
}
void Update(YanisWindow* yw){
    //memset(yw->Buffer,0,sizeof(unsigned int) * yw->Width * yw->Height);
    //YanisWindow_Clear(yw,0xFF00FF00);

    if(yw->Strokes[ALX_MOUSE_L].PRESSED)
        yw->Buffer[yw->MouseY * yw->Width + yw->MouseX] = 0xFFFFFFFF;
}
void Free(YanisWindow* yw){

}

int main(){
    YanisWindow yw;
    YanisWindow_Init(&yw,"Hello World",800,600,Setup,Update,Free);
    YanisWindow_Start(&yw);
    YanisWindow_Free(&yw);
    return 0;
}