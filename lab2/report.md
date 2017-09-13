\title: Lab 2: Texture, viewing and shader variations

# 1) Procedural texture mapping

## How are the textures coordinates mapped on the bunny? Can you see what geometry was used?

I think that they used an spherical mapping to set the texture coordinates.

## What kind of procedural texture did you make?

I did a grid texture with white lines in coordinates multiples of 0.1: 0.0, 0.1, 0.2, 0.3, ... I did that by:
- Multiplying the coordinates by 10.
- Retain the fractional part with `fract`.
- Substracting 0.1 (proportional to the width of the lines).
- Calculating the sign.
- Putting white if sign is positive, black in other case.


# 2) Texture mapping

## Can we modify how we access the texture? How?

It is possible to access using the normal coordinates, what gives an effect of mirror texture.

## Why can't we just pass the texture object to the shader? There is a specific reason for this, a limited resource. What?

It is necessary to define a way to access to (or sample) the texture. There are parameters to set like interpolation, midmapping, etc.


# 3) Projection

## How did you move the bunny to get it in view?

I moved it (0,0,-2) to put in front of the camera.


# 4) Viewing using the lookat function

## Given a certain vector for v, is there some place you can't place the camera?

We can't put the camera exactly at the bottom of the location we want to look at, because in this situation the camera can rotate around the axis in any direction (the resulting rotation matrix is singular).


# 5) Diffuse shading

## Did you implement your light calculations in the vertex or fragment shader? So, which kind of shading did you implement?

I implemented it in the vertex shader, so I'm using Gouraud shading.

## Some geometry data must be vec4, others are just as well vec3's. Which ones, and why? How about vertices, light source, normal vectors...?

Initially the normal of the vertex are vec4 but I turned them to vec3 to multiply it by a translation-free version of the transformation matrix. Colors are vec4 (r,g,b,alpha). The rest are mat4 and vec4, the usual homogeneous format.


# 6) Extra: Gouraud vs Phong

## Was the difference big? If not, why?

No, because the calculation is basically the same, just in a different element (vertex vs fragment).

## You are doing almost the same operations. So what is the difference performance-wise? Compare the two methods from a performance standpoint.

Usually there are much less vertex in a model than fragments rendered, so if we have to calculate the light fragment-wise we will have to do much more operations.


# 7) Building a scene + camera movement

## If you rotate an object or rotate the camera, what matrices are affected?

When rotating an object I have to change the transformationMatrix for that object, so usually I have to change this matrix for each object. When rotating the camera I have to change the lookAtMatrix, and it is the same for all objects so is not necessary to change it for each object.


# Extra) Vertex shader fun

Vertical wave effect added.
