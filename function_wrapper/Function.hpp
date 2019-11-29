/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Function.hpp
 * Author: Yash Patel
 *
 * Created on 20 November 2019, 09:20
 */

#ifndef FUNCTION_HPP
#define FUNCTION_HPP
#include <iostream>
#include<memory>
#include<assert.h>
#include <functional>
namespace cs540{


    
    
class BadFunctionCall{
    
};

template <typename> class Function{};

template <typename ResultType, typename ... ArgumentTypes>
class Function<ResultType(ArgumentTypes...)>{
    
    private:
        class ICallable {
	public:
		virtual ~ICallable() = default;
		virtual ResultType Invoke(ArgumentTypes...) = 0;
	};

	template <typename T>
	class CallableT : public ICallable {
	public:
		CallableT(const T& t)
			: t_(t) {
		}

		~CallableT() override = default;

		ResultType Invoke(ArgumentTypes... args) override {
			return t_(args...);
		}

	private:
		T t_;
	};
        //std::unique_ptr<ICallable> callable_;
	
        
    public:
    std::shared_ptr<ICallable> callable_;
    void (*fnptr)(ArgumentTypes...);
    BadFunctionCall badFunctionCall;
    
    Function &operator=(const Function &fn){
        callable_ = fn.callable_;
	return *this;
    }
    
    ResultType operator()(ArgumentTypes... ag){
        if(callable_==nullptr){
            throw badFunctionCall;
        }
        
        return callable_->Invoke(ag...);
    }
    
    
    
    
    Function(){
        //std::cout<<"default constructor\n";
        callable_=nullptr;
    }
    
    template <typename FunctionType>
    Function(FunctionType ft){
        //std::cout<<"constructiong through ft\n";
        callable_ = std::make_unique<CallableT<FunctionType>>(ft);
	
//        //cast the pointer to void(*) when storing
//        fnptr = (void(*)(ArgumentTypes...))&ft;
//        std::cout<<"res is"<<((ResultType(*)(ArgumentTypes...))(fnptr))();
    }
    
    Function(const Function & fn){
        //std::cout<<"calling ctor * to fn";
        if(callable_!=nullptr){
           // delete callable_;
            //std::cout<<"deleting callable\n";
        }
        callable_=fn.callable_;
    }
    
    
    
    explicit operator bool() const{
//     if(callable_==nullptr){
//         std::cout<<"BOOL returning false \n";
//            return false; 
//        }
//        std::cout<<"BOOL returning true \n";
//        return true;
        //even better
        return !(callable_ == NULL);
    }
    
};


// free standing functions
template <typename ResultType, typename... ArgumentTypes>
bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){ 
    if(f.callable_==NULL ){
        return true;
    }
    return false;
}

template <typename ResultType, typename... ArgumentTypes>
bool operator==( std::nullptr_t,const Function<ResultType(ArgumentTypes...)> &f){
    if(f.callable_==NULL ){
        return true;
    }
    return false;
}

template <typename ResultType, typename... ArgumentTypes>
bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
if(f.callable_!=NULL ){
        return true;
    }
    return false;

}
template <typename ResultType, typename... ArgumentTypes>
bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f){

if(f.callable_!=NULL ){
        return true;
    }
    return false;
}

}
#endif /* FUNCTION_HPP */

