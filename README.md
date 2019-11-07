# ConvNN

A convolutional neural network for image classification

Uses the cross-correlation (convolution) operation on the images through a set of filters for each RGB color.

The network uses a ReLU activiation function and softmax function on the last layer of the network for image classification.

In the example shown, the network is trained to differentiate between specific pictures of apples and oranges and then it is
able to classify the correct fruit in the image based on a different set of pictures.

Note: The network may experience gradient blowup depending on the intial values of the weights. L1 or L2 regularization needs to be implemented. For now the network is redesigned if gradient blowup occurs. Also, stochastic gradient descent should be implemented to avoid local minima.
