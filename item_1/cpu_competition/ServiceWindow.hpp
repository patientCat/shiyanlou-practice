//c++ program
//design by >^.^<
//@date    2018-10-11 15:41:09
#ifndef _SERVICEWINDOW_HPP_
#define _SERVICEWINDOW_HPP_

#include "Node.hpp" 

using WindowStatus = enum{ SERVICE, IDLE};
//typedef enum{SERVICE , IDLE} WS;

class ServiceWindow 
{ 
public: 
    ServiceWindow ()
    : _window_status(IDLE) {} 
    ~ServiceWindow () = default;
     
    bool isIdle() const{    return _window_status == IDLE ? true : false;   }
    void serveCustomer(Customer &customer)  { _customer = customer; }
    void setBusy()  {   _window_status = SERVICE;   }
    void setIdle()  {   _window_status = IDLE;   }
    int getCustomerArriveTime() const   {   return _customer.arrive_time;   }
    int getCustomerDuration() const {   return _customer.duration;      }
private: 
    Customer            _customer;
    WindowStatus        _window_status;
}; 

#endif /* ServiceWindow_hpp */
