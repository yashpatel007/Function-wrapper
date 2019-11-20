/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Array.hpp
 * Author: Yash Patel
 *
 * Created on 19 November 2019, 09:49
 */

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <stdlib.h>
#include <cstring>

class Array{

    public:
        MyInt* data;
        size_t length;
        //std:: init list ctor
        Array(std::initializer_list<MyInt> list){
            
            // allocate the memory
            length = sizeof(list.size())-1;
            
            data =(MyInt*)(malloc(sizeof(MyInt[length])));
            //copy the contents
            int k=0;
            for(auto i=list.begin(); i!=list.end(); i++){
                data[k]=*i;
                k++;
            }
        }
        
        // copy constructor
        Array(const Array& arr){
            length=arr.length;
            data =(MyInt*)(malloc(sizeof(MyInt[length])));     
            //memcpy(data,arr.data,sizeof(MyInt[length]));
            for(int i=0; i<length;i++){
                data[i]=(MyInt)arr.data[i];
            }
           
        }
        //move assignment cunstructor
        Array(const Array && arr){
            if(arr.data!=nullptr){
                data=arr.data;
                length=arr.length;
                //arr.data=nullptr;
            }
        }
        //COPY ASSIGNMENT OPERATOR
        Array& operator=(const Array& arr){
            length=arr.length;
            data =(MyInt*)(malloc(sizeof(MyInt[length])));     
            //memcpy(data,arr.data,sizeof(MyInt[length]));
            for(int i=0; i<length;i++){
                data[i]=(MyInt)arr.data[i];
            }
            return *this;
        
        }
        
        Array& operator=(const Array&& arr){
            
            if(arr.data!=nullptr){
                length=arr.length;
                data=arr.data;
            }
            return *this;
        }
        friend std::ostream &operator<<(std::ostream &os, const Array &arr) {
             
            for (int i = 0; i < arr.length; ++i) { 
                os << arr.data[i]; 
                if (i != arr.length - 1){ 
                    os << ", "; 
                }
            }  
            return os; 
              
        }
        
        
};
#endif /* ARRAY_HPP */
