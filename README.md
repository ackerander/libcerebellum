# libcerebellum

Neural net library written in C, no dependencies.

## Quick Start

```console
$ make install
```

Remember to add the header `cerebellum.h` and link with `-lcerebellum`

## Intro

### Network Creation

To get started, create an empty `net_t` and give it

1) The number of layers
2) An array with the number of nodes in each layer
3) The learning rate

Optionally provide function pointers to initialize the weights and biases.

Example: A 3 layer network with learning rate 0.2
```C
size_t layers[] = { 4, 11, 2 };
net_t net3layer = { .layers = layers, .nlayers = 3, .rate = 0.2 };

initNet(&net3layer, NULL, NULL);
```

Or you can import a network from a file:

```C
net_t importedNet;

netFromFile(&importedNet, "someNetworkFile");
```

### Using a Network

To feed forward the network, simply provide an input array of equal length to the first layer:

```C
double input[4] = { 0, 0.42, 0.9, 0.1 };

feedforward(&net3layer, input);
```

To train the network, provide an input as before and the expected output (of size equal to the last layer).
`feedforward` must be called before `train` for it to be meaningful.

```C
double output[2] = { 0, 1 };

train(&net3layer, input, output);
```

### Cleaning up

If the network was initialized using `netFromFile`, free the layers array.
Finally call `delNet`.

```C
free(importedNet.layers);
delNet(&importedNet);
```

If you want to save the network before cleaning up, call `saveNet`:

```C
saveNet(&net3layer, "networkSaveFile");
delNet(&net3layer);
```

## TODO

* Fix dropout
* Add Convolutional Nets
* Add Recurrent Nets
* More ...
