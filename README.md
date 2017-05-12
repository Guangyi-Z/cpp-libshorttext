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

## Converting the model file from LibShortText (Python)

```bash
python model_converter.py test/stub/train_file.model
# test/stub/train_file.model => test/stub/train_file.model_converted
```

## Running unit tests

After building this project you may run its unit tests by using these commands:

    $ make test  # To run all tests via CTest
    $ make catch # Run all tests directly, showing more details to you

### About testing stub

Download [LibShortText](https://www.csie.ntu.edu.tw/~cjlin/libshorttext/) zip file, and cd `demo` directory.
Execute the following commands, and you will obtain the benchmark data.

```bash
python ../text-train.py -P 0 -G 1 -F 1 -N 0 -L 3 -f train_file
python ../text-predict.py -f test_file train_file.model predict_result
```

## Denpendency

* catch
* [liblinear](https://github.com/cjlin1/liblinear)

## Missing

* Ignoring extra file in model file: `converter/extra_file_ids.pickle` and `converter/extra_nr_feats.pickle`
* Ignoring `-P`, `-G` options in LibShortText
* Ignoring TFIDF features

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
