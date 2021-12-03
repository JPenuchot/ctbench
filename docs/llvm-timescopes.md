# LLVM Timescopes

Quick summary of what different Clang/LLVM's TimeScopes measure.

## ExecuteCompiler

Complete compiler execution time.

## Frontend

Files: `clang/lib/CodeGen/CodeGenAction.cpp`, `clang/lib/Parse/ParseAST.cpp`

## Source

File: `clang/lib/Sema/Sema.cpp`

The Source timer measures pre-processing time. It can be used to ensure that
benchmark generation using macros has a negligible impact on final benchmark
results.

## ParseClass

File: `clang/lib/Parse/ParseDeclCXX.cpp`

## InstantiateClass

File: `clang/lib/Sema/SemaTemplateInstantiate.cpp`

## Backend

File: `clang/lib/CodeGen/BackendUtil.cpp`

## ParseTemplate

File: `clang/lib/Parse/ParseTemplate.cpp`

## OptModule

File: `llvm/lib/IR/LegacyPassManager.cpp`

## CodeGenPasses

File: `clang/lib/CodeGen/BackendUtil.cpp`

## PerModulePasses

File: `clang/lib/CodeGen/BackendUtil.cpp`

## PerFunctionPasses

File: `clang/lib/CodeGen/BackendUtil.cpp`

## PerformPendingInstantiations

File: `clang/lib/Sema/Sema.cpp`
