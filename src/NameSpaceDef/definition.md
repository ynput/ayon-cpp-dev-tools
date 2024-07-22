## Top-Level Namespace

- ynput: Root namespace encompassing all Ynput-related code.

## Standard Sub-Layer Namespaces

- ynput::core:
  - Foundational functions and classes providing abstract functionality for
    higher layers.
- ynput::lib:
  - Self-contained tools, classes, and function groups focusing on specific
    tasks in an abstract manner.
- ynput::dt:
  - Development tools essential during development but excluded from release
    builds.
- ynput::tool:
  - Tools addressing specific problems with reduced abstraction compared to
    libraries.
- ynput::app:
  - Standalone systems requiring startup procedures or running independently.

# Description

## Core Libraries

Foundational functions and classes designed for abstract functionality without
external dependencies. Ideal for header-only implementations.

- ynput::core:

### Nested Standard Namespaces

- ynput::core::utils: General namespace for tools without a dedicated namespace
  association.
- ynput::core::config: Management of configuration settings.
- ynput::core::math: Mathematical operations and utilities.
- ynput::core::time: Utilities for handling time and date.
- ynput::core::iostd: Utilities for handling std input-output.
- ynput::core::fio: Utilities for handling file input-output.
- ynput::core::threading: Utilities for handling threading and thread
  management.
- ynput::core::dt: data types and classes providing extended or specific data
  control
- ynput::core::cp: Utilities to help with cross-platform development

## Developer Tooling

Classes and functions supporting C/C++ software development are excluded from
release builds through macros.

- ynput::dt:

- ynput::dt::trace: Functions specifically for tracing purposes.

## Libraries

Self-contained libraries with abstract interfaces (pure virtual classes),
thorough documentation, defined behaviour, internal exception handling, and
versioning.

- ynput::lib:

- ynput::lib::logging: Functions dedicated to logging.

## Tool Modules

Tools or collections of tools targeting specific problems offering less
abstraction than libraries.

- ynput::tool:

- ynput::tool::usd: Example: a specific tool interacting with Ayon-Server to
  build USD files.

## Application Modules

Namespaces for standalone applications within the Ynput ecosystem require
startup procedures and do not compile to a static or dynamic library.

- ynput::app:

### Nested Standard Namespaces

- ynput::app::{name}: Specific namespaces for individual applications. (this
  should be avoided if possible)
