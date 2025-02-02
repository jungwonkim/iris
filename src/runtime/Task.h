#ifndef BRISBANE_SRC_RT_TASK_H
#define BRISBANE_SRC_RT_TASK_H

#include "Retainable.h"
#include "Command.h"
#include "Platform.h"
#include <pthread.h>
#include <vector>

#define BRISBANE_COMPLETE   0x0
#define BRISBANE_RUNNING    0x1
#define BRISBANE_SUBMITTED  0x2
#define BRISBANE_QUEUED     0x3
#define BRISBANE_NONE       0x4

#define BRISBANE_TASK       0x0
#define BRISBANE_TASK_PERM  0x1
#define BRISBANE_MARKER     0x2

namespace brisbane {
namespace rt {

class Scheduler;

class Task: public Retainable<struct _brisbane_task, Task> {
public:
  Task(Platform* platform, int type = BRISBANE_TASK, const char* name = NULL);
  virtual ~Task();

  void AddCommand(Command* cmd);
  void ClearCommands();

  void AddSubtask(Task* subtask);
  bool HasSubtasks();

  void AddDepend(Task* task);

  void Submit(int brs_policy, const char* opt, int sync);

  bool Dispatchable();
  bool Executable();
  void Complete();
  void Wait();
  int Ok();

  double TimeInc(double t);

  int type() { return type_; }
  char* name() { return name_; }
  void set_name(const char* name);
  bool perm() { return perm_; }
  bool user() { return user_; }
  bool system() { return system_; }
  void set_system() { system_ = true; }
  bool marker() { return type_ == BRISBANE_MARKER; }
  int status() { return status_; }
  Task* parent() { return parent_; }
  Command* cmd(int i) { return cmds_[i]; }
  Command* cmd_kernel() { return cmd_kernel_; }
  Command* cmd_last() { return cmd_last_; }
  void set_dev(Device* dev) { dev_ = dev; }
  Platform* platform() { return platform_; }
  Device* dev() { return dev_; }
  void set_devno(int devno) { devno_ = devno; }
  int devno() { return devno_; }
  int ncmds() { return ncmds_; }
  int ncmds_kernel();
  int ncmds_memcpy();
  void set_time_start(double d) { time_start_ = d; }
  void set_time_end(double d) { time_end_ = d; }
  double time() { return time_; }
  double time_start() { return time_start_; }
  double time_end() { return time_end_; }
  void set_parent(Task* task);
  void set_brs_policy(int brs_policy);
  void set_opt(const char* opt);
  void set_target_perm(int brs_policy, const char* opt);
  char* opt() { return opt_; }
  int brs_policy() { return brs_policy_; }
  int brs_policy_perm() { return brs_policy_perm_; }
  bool sync() { return sync_; }
  std::vector<Task*>* subtasks() { return &subtasks_; }
  Task* subtask(int i) { return subtasks_[i]; }
  bool is_subtask() { return parent_ != NULL; }
  int ndepends() { return ndepends_; }
  Task** depends() { return depends_; }
  Task* depend(int i) { return depends_[i]; }
  void* arch() { return arch_; }
  void set_arch(void* arch) { arch_ = arch; }

private:
  void CompleteSub();

private:
  char name_[64];
  bool given_name_;
  Task* parent_;
  int ncmds_;
  Command* cmds_[64];
  Command* cmd_kernel_;
  Command* cmd_last_;
  Device* dev_;
  int devno_;
  Platform* platform_;
  Scheduler* scheduler_;
  std::vector<Task*> subtasks_;
  size_t subtasks_complete_;
  void* arch_;

  Task** depends_;
  int depends_max_;
  int ndepends_;

  int brs_policy_;
  int brs_policy_perm_;
  char opt_[64];
  bool sync_;

  int type_;
  int status_;
  bool perm_;
  bool user_;
  bool system_;

  double time_;
  double time_start_;
  double time_end_;

  pthread_mutex_t mutex_executable_;
  pthread_mutex_t mutex_complete_;
  pthread_mutex_t mutex_subtasks_;
  pthread_cond_t complete_cond_;

public:
  static Task* Create(Platform* platform, int type, const char* name);
};

} /* namespace rt */
} /* namespace brisbane */

#endif /* BRISBANE_SRC_RT_TASK_H */
