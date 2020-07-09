// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "eeienv.h"
#include "runtime/importobj.h"

#include <cstdint>

namespace SSVM {
namespace Host {

class EEIModule : public Runtime::ImportObject {
public:
  EEIModule() = delete;
  EEIModule(uint64_t &CostLimit, uint64_t &CostSum,
            const evmc_host_interface *IHost, evmc_host_context *Cxt);

  EVMEnvironment &getEnv() { return Env; }

private:
  EVMEnvironment Env;
};

} // namespace Host
} // namespace SSVM
