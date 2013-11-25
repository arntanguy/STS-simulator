STS-simulator
=============

A program displaying curves for studying the Scanning Tunneling Spectroscopy data.

The goal of the simulator is to produce (C++) software for visual, real-time simulation of experimental scanning tunnelling spectroscopic data. Using the input of two user-defined, analytical curves two new curves must be calculated with known analytical equations. All four curves should be displayed in graphs simultaneously and real-time will mean that any change in the input must reflect the result in short (video) time. The two inputs should be defined by the user in separate panels displayed together with the four graphs. The first resulting curve is an integral equation of the two input curves and the second result is a derivative of the first. Experimental data should be displayed in the result-graphs for comparison and fit with the simulated data.


** Building
This project is using the Qt Library, so you'll need to install Qt first.
Then, you only need to do

    qmake-qt4
    make

That's it, you're done!

** Usage details

*Graphs
- Zooming: Left mouse drag for zooming in
           Middle mouse for zooming out one step
           Right mouse for zooming out all the way
