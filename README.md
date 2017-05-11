# cpp-libshorttext

LibShortText: A Library for Short-text Classification and Analysis, in Pure C++.

* Only for predicting part
* LibShortText (Python) model files adapted

## Building From Source

First make sure that you have [CMake](http://www.cmake.org/) and an C++ compiler environment installed.

Then open a terminal, go to the source directory and type the following commands:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

## Running unit tests

After building this project you may run its unit tests by using these commands:

    $ make test  # To run all tests via CTest
    $ make catch # Run all tests directly, showing more details to you

## Denpendency

* catch
* [liblinear](https://github.com/cjlin1/liblinear)

## Missing

* Ignoring extra file in model file: `converter/extra_file_ids.pickle` and `converter/extra_nr_feats.pickle`
* Ignoring `-P`, `-G` options in LibShortText

## Trial and Error

* [TextGrocery for better understanding of LibShortText](https://github.com/2shou/TextGrocery)
* [PicklingTools not work](https://github.com/pyloor/picklingtools/issues/4)

## License

![GNU GPLv3 Image](https://www.gnu.org/graphics/gplv3-127x51.png)

This program is Free Software: You can use, study share and improve it at your
will. Specifically you can redistribute and/or modify it under the terms of the
[GNU General Public License](https://www.gnu.org/licenses/gpl.html) as
published by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
