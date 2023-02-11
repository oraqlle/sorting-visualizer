# SV - Sorting Visualizer

SV is a simple sorting visualizer programming using C++20 and SFML. Users can run many different sorting algorithms with randomly generated starting values or use an input list of starting values in order to visualize what certain configurations will yield.

## Content

- [SV - Sorting Visualizer](#sv---sorting-visualizer)
  - [Content](#content)
  - [Installing and Building](#installing-and-building)
  - [Commands](#commands)
  - [Included sorting algorithms](#included-sorting-algorithms)
    - [Adding your own](#adding-your-own)
  - [License, Code of Conduct \& Contributing](#license-code-of-conduct--contributing)

## Installing and Building

This project depends on [SFML](https://www.sfml-dev.org/download.php). Download and build it on your machine before building SV

To install just clone this repository onto your local machine. SV is built using [bpt](https://bpt.pizza/docs/latest/index.html). To build simply run:

```sh
$ bpt build -t build.yaml -o build
```

This will build and verify the program and place the built binary in `build/bin`. Once built you can run using:

```sh
$ ./build/bin/sv
```

## Commands

~

## Included sorting algorithms

The included sorting algorithms in SV are:

- Bucket sort
- Bubble sort
- Counting sort
- Insertion sort
- Intro sort
- Heap sort
- Mergesort
- Pancake sort
- Quicksort
- Radix sort
- Selection sort
- Shellsort

### Adding your own

~

## License, Code of Conduct & Contributing

This project is under the [MIT License](LICENSE). The [code of conduct](CODE_OF_CONDUCT.md) is governed by the contributor covenant (v2.1). Contributing guidelines are outlined in the [here](CONTRIBUTING.md).
