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


namespace cs540{
    
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
class ptrdata{
    public:
    void* ptr;
    int refcount;
    void (*dest_ptr)(void*);
    // all constructors
    ptrdata():ptr(nullptr),refcount(0){};
    ptrdata(void* obj) : ptr(obj), refcount(1){};
    ptrdata(const void* obj) : ptr(const_cast<void*>(obj)), refcount(1){};
        
    ~ptrdata() {
	dest_ptr = NULL;
	ptr = NULL; 
    }
};


template <typename U>
void destruct(void* dobj){
    //std::cout<<"destructor\n";
    delete static_cast<U*>(dobj);
}

template<typename T>
class SharedPtr{
    
//    T* ptr;
//    size_t refcount=NULL;
    public:
       ptrdata *data;
       SharedPtr() : data(new ptrdata()){}
        // ctor on pointer to some object 
       template <typename U>
       explicit SharedPtr(U* u){
           pthread_mutex_lock(&lock);
            data = new ptrdata(u);
            //set dest_ptr fn pointer tp destruct
            data->dest_ptr = destruct<U>;
            pthread_mutex_unlock(&lock);
       }
      // template <typename U>
      // SharedPtr(ptrdata* spdtr, U* obj) : data(spdtr){}
       
        //Single refrence init-------------------------------
        //copy constructor for refrence to shardptr
        SharedPtr(const SharedPtr& p){
            pthread_mutex_lock(&lock);
            
            
            data = p.data;
            if(data->ptr != nullptr){ 
                ++data->refcount;
            }
            pthread_mutex_unlock(&lock);
        }

	template<typename U>
	SharedPtr(const SharedPtr<U>& p){
            pthread_mutex_lock(&lock);
            data = const_cast<ptrdata*>(p.data);
            if(data->ptr != nullptr){
                ++data->refcount;
            }
            pthread_mutex_unlock(&lock);
	}

        //double refrence init-----------------------------------
        // copy constructor for pointer to the shared object
        SharedPtr(SharedPtr&& p){
            //found a neat way to do it..haha
            pthread_mutex_lock(&lock);
            data = new ptrdata(std::move(*p.data));
            p.data = nullptr;
            pthread_mutex_unlock(&lock);
	}
        
	template <typename U>
	SharedPtr(SharedPtr<U>&& p){
            pthread_mutex_lock(&lock);
            data = new ptrdata(std::move(*p.data));
            p.data = nullptr;
            pthread_mutex_unlock(&lock);
	}
        
    template <typename U>
	SharedPtr(ptrdata* p, U* o) : data(p){}

        SharedPtr& operator=(const SharedPtr& p){
            //std::cout<<"in equality ctor\n";
            pthread_mutex_lock(&lock);
            //check self assignment
            // if(p==this){
            //     return *this;
            // }

            if(data->ptr != nullptr){ 
                --data->refcount;
            }
            data = p.data;
            if(data->ptr != nullptr){ 
                ++data->refcount;
            }
            pthread_mutex_unlock(&lock);
            return *this;
	}

	template <typename U>
	SharedPtr<T>& operator=(const SharedPtr<U>& p){
            pthread_mutex_lock(&lock);
            
            // if(p==this){
            //     return *this;
            // }
            
            //delete data->ptr;
            if(data->ptr != nullptr){ 
                --data->refcount;
            }
            //data->dest_ptr(data->ptr);
            data = p.data;
            if(data->ptr != nullptr){
                ++data->refcount;
            }
            pthread_mutex_unlock(&lock);
            return *this;
        }
        
        SharedPtr& operator=(SharedPtr&& p){
            pthread_mutex_lock(&lock);
            data = std::move(p.data);
            p.data->ptr = nullptr;
            p.data->refcount = 0;
            pthread_mutex_unlock(&lock);
            return *this;
	}

	template <typename U>
	SharedPtr<T>& operator=(SharedPtr<U>&& p){
            pthread_mutex_lock(&lock);
            //data->dest_ptr(data->ptr);
            data = std::move(p.data);
            p.data->ptr = nullptr;
            p.data->refcount = 0;
            pthread_mutex_unlock(&lock);
            return *this;
	}
        
        ~SharedPtr(){
            pthread_mutex_lock(&lock);
            if(data->ptr != nullptr && data->refcount != 0){
                //std::cout<<"here\n";
                    --data->refcount;
                    if(data->refcount <= 0){
                        //std::cout<<"deleting\n";
                        data->dest_ptr(data->ptr);
                    }
                    data->ptr = nullptr;
            }
            pthread_mutex_unlock(&lock);
	}
        
    void reset(){
        pthread_mutex_lock(&lock);
        if(data->ptr != nullptr){
		--data->refcount;
        }
        data->ptr = nullptr;
        pthread_mutex_unlock(&lock);
	    }

	template <typename U>
	void reset(U* p){
            pthread_mutex_lock(&lock);
            if(data->ptr != nullptr){
		--data->refcount;
		if(data->refcount <= 0) {
            data->dest_ptr(data->ptr);
        }
		data->ptr = nullptr;
		}
		data->ptr = p;
		data->refcount = 1;
		data->dest_ptr = destruct<U>;
        pthread_mutex_unlock(&lock);
	}
        
        
        T *get(){
            return static_cast<T*>(data->ptr);
        }
        
        int getrefrences(){
            return this->data->refcount;
        }
        
        T& operator*() const{
            return *static_cast<T*>(data->ptr);
        }
	T* operator->() const{ 
            return static_cast<T*>(data->ptr);
        }
        
        explicit operator bool() const{
            return data->ptr != nullptr;
        }
        
        
        
        
};
//free standing finctions

template <typename T1, typename T2>
bool operator==(const SharedPtr<T1>& p1, const SharedPtr<T2>& p2){
	return p1.data->ptr == p2.data->ptr;
}
	
template <typename T1>
bool operator==(const SharedPtr<T1>& p, std::nullptr_t np){
	return p.data->ptr == np;
}

template <typename T1>
bool operator==(std::nullptr_t np, const SharedPtr<T1>& p){
	return p == np;
}

template <typename T1, typename T2>
bool operator!=(const SharedPtr<T1>& p1, const SharedPtr<T2>& p2){
	return !(p1 == p2);
}

template <typename T1>
bool operator!=(const SharedPtr<T1>& p, std::nullptr_t np){
	return !(p == np);
}

template <typename T1>
bool operator!=(std::nullptr_t np, const SharedPtr<T1>& p){
	return p != np;
}

template <typename T1, typename U>
SharedPtr<T1> static_pointer_cast(const SharedPtr<U>& p){
    T1* new_ptr = static_cast<T1*>(p.data->ptr);
    return SharedPtr<T1>(p.data, new_ptr);
}

template <typename T1, typename U>
SharedPtr<T1> dynamic_pointer_cast(const SharedPtr<U>& p){
    auto new_ptr = dynamic_cast<T1*>(static_cast<U*>(p.data->ptr));
    if(new_ptr){ 
        return SharedPtr<T1>(p.data, new_ptr);
    }
    return SharedPtr<T1>();
}


}
#endif /* SMARTPOINTER_HPP */

