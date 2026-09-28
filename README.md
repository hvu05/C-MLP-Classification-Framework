# C++ MLP Classification Framework

A lightweight neural-network framework written in C++17 for experimenting with multi-layer perceptron (MLP) classifiers. The project uses the header-only [xtensor](https://github.com/xtensor-stack/xtensor) library, included under `Code/include/tensor`, for tensor operations.

## Features

- Layer-based MLP models with Fully Connected, ReLU, Sigmoid, Tanh, and Softmax layers.
- Mini-batch data loading with shuffling and deterministic seeded sampling.
- Dataset preprocessing: train-set normalization and one-hot label encoding.
- Training primitives: forward/backward propagation, Cross-Entropy loss, and SGD, AdaGrad, and Adam optimizers.
- Classification evaluation through Accuracy, Precision, Recall, F1-score, and a confusion matrix.
- Model checkpoint save/load using an architecture file and NumPy `.npy` weight files.

## Project structure

```
Code/
├── datasets/       # Binary and three-class NumPy datasets
├── include/        # Framework headers and bundled dependencies
├── models/         # Example checkpoints
├── src/            # Framework implementation and entry point
├── Makefile
└── config.txt
```

## Build and run

Requirements: a C++17-compatible compiler and `make`.

```bash
cd Code
make
./program
```

The default entry point runs the two-class classification example. To run the three-class example, update `Code/src/program.cpp` to call `threeclasses_classification()` instead.

## Example usage

```cpp
ILayer* layers[] = {
    new FCLayer(2, 50, true),
    new ReLU(),
    new FCLayer(50, 2, true),
    new Softmax()
};

MLPClassifier model("./config.txt", "2c-classification", layers, 4);
SGD optimizer(2e-3);
CrossEntropy loss;
ClassMetrics metrics(2);

model.compile(&optimizer, &loss, &metrics);
model.fit(&train_loader, &valid_loader, 1000);
```

## Technologies

- C++17
- xtensor
- C++ Standard Library
- NumPy `.npy` datasets and checkpoints

## Notes

This repository was developed as a data-structures-and-algorithms coursework project. The datasets and bundled third-party headers are retained to make the examples reproducible.
