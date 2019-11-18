/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   smartpointer.hpp
 * Author: Yash Patel
 *
 * Created on 8 November 2019, 12:53
 */

#ifndef SMARTPOINTER_HPP
#define SMARTPOINTER_HPP
#include<cstddef>
#include<iostream>


template<typename T>
class ptrdata{
    public:
    T* ptr;
    int refcount;
    // all constructors
    ptrdata():ptr(nullptr),refcount(0){};
    ptrdata(T* obj) : ptr(obj), refcount(1){};
    ptrdata(const T* obj) : ptr(const_cast<T*>(obj)), refcount(1){};
    
};


template<typename T>
class SharedPtr{
    
//    T* ptr;
//    size_t refcount=NULL;
    public:
       ptrdata<T> *data;
       SharedPtr() : data(new ptrdata<T>()){}
        // ctor on pointer to some object 
       template <typename U>
       explicit SharedPtr(U* u){
            data = new ptrdata<T>(u);
           
       }
//       template <typename U>
//       SharedPtr(ptrdata* spdtr, U* obj) : data(spdtr){}
       
        //Single refrence init-------------------------------
        //copy constructor for refrence to shardptr
        SharedPtr(const SharedPtr& p){
            
            data = p.data;
            if(data->ptr != nullptr) ++data->refcount;
        }

	template<typename U>
	SharedPtr(const SharedPtr<U>& p){
            data = const_cast<ptrdata<T>*>(p.data);
            if(data->ptr != nullptr) ++data->refcount;
	}

        //double refrence init-----------------------------------
        // copy constructor for pointer to the shared object
        SharedPtr(SharedPtr&& p){
            //found a neat way to do it..haha
            data = new ptrdata<T>(std::move(*p.data));
            p.data = nullptr;
	}
        
	template <typename U>
	SharedPtr(SharedPtr<U>&& p){
            data = new ptrdata<T>(std::move(*p.data));
            p.data = nullptr;
	}

        SharedPtr& operator=(const SharedPtr& p){
            std::cout<<"in equality ctor\n";
            if(data->ptr != nullptr){ 
                --data->refcount;
            }
            data = p.data;
            if(data->ptr != nullptr){ 
                ++data->refcount;
            }
            return *this;
	}

	template <typename U>
	SharedPtr<T>& operator=(const SharedPtr<U>& p){
            if(data->ptr != nullptr){ 
                --data->refcount;
            }
            data = p.data;
            if(data->ptr != nullptr){
                ++data->refcount;
            }
            return *this;
        }
        
        SharedPtr& operator=(SharedPtr&& p){
				
            data = std::move(p.data);
            p.data->ptr = nullptr;
            p.data->refcount = 0;
            return *this;
	}

	template <typename U>
	SharedPtr<T>& operator=(SharedPtr<U>&& p){
				
            data = std::move(p.data);
            p.data->ptr = nullptr;
            p.data->refcount = 0;
            return *this;
	}
        
        void reset(){
            if(data->ptr != nullptr)
		--data->refcount;
		data->ptr = nullptr;
	}

	template <typename U>
	void reset(U* p){
            if(data->ptr != nullptr){
		--data->refcount;
		
		data->ptr = nullptr;
		}
		data->ptr = p;
		data->refcount = 1;
			
	}
        
        
        
        ptrdata<T> *get_data(){
            return this->data;
        }
        
        int getrefrences(){
            return this->data->refcount;
        }
        
        T& operator*()const{
        
        }
};
//free standing finctions
#endif /* SMARTPOINTER_HPP */

