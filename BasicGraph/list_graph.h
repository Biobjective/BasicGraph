#pragma once
#ifndef LIST_H_
#define LIST_H_
template<typename T,typename E>
struct listpoint
{
    T data;//节点值  如无值则与编号相同
    E value;//边权重
    listpoint<T,E>* next;
    listpoint(T cost1=0,E cost2 = 0):data(cost1),value(cost2),next(nullptr){}
    bool operator != (listpoint<T,E>& R)const
    {
        return (data != R.data) ? true : false;
    }
};


#endif