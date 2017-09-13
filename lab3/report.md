---
title: Lab 3: Virtual world and specular shading
---

# 1) Hierarcical modelling, the windmill

## How can you get all four blades to rotate with just one time-dependent rotation matrix?

First I store a `blade_transform` matrix for each blade to set them in position. Then, when rendering I calculate a `blade_rotation` matrix each frame to spin the blades, and apply it to each blade by `Mult(w->blade_transform[i], blade_rotation)`.

## How do you make the wings follow the body's movements?

I apply the transforms of the blade to the transforms of the rest of the body, chaining all the transforms in this way.

## You may have to tweak some numbers to make the placement right. How can you do that without making a lot of small changes in numbers in the code followed by recompilations?

By looking at the model in a 3d editor like Blender? :P


# 2) Manual viewing controls

## What kind of control did you implement?

A first person view control.

## Can you make this kind of control in some other way than manipulating a "look-at" matrix?

Yes, more or less like I did but instead of ending using `lookAtv`,  applying all the rotation and translation transformations directly in a transformation matrix.


# 3) Virtual world and skybox

## How did you handle the camera matrix for the skybox?

I had stored the position of the camera as a `vec3`, so I'm just using it. I also have the `mat4` with the world-to-camera transformation, but I found easier to just reuse the position vector.


## How did you represent the objects? Is this a good way to manage a scene or would you do it differently for a "real" application?

I'm trying to apply an object-oriented approach as much as I know in C. It is difficult because there are lots of objects that behave or have very different properties (except for the static ones; camera, skybox and windmill behave very differently).


## What special considerations are needed when rendering a skybox?

It must follow the camera and it should be drawn first without Z and cull tests.


## What is the problem with the skybox object used in the lab? (The problem doesn't have to be corrected.)

That it is not a complete cube but the top-half. Sometimes looking from high places it's possible to see a very unrealistic horizon.


# 4) Specular shading, external light sources

## How do you generate a vector from the surface to the eye?

- In the vertex shader, I generate an output `vPos` to the pixel shader that contains the position in the world of each fragment.
- In the pixel shader, I have an uniform `cameraWorldPosition`.
- Then I only have to do `cameraWorldPosition - vPos` and normalize.


## Which vectors need renormalization in the fragment shader?

???
I had to normalize the direction vectors from light sources to surface points and from surface points to camera.


# 5) Multitexturing

## How did you choose to combine the texture colour and the lighting colour?

I multiply the texture color by the lighting.

## How did you choose to combine the two textures?

I add both colors of each texture.


# Extra) Managing transparency

## How did you remove the errors caused by transparency?

I disabled the cull test.
