=======================
Flow Network Generation
=======================

coming soon\ |tm|...

.. |tm| unicode:: U+2122

Flow networks robust against damages are simple model networks described in a
series of publications by Kaluza *et al*. [1]_, [2]_, [3]_. The C++ code
presented in this repository allows the generation of such networks via a
simulated evolution.

Although the code was programmed with a single core in mind, the compiled binary
can easily be (and has been) run multiple times in parallel.

Requirements
------------

**C++:**

* Boost_ namely: /algorithm, /random, /graph, /filesystem, /accumulators,
  /pending/queue, /program_options
* `GNU Scientific Library`__

**Others:**

* cmake_

.. _Boost: http://www.boost.org/
__ gsl_
.. _gsl: http://www.gnu.org/software/gsl/
.. _cmake: http://www.cmake.org/

References
----------

.. [1] Kaluza, P., Ipsen, M., Vingron, M. & Mikhailov, A. S. Design and statistical properties of robust functional networks: A model study of biological signal transduction. Physical Review E 75, 15101 (2007).
.. [2] Kaluza, P. & Mikhailov, A. S. Evolutionary design of functional networks robust against noise. Europhysics Letters 79, 48001 (2007).
.. [3] Kaluza, P., Vingron, M. & Mikhailov, A. S. Self-correcting networks: function, robustness, and motif distributions in biological signal processing. Chaos 18, 026113 (2008).

