#ifndef BRISBANE_SRC_RT_PLATFORM_H
#define BRISBANE_SRC_RT_PLATFORM_H

#include <iris/brisbane.h>
#include <pthread.h>
#include <stddef.h>
#include <set>
#include <map>
#include <mutex>
#include <string>
#include "Config.h"

namespace brisbane {
namespace rt {

class Device;
class Filter;
class Graph;
class JSON;
class Kernel;
class LoaderCUDA;
class LoaderHIP;
class LoaderLevelZero;
class LoaderOpenCL;
class LoaderOpenMP;
class LoaderHexagon;
class Mem;
class Polyhedral;
class Pool;
class PresentTable;
class Profiler;
class Queue;
class Scheduler;
class SigHandler;
class Task;
class Timer;
class Worker;

class Platform {
private:
  Platform();
  ~Platform();

public:
  int Init(int* argc, char*** argv, int sync);
  int Finalize();
  int Synchronize();

  int EnvironmentInit();
  int EnvironmentSet(const char* key, const char* value, bool overwrite);
  int EnvironmentGet(const char* key, char** value, size_t* vallen);

  int PlatformCount(int* nplatforms);
  int PlatformInfo(int platform, int param, void* value, size_t* size);
  int PlatformBuildProgram(int model, char* path);

  int DeviceCount(int* ndevs);
  int DeviceInfo(int device, int param, void* value, size_t* size);
  int DeviceSetDefault(int device);
  int DeviceGetDefault(int* device);
  int DeviceSynchronize(int ndevs, int* devices);

  int PolicyRegister(const char* lib, const char* name, void* params);
  int RegisterCommand(int tag, int device, command_handler handler);
  int RegisterHooksTask(hook_task pre, hook_task post);
  int RegisterHooksCommand(hook_command pre, hook_command post);

  int KernelCreate(const char* name, brisbane_kernel* brs_kernel);
  int KernelGet(const char* name, brisbane_kernel* brs_kernel);
  int KernelSetArg(brisbane_kernel brs_kernel, int idx, size_t size, void* value);
  int KernelSetMem(brisbane_kernel brs_kernel, int idx, brisbane_mem mem, size_t off, size_t mode);
  int KernelSetMap(brisbane_kernel brs_kernel, int idx, void* host, size_t mode);
  int KernelRelease(brisbane_kernel brs_kernel);

  int TaskCreate(const char* name, bool perm, brisbane_task* brs_task);
  int TaskDepend(brisbane_task brs_task, int ntasks, brisbane_task* brs_tasks);
  int TaskKernel(brisbane_task brs_task, brisbane_kernel brs_kernel, int dim, size_t* off, size_t* gws, size_t* lws);
  int TaskKernel(brisbane_task task, const char* kernel, int dim, size_t* off, size_t* gws, size_t* lws, int nparams, void** params, size_t* params_off, int* params_info, size_t* memranges);
  int TaskKernelSelector(brisbane_task task, brisbane_selector_kernel func, void* params, size_t params_size);
  int TaskHost(brisbane_task task, brisbane_host_task func, void* params);
  int TaskCustom(brisbane_task task, int tag, void* params, size_t params_size);
  int TaskMalloc(brisbane_task brs_task, brisbane_mem brs_mem);
  int TaskH2D(brisbane_task brs_task, brisbane_mem brs_mem, size_t off, size_t size, void* host);
  int TaskD2H(brisbane_task brs_task, brisbane_mem brs_mem, size_t off, size_t size, void* host);
  int TaskH2DFull(brisbane_task brs_task, brisbane_mem brs_mem, void* host);
  int TaskD2HFull(brisbane_task brs_task, brisbane_mem brs_mem, void* host);
  int TaskMap(brisbane_task brs_task, void* host, size_t size);
  int TaskMapTo(brisbane_task brs_task, void* host, size_t size);
  int TaskMapToFull(brisbane_task brs_task, void* host);
  int TaskMapFrom(brisbane_task brs_task, void* host, size_t size);
  int TaskMapFromFull(brisbane_task brs_task, void* host);
  int TaskSubmit(brisbane_task brs_task, int brs_policy, const char* opt, int wait);
  int TaskWait(brisbane_task brs_task);
  int TaskWaitAll(int ntasks, brisbane_task* brs_tasks);
  int TaskAddSubtask(brisbane_task brs_task, brisbane_task brs_subtask);
  int TaskKernelCmdOnly(brisbane_task brs_task);
  int TaskRelease(brisbane_task brs_task);
  int TaskReleaseMem(brisbane_task brs_task, brisbane_mem brs_mem);
  int TaskInfo(brisbane_task brs_task, int param, void* value, size_t* size);

  int MemCreate(size_t size, brisbane_mem* brs_mem);
  int MemArch(brisbane_mem brs_mem, int device, void** arch);
  int MemMap(void* host, size_t size);
  int MemUnmap(void* host);
  int MemReduce(brisbane_mem brs_mem, int mode, int type);
  int MemRelease(brisbane_mem brs_mem);

  int GraphCreate(brisbane_graph* brs_graph);
  int GraphCreateJSON(const char* json, void** params,  brisbane_graph* brs_graph);
  int GraphTask(brisbane_graph brs_graph, brisbane_task brs_task, int brs_policy, const char* opt);
  int GraphSubmit(brisbane_graph brs_graph, int brs_policy, int sync);
  int GraphWait(brisbane_graph brs_graph);
  int GraphWaitAll(int ngraphs, brisbane_graph* brs_graphs);

  int RecordStart();
  int RecordStop();

  int TimerNow(double* time);

  int ndevs() { return ndevs_; }
  int device_default() { return dev_default_; }
  Device** devices() { return devs_; }
  Device* device(int devno) { return devs_[devno]; }
  Polyhedral* polyhedral() { return polyhedral_; }
  Worker** workers() { return workers_; }
  Worker* worker(int i) { return workers_[i]; }
  Queue* queue() { return queue_; }
  Pool* pool() { return pool_; }
  Scheduler* scheduler() { return scheduler_; }
  Timer* timer() { return timer_; }
  Kernel* null_kernel() { return null_kernel_; }
  char* app() { return app_; }
  char* host() { return host_; }
  Profiler** profilers() { return profilers_; }
  int nprofilers() { return nprofilers_; }
  double time_app() { return time_app_; }
  double time_init() { return time_init_; }
  bool enable_profiler() { return enable_profiler_; }
  hook_task hook_task_pre() { return hook_task_pre_; }
  hook_task hook_task_post() { return hook_task_post_; }
  hook_command hook_command_pre() { return hook_command_pre_; }
  hook_command hook_command_post() { return hook_command_post_; }

  Kernel* GetKernel(const char* name);
  Mem* GetMem(brisbane_mem brs_mem);
  Mem* GetMem(void* host, size_t* off);

private:
  int SetDevsAvailable();
  int InitCUDA();
  int InitHIP();
  int InitLevelZero();
  int InitOpenCL();
  int InitOpenMP();
  int InitHexagon();
  int InitDevices(bool sync);
  int InitScheduler();
  int InitWorkers();
  int FilterSubmitExecute(Task* task);
  int ShowKernelHistory();

public:
  static Platform* GetPlatform();

private:
  bool init_;
  bool finalize_;

  char platform_names_[BRISBANE_MAX_NPLATFORMS][64];
  int nplatforms_;
  Device* devs_[BRISBANE_MAX_NDEVS];
  int ndevs_;
  int dev_default_;
  int devs_enabled_[BRISBANE_MAX_NDEVS];
  int ndevs_enabled_;

  LoaderCUDA* loaderCUDA_;
  LoaderHIP* loaderHIP_;
  LoaderLevelZero* loaderLevelZero_;
  LoaderOpenCL* loaderOpenCL_;
  LoaderOpenMP* loaderOpenMP_;
  LoaderHexagon* loaderHexagon_;
  size_t arch_available_;

  Queue* queue_;

  std::set<Kernel*> kernels_;
  std::set<Mem*> mems_;
  std::map<std::string, std::string> env_;

  PresentTable* present_table_;
  Pool* pool_;

  Worker* workers_[BRISBANE_MAX_NDEVS];

  Scheduler* scheduler_;
  Timer* timer_;
  Polyhedral* polyhedral_;
  bool polyhedral_available_;
  Filter* filter_task_split_;
  SigHandler* sig_handler_;
  JSON* json_;

  bool recording_;

  bool enable_profiler_;
  Profiler* profilers_[8];
  int nprofilers_;

  Kernel* null_kernel_;

  pthread_mutex_t mutex_;
  hook_task hook_task_pre_;
  hook_task hook_task_post_;
  hook_command hook_command_pre_;
  hook_command hook_command_post_;

  char app_[256];
  char host_[256];
  double time_app_;
  double time_init_;

private:
  static Platform* singleton_;
  static std::once_flag flag_singleton_;
  static std::once_flag flag_finalize_;
};

} /* namespace rt */
} /* namespace brisbane */

#endif /* BRISBANE_SRC_RT_PLATFORM_H */
