---
title: Lab 4: Interactive terrain
---

# 1) Load and inspect the heightmap.

## What kind of data does the heightmap image data hold? What range is the heightmap data?

The heightmap contains datapoints of 24 bites per pixel with values ranged from 0 to 255.

## The terrain is 4x4. How many polygons does it draw?

The heightmap data is interpreted as the height of single points, and the terrain is composed of cells defined by 4 points, so we have 3x3=9 cells. The code provided draws two triangles per cell, so it is drawing 18 triangles.


# 2) Navigating the heightmap

## Did you need to extend your camera movement code? How?

Nope. It worked without problem.


# 3) Calculate normal vectors and apply lighting

## How did you implement the cross product? Function call, inline...? Normalization?

I did everything hard-coded. Not elegant, but fast. Debugging will be surely more "enjoyable", also.

I used the formal determinant to calculate it just as a sum of products.

## Which method did you use to find the normal vector?

I calculated the normal of all triangles first, and then I did the mean vector of all neighbour cells normals.


# 4) Calculating map height for a point

## How did you figure out what surface to use?

First I rescaled and translated the point to have terrain coordinates. Then I got the floor of the `x` and `z` coordinates to obtain the corner of the cell. Then I checked in what triangle the point falls by using the expression `x+z < 1`, that decides in what side of the plane the point is.

## How did you calculate the height from the surface?

I first obtained the equation of the triangle by using crossproduct of the sides for the normal, and dot product of the normal with a point for the displacement. Then I used the equation of the intersection between a segment and a plane, with a unit vertical segment that go through the point asked.


# 5) Pick one of the "extras" below as final part of the lab.

## 5a) Multitextured terrain [medium] - What kind of multitexturing function did you implement?

I'm rendering all the textures with a mask (that is another texture) to mix them in a smooth way.
