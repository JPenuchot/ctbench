# Useful configs

Useful configs to break down time-trace data.

## `feature_comparison.json`

Generates comparison graphs for every major compile pass, namely:

* `ExecuteCompiler`
* `Frontend`
* `Backend`
* `ModuleToFunctionPassAdaptor`
* `ModuleInlinerWrapperPass`
* `OptModule`
* `Source`
* `ParseClass`
* `ParseTemplate`
* `InstantiateClass`
* `InstantiateFunction`
* `CodeGen Function`
* `PerformPendingInstantiations`

## `ExecuteCompiler-stacked.json`
  * `Frontend`
  * `Backend`

## `Backend-stacked.json`
  * `ModuleToFunctionPassAdaptor`
  * `ModuleInlinerWrapperPass`
  * `OptModule`

## `Frontend-stacked.json`
  * `Source`
  * `ParseClass`
  * `ParseTemplate`
  * `InstantiateClass`
  * `InstantiateFunction`
  * `CodeGen Function`
  * `PerformPendingInstantiations`
