// SPDX-License-Identifier: Apache-2.0
#include "eeimodule.h"
#include "evmc/evmc.h"
#include "evmc/utils.h"
#include "common/hexstr.h"
#include "common/log.h"
//#include "vm/configure.h"
#include "vm/vm.h"

namespace {

static bool isWasmBinary(std::vector<uint8_t> &Code) {
  return Code.size() >= 8 && Code[0] == 0 && Code[1] == 'a' && Code[2] == 's' &&
         Code[3] == 'm';
}

static evmc_capabilities_flagset get_capabilities(struct evmc_vm *vm) {
  return EVMC_CAPABILITY_EWASM;
}

static void destroy(struct evmc_vm *vm) { delete vm; }

static void release(const struct evmc_result *result) {
  if (result->output_data != nullptr) {
    delete[] result->output_data;
  }
  return;
}

static struct evmc_result
execute(struct evmc_vm *instance, const struct evmc_host_interface *host,
        struct evmc_host_context *context, enum evmc_revision rev,
        const struct evmc_message *msg, const uint8_t *code, size_t code_size) {
  SSVM::Log::setErrorLoggingLevel();
  // Prepare EVMC result
  struct evmc_result result;
  result.status_code = EVMC_SUCCESS;
  result.gas_left = msg->gas;
  result.output_size = 0;
  result.output_data = nullptr;
  result.release = ::release;
  result.create_address = {};

  /// Create VM with ewasm configuration.
  SSVM::Configure Conf;
  SSVM::VM::VM EVM(Conf);
  SSVM::Statistics::Statistics &Measure = EVM.getStatistics();
  uint64_t costLimit = Measure.getCostLimit();
  uint64_t totalCost = Measure.getTotalCost();
  SSVM::Host::EEIModule EEIMod(costLimit, totalCost,
                               host, context);
  Measure.setCostTable(std::vector<uint64_t>());
  EVM.registerModule(EEIMod);

  /// Set data from message.
  std::vector<uint8_t> Code(code, code + code_size);
  SSVM::Host::EVMEnvironment &EEIEnv = EEIMod.getEnv();
  EEIEnv.setEVMCMessage(msg);
  EEIEnv.setEVMCCode(code, code_size);
  if (msg->input_size > 0) {
    EEIEnv.getCallData() = std::vector<uint8_t>(
        msg->input_data, msg->input_data + msg->input_size);
  }
  EVM.getStatistics().setCostLimit(msg->gas);

  /// Debug log.
  LOG(DEBUG) << "msg->gas: " << msg->gas;
  LOG(DEBUG) << "msg->depth: " << msg->depth;
  LOG(DEBUG) << "msg->input_size: " << msg->input_size;
  LOG(DEBUG) << "Caller: " << EEIEnv.getCallerStr();
  LOG(DEBUG) << "CallValue: " << EEIEnv.getCallValueStr();

  /// Load, validate, and instantiate code.
  if (result.status_code == EVMC_SUCCESS && !EVM.loadWasm(Code)) {
    result.status_code = EVMC_FAILURE;
  }
  if (result.status_code == EVMC_SUCCESS && !EVM.validate()) {
    result.status_code = EVMC_FAILURE;
  }
  if (result.status_code == EVMC_SUCCESS && !EVM.instantiate()) {
    result.status_code = EVMC_FAILURE;
  }

  /// Checking for errors.
  auto &Store = EVM.getStoreManager();
  if (result.status_code == EVMC_SUCCESS && Store.getMemExports().size() == 0) {
    /// Memory exports not found
    result.status_code = EVMC_FAILURE;
  }
  if (result.status_code == EVMC_SUCCESS &&
      (*Store.getActiveModule())->getStartAddr()) {
    /// Module contains start function
    result.status_code = EVMC_FAILURE;
  }

  /// Execute.
  if (result.status_code == EVMC_SUCCESS) {
    if (auto Res = EVM.execute("main")) {
      if (EEIEnv.getIsRevert()) {
        result.status_code = EVMC_REVERT;
      }
    } else {
      result.status_code = EVMC_FAILURE;
    }
  }

  /// Get execution results.
  uint64_t usedGas = EVM.getStatistics().getTotalCost();
  std::vector<uint8_t> &ReturnData = EEIEnv.getReturnData();

  /// Verify the deployed code.
  if (isWasmBinary(ReturnData) && msg->kind == EVMC_CREATE &&
      result.status_code != EVMC_REVERT) {
    SSVM::Loader::Loader WasmLoader(Conf);
    if (auto Res = WasmLoader.parseModule(ReturnData)) {
      const SSVM::AST::StartSection &StartSec = (*Res)->getStartSection();
      if (!StartSec.getContent()) {
        result.status_code = EVMC_FAILURE;
      }
    } else {
      result.status_code = EVMC_FAILURE;
    }
  }

  /// Copy return data and left gas.
  if (ReturnData.size() > 0) {
    uint8_t *outputData = new uint8_t[ReturnData.size()];
    std::copy(ReturnData.begin(), ReturnData.end(), outputData);
    result.output_size = ReturnData.size();
    result.output_data = outputData;
  }
  result.gas_left =
      (result.status_code == EVMC_FAILURE) ? 0 : msg->gas - usedGas;

  /// Debug log.
  LOG(DEBUG) << "gas_left: " << result.gas_left;
  LOG(DEBUG) << "output_size: " << result.output_size;

  return result;
}

} // namespace

extern "C" EVMC_EXPORT struct evmc_vm *evmc_create() EVMC_NOEXCEPT {
  struct evmc_vm *VM = new struct evmc_vm({
      EVMC_ABI_VERSION,
      "ssvm",
      "0.6.3",
      ::destroy, // destroy
      ::execute, // execute
      ::get_capabilities,
      nullptr,
  });

  return VM;
}
