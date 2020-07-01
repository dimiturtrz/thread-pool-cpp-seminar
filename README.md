# thread-pool-cpp-seminar

should be C++ implementation of a thread pool with some api for parallel executions and profiling 

TODO:

high priority:
 - profiling data for processor modes
 
medium priority:
 - should the thread pool accept different function types?
 - completion handlers after work is done
 - vector parallelism (no template argument, copy -> move, better passing)
 
low priority:
 - are lambdas really fine?
 - task parallelism by tags
 - memory data in profiling
 
done/check again priority:
 - threadPool queue synchronization (see if something could be done better)
 - copy -> move (do more tests if the moving is done correctly)
