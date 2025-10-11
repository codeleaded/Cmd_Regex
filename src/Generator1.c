#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Container/DataStream.h"

int main(){
    DataStream ds = DataStream_New();

    for(i32 i = 0;i<1000;i++){
        String email = String_New();

        const i32 namelen1 = Random_u32_MinMax(3,20);
        for(i32 i = 0;i<namelen1;i++){
            const char c1 = Random_u32_MinMax('a','z' + 1);
            const char c2 = Random_u32_MinMax('A','Z' + 1);
            
            const i32 des = Random_u32_MinMax(0,2);
            const char c = des == 0 ? c1 : c2;

            String_AppendChar(&email,c);
        }

        String_AppendChar(&email,'@');

        const i32 namelen2 = Random_u32_MinMax(3,20);
        for(i32 i = 0;i<namelen2;i++){
            const char c1 = Random_u32_MinMax('a','z' + 1);
            const char c2 = Random_u32_MinMax('A','Z' + 1);
            
            const i32 des = Random_u32_MinMax(0,2);
            const char c = des == 0 ? c1 : c2;

            String_AppendChar(&email,c);
        }

        String_AppendChar(&email,'.');

        char* ends[] = {
            "com","net","de"
        };
        
        const i32 des = Random_u32_MinMax(0,sizeof(ends) / sizeof(char*));
        String_Append(&email,ends[des]);

        String_AppendChar(&email,'\n');


        DataStream_PushCount(&ds,email.Memory,email.size * email.ELEMENT_SIZE);
        String_Free(&email);
    }

    Files_WriteT("./data/Emails.txt",ds.Memory,ds.size);
    DataStream_Free(&ds);


    return 0;
}