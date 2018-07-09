//
// Created by capitalg on 2018/7/9.
//

#include "get_unique_id.h"
int get_unique_id(){
    static int i = 1;
    return i++;
}
