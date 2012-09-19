#ifndef RUNNABLE_H
#define RUNNABLE_H

namespace rollercoaster{

  /**
   *Base class for runnable threadable objects
   *All derived classes must implement a run method
   *which will be executed by the thread
   *As a parameter they should take copy of themselves
   * Note: all implementing classes must be copy safe
   */

  class Runnable{

  public:
    virtual void run() = 0;
    virtual ~Runnable(){}

  };



}//namespace rollercoaster

#endif
