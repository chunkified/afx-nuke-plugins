// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// Copyright (C) 2012-2016, Ryan P. Wilson
//
//      Authority FX, Inc.
//      www.authorityfx.com

#ifndef INCLUDE_THREADING_H_
#define INCLUDE_THREADING_H_

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include "include/settings.h"
#include "include/bounds.h"

namespace afx {

class ThreaderBase {
 private:
  boost::asio::io_service io_service_;
  boost::thread_group thread_pool_;
  boost::scoped_ptr<boost::asio::io_service::work> work_ptr_;
  unsigned int num_threads_;

  boost::condition_variable synchronized_;
  boost::condition_variable ready_condition_;
  bool running_;

  boost::mutex mutex_;

  // This is the function that is run in each thread
  void Worker_();

 public:
   ThreaderBase();
   explicit ThreaderBase(unsigned int req_num_threads);
  ~ThreaderBase();
  // Launch additional threads.
  void AddThreads(unsigned int num_threads);
  // Start asio service. Launch req num of threads. 0 will launch hardware concurency
  void InitializeThreads(unsigned int req_num_threads = 0);
  // Poll until all work is submitted. Stop asio service. Block until threads have completed work. Restart asio service
  void Synchonize();
  // Synchonize and join all threads.
  void StopAndJoin();
  void AddWork(boost::function<void()> function);
  unsigned int Threads() const;
};

class ImageThreader : public ThreaderBase {
 public:
  // Split bounds into num of rows.
  void ThreadImageRows(const Bounds& region, boost::function<void(Bounds)> function);
  // Split Bounds into num_threads chunks in y axis.
  void ThreadImageChunks(const Bounds& region, boost::function<void(Bounds)> function);
  // Split Bounds into num_threads chunks in y axis.
  void ThreadImageChunksY(const Bounds& region, boost::function<void(Bounds)> function);
};

}  // namespace afx

#endif  // INCLUDE_THREADING_H_
