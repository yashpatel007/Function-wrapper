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
namespace cs540{
class Array{

    public:
        MyInt* data;
        int length;
        //std:: init list ctor
        Array(std::initializer_list<MyInt> list){
            
            // allocate the memory
            length = (list.size());
            
            data =(MyInt*)(malloc(sizeof(MyInt)*length));
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
            data =(MyInt*)(malloc(sizeof(MyInt)*length));     
            //memcpy(data,arr.data,sizeof(MyInt[length]));
            for(int i=0; i<length;i++){
                data[i]=(MyInt)arr.data[i];
            }
           
        }
        //move assignment cunstructor
        Array(Array && arr){
            if(arr.data!=nullptr){
                //delete data;
                data=arr.data;
                length=arr.length;
                arr.data=nullptr;
            }
        }
        ~Array(){
            
            delete[] data;
        }
        //COPY ASSIGNMENT OPERATOR
        Array& operator=(const Array& arr){
            
            // Self-assignment detection
            if (&arr == this){
                return *this;
            }
 
            // Release any resource we're holding
            delete data;
            
            length=arr.length;
            data =(MyInt*)(malloc(sizeof(MyInt)*length));     
            //memcpy(data,arr.data,sizeof(MyInt[length]));
            for(int i=0; i<length;i++){
                data[i]=(MyInt)arr.data[i];
            }
            return *this;
        
        }
        
        Array& operator=(Array&& arr){
            
            if(&arr ==this){
                return *this;
            }
            delete data;
            if(arr.data!=nullptr){
                length=arr.length;
                data=arr.data;
                arr.data=nullptr;
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

}// namespace ends here
#endif /* ARRAY_HPP */
