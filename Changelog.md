### 0.1.1 (2020-06-24)

Refactor:

* Update host function signature.
  * Use memory instance pointer rather than reference due to API change of host function base class.
* Remove `revert` error code.
  * Return as terminated and handle revert event in EEI environment.


### 0.1.0 (2020-05-20)

Features:

* Ethereum environment interface
  * Moved from [SSVM](https://github.com/second-state/SSVM) project.
  * Implemented all EEI functions in a host module.

Tools:

* SSVM-EVMC
  * SSVM-EVMC integrates EVMC and Ethereum Environment Interface(EEI).
  * SSVM-EVMC is a shared library for EVMC-compatible clients.

Tests:

* ERC20 contracts for SSVM-EVMC
  * Create an example VM for testing.
  * Test the following functionalities of ERC20 contracts:
    * Deploy ERC20 contract
    * Check balance
    * Check total supply
    * Transfer
    * Approve
    * Check allowance