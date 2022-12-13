---
title: 'ctbench - compile-time benchmarking and analysis'
tags:
  - C++
  - metaprogramming
  - compilation
  - benchmarking
  - library
authors:
  - name: Jules P'enuchot
    orcid: 0000-0002-6377-6880
    equal-contrib: true
    affiliation: 1
affiliations:
  - name: Jules P'enuchot, LISN, Paris-Saclay University, France
    index: 1
date: 07 December 2023
bibliography: paper.bib
---

# Summary

C++ metaprograms become larger over time as metaprogramming goes mainstream

metaprogrammed libraries like Eigen <!--TODO: CITE-->, Blaze <!--TODO: CITE-->,
or even CTRE <!--TODO: CITE--> are require increasingly compute intensive
at compile-time

therefore more attention to compile-time performance is needed

there are a lot of tool to run benchmarks for runtime programs
but almost none to run compile-time variably sized benchmarks,
ie. tools that can help analyze scaling and complexity of metaprograms

we need proper tools to study how meta-programs scale

metaprograms run within compilers, so we might need more than just measuring
compiler execution times and start looking at more targeted data like what
Clang's integrated profiler provides with the `-ftime-trace` option.

<!--The forces on stars, galaxies, and dark matter under external gravitational
fields lead to the dynamical evolution of structures in the universe. The orbits
of these bodies are therefore key to understanding the formation, history, and
future state of galaxies. The field of "galactic dynamics," which aims to model
the gravitating components of galaxies to study their structure and evolution,
is now well-established, commonly taught, and frequently used in astronomy.
Aside from toy problems and demonstrations, the majority of problems require
efficient numerical tools, many of which require the same base code (e.g., for
performing numerical orbit integration).-->

# Statement of need

originally inspired by metabench <!--TODO: CITE-->, ctbench development was
driven by the need for a tool that can be integrated within an existing project
(metabench being a framework), component reusability, and analysis capabilities
for Clang profiling data in addition to compiler execution time measurement

additionally, metabench uses ERB markup for benchmark generation whereas ctbench
relies on C++ preprocessor

therefore ctbench is an attempt to provide a robust set of tools to run variably
sized compile-time benchmarks, then aggregate and analyze benchmark data

It has first class support for Clang's profiling data output format which makes
it possible to target specific Clang compiling passes, sometimes even for
specific symbols

It can also used to simply measure and plot compiler execution time for other
compilers than Clang making comparisons across compilers possible

## grapher architecture

provides several plotting strategies, more or less experimental,
and extension points to add new strategies in the future

## tooling

provides a dev-friendly CMake API for automated benchmark target declaration,
and plotters with JSON configuration interfaces

benchmark targets can be customized, CLI tooling is provided as well
to list plotters and output default configurations

benchmarks cases rely on the pre-processor for scaling

ctbench was designed to better understand the impact of metaprogramming
techniques on compile time, by providing an easy, robust, and repeatable way to
measure and analyze Clang's profiling data.

Its functionality was then expanded to compiler execution time measurement
to allow GCC vs Clang comparisons on compile-time benchmarks.

tailored for our own needs in Poacher and other metaprogramming projects
<!--TODO: CITE-->, with a C++ developer-friendly API to provide a compile-time
benchmarking toolset for the masses

easy to install, easy to use, reproductible benchmarks and scaling analyses
for metaprograms

<!--`Gala` is an Astropy-affiliated Python package for galactic dynamics. Python
enables wrapping low-level languages (e.g., C) for speed without losing
flexibility or ease-of-use in the user-interface. The API for `Gala` was
designed to provide a class-based and user-friendly interface to fast (C or
Cython-optimized) implementations of common operations such as gravitational
potential and force evaluation, orbit integration, dynamical transformations,
and chaos indicators for nonlinear dynamics. `Gala` also relies heavily on and
interfaces well with the implementations of physical units and astronomical
coordinate systems in the `Astropy` package [@astropy] (`astropy.units` and
`astropy.coordinates`).-->

<!--`Gala` was designed to be used by both astronomical researchers and by
students in courses on gravitational dynamics or astronomy. It has already been
used in a number of scientific publications [@Pearson:2017] and has also been
used in graduate courses on Galactic dynamics to, e.g., provide interactive
visualizations of textbook material [@Binney:2008]. The combination of speed,
design, and support for Astropy functionality in `Gala` will enable exciting
scientific explorations of forthcoming data releases from the *Gaia* mission
[@gaia] by students and experts alike.-->

# Citations

<!--https://pandoc.org/MANUAL.html#extension-citations-->

# Figures

<!--Figures can be included like this:
![Caption for example figure.\label{fig:example}](figure.png)
and referenced from text using \autoref{fig:example}.-->

<!--Figure sizes can be customized by adding an optional second parameter:
![Caption for example figure.](figure.png){ width=20% }-->

# Acknowledgements

<!--We acknowledge contributions from Brigitta Sipocz, Syrtis Major, and Semyeong
Oh, and support from Kathryn Johnston during the genesis of this project.-->

# References
