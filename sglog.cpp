// =====================================================================================
// 
//       Filename:  sglog.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2014-09-01 16时17分12秒
//       Revision:  none
// 
// =====================================================================================

#include "commlog.h"

int main(){
    int i = 0;
    while(i++ < 1000000){

        LOG_INIT("./");

        LOG("%s", "heheh");

    }
    return 0;
}
