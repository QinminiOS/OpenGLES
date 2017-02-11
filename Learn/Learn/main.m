//
//  main.m
//  Learn
//
//  Created by qinmin on 2017/2/11.
//  Copyright © 2017年 qinmin. All rights reserved.
//

#import <Foundation/Foundation.h>

int i = 100;

int main(int argc, const char * argv[]) {
    
    int i = i;
    printf("i = %d \n", i);
    
    int arr[] = {11, 12, 13, 14, 15};
    int *ptr = arr;
    //*(ptr++) += 100;
    
    printf("%d\n", *(ptr++));
    printf("%d\n", *ptr);
    
   // printf("%d, %d\n", *ptr, *(++ptr));
    
    return 0;
}
