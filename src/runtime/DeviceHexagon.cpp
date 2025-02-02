#include "DeviceHexagon.h"
#include "Debug.h"
#include "Kernel.h"
#include "LoaderHexagon.h"
#include "Mem.h"
#include "Task.h"
#include "Utils.h"
#include <iris/hexagon/rpcmem.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdlib.h>

namespace brisbane {
namespace rt {

DeviceHexagon::DeviceHexagon(LoaderHexagon* ld, int devno, int platform) : Device(devno, platform) {
  ld_ = ld;
  type_ = brisbane_hexagon;
  model_ = brisbane_hexagon;
  strcpy(name_, "Hexagon DSP");
  _info("device[%d] platform[%d] device[%s] type[%d]", devno_, platform_, name_, type_);
}

DeviceHexagon::~DeviceHexagon() {
  ld_->brisbane_hexagon_finalize();
}

int DeviceHexagon::Init() {
  ld_->brisbane_hexagon_init();
  return BRISBANE_OK;
}

int DeviceHexagon::MemAlloc(void** mem, size_t size) {
  void** hxgmem = mem;
  *hxgmem = ld_->brisbane_hexagon_rpcmem_alloc(RPCMEM_HEAP_ID_SYSTEM, RPCMEM_DEFAULT_FLAGS, (int) size);
  if (*hxgmem == 0) {
    _error("hxgmem[%p]", hxgmem);
    return BRISBANE_ERR;
  }
  return BRISBANE_OK;
}

int DeviceHexagon::MemFree(void* mem) {
  void* hxgmem = mem;
  if (hxgmem) ld_->brisbane_hexagon_rpcmem_free(hxgmem);
  return BRISBANE_OK;
}

int DeviceHexagon::MemH2D(Mem* mem, size_t off, size_t size, void* host) {
  void* hxgmem = mem->arch(this);
  memcpy((char*) hxgmem + off, host, size);
  return BRISBANE_OK;
}

int DeviceHexagon::MemD2H(Mem* mem, size_t off, size_t size, void* host) {
  void* hxgmem = mem->arch(this);
  memcpy(host, (char*) hxgmem + off, size);
  return BRISBANE_OK;
}

int DeviceHexagon::KernelGet(void** kernel, const char* name) {
  return BRISBANE_OK;
}

int DeviceHexagon::KernelLaunchInit(Kernel* kernel) {
  return ld_->brisbane_hexagon_kernel(kernel->name());
}

int DeviceHexagon::KernelSetArg(Kernel* kernel, int idx, size_t size, void* value) {
  return ld_->brisbane_hexagon_setarg(idx, size, value);
}

int DeviceHexagon::KernelSetMem(Kernel* kernel, int idx, Mem* mem, size_t off) {
  void* hxgmem = mem->arch(this);
  return ld_->brisbane_hexagon_setmem(idx, hxgmem, (int) mem->size());
}

int DeviceHexagon::KernelLaunch(Kernel* kernel, int dim, size_t* off, size_t* gws, size_t* lws) {
  _trace("kernel[%s] dim[%d] off[%zu] gws[%zu]", kernel->name(), dim, off[0], gws[0]);
  return ld_->brisbane_hexagon_launch(dim, off[0], gws[0]);
}

int DeviceHexagon::Synchronize() {
  return BRISBANE_OK;
}

int DeviceHexagon::AddCallback(Task* task) {
  task->Complete();
  return BRISBANE_OK;
}

} /* namespace rt */
} /* namespace brisbane */

