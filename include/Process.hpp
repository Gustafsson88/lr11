// Copyright 2021 Danil Postvaykin <postvaykin01@mail.ru>

#ifndef INCLUDE_PROCESS_HPP_
#define INCLUDE_PROCESS_HPP_

#include <iostream>
#include <string>

#include <thread>
#include <async++.h>
#include <boost/process.hpp>
#include <boost/program_options.hpp>

using namespace boost::process;
using namespace boost::program_options;

class Process
{
 public:
  Process();
  ~Process();
  void start_process(bool _install, bool _pack, std::string _config,
                     int _time);
  void timer(int time);
  bool my_task(std::string task);
  void out_help();
 private:
  child new_child;
};
#endif // INCLUDE_PROCESS_HPP_
