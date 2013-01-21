=======================
Flow Network Generation
=======================

Flow networks robust against damages are simple model networks described in a
series of publications by Kaluza *et al*.\ [1_, 2_, 3_]. The C++ code
presented in this repository allows for the generation of such networks via a
simulated evolution.

Although the code was programmed with a single core in mind, the compiled binary
can easily be (and has been) run multiple times in parallel.

Installation
------------

In the main directory, you need to edit the `CMakeLists.txt` file. If your
libraries are installed in non-standard locations, please adapt lines 8 to 11.
Then run the following commands::

    cmake init .
    make

If everything went smoothly there should now be a `bin` and `lib` subdirectory.
Add the path to your environment variable LD_LIBRARY_PATH if you want to run
things from here. In bash this can be done by::

    export LD_LIBRARY_PATH="$HOME/path/to/rfn-generation/lib:$LD_LIBRARY_PATH"

You can now run the `simulation` binary in the `bin` subdirectory which will
print some info to stdout.

If you want to install the `bin` and `lib` subdirectories in a different
location, either edit line 17 of the `CMakeLists.txt` file or invoke::

    make install -DDESTDIR=/your/favourite/path

When you are ready to move from testing to large-scale computation you should
rebuild the project without debugging and text output, follow these commands::

    cmake -DDEBUG=OFF .
    make

Enjoy!

Note
----

The output files are binary and their exact structure depends on your system's architecture (32 or 64 bit).

Requirements
------------

C++:
~~~~

*NB*: If you install these libraries from system packages, please make sure to also
install the `dev` packages as the headers are needed for compilation.

* Boost_/ namely: algorithm, random, graph, filesystem V3, accumulators,
  pending/queue, program_options
* `GNU Scientific Library`__

Others:
~~~~~~~

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

