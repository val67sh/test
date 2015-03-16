

/*

Image stabilization information
===============================

We have two frames: current frame and reference frame (previous frame).
Attention!!!  We need try to find a MB (makro block) of reference frame in the current frame.
That means that the current frame has  a area for the calculation of LMV (local motion vector) and size
of this area is large than a size of MB of refernce frame.





















*/