\title: Lab 1: Introduction to OpenGL

# 1) Setup and getting acquainted with the lab shell

## Where is the origin placed in the on-screen coordinate system?

The origin is in the center of the window.

## Which direction are the X and Y axes pointing in the on-screen coordinate system?

The X axe is pointing to the right, and the Y axe is pointing to the top. Both axes have half-window length.

## The triangle color is controlled from the fragment shader. Would it be possible to control it from the main program? How?

Yes, by using uniform variables to send values from the host to the GPU. I had to add this in the initialization code:

```
// Sending color to the fragment shader
glUniform4fv(glGetUniformLocation(program, "in_color"), 1, color);
```

and modify the fragment code:

```C
uniform vec4 in_color;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(in_color);
}
```

# 2) Transformations in the vertex shader

## What is the purpose of the "in", "out" and "uniform" modifiers?

All of them are storage qualifiers, used to _modify the storage or behaviour of global and locally defined variables_ [(source)](https://www.khronos.org/opengl/wiki/Type_Qualifier_(GLSL)#Storage_qualifier).

As seen in [stackexchange](http://gamedev.stackexchange.com/a/29673):

- `in`: linkage into shader from previous stage.
- `out`: linkage out of a shader to next stage.

So `in` and `out` keywords define information "pipes" between contiguous stages.

On the other hand, `uniform` variables _act as parameters that the user of a shader program can pass to that program_ [(source)](https://www.khronos.org/opengl/wiki/Uniform_(GLSL)).


## What is the output of the vertex shader?

The output are the same vertices of the input but transformed in some way, for example from model coordinates to screen coordinates.


## What does the function glUniformMatrix4fv do?

Initializes a matrix in the GPU, and stores the values from a matrix in the CPU to the GPU. In this way this values are accesible from the shaders.


# 3) Simple animation

## What is the frame rate of the animation?

As we are redrawing each 20 ms, we have 20 milliseconds/frame, and the inverse is 0.05 frames/ms or 50 frames/second.


# 4) Color shading

## Did you need to do anything different when uploading the color data?

Yes, I had to:

1. Create a vertex buffer object, with `glGenBuffers`.
2. Bind to a host reference, with `glBindBuffer`.
3. Initialize it with values from a host array with color-per-vertex info, with `glBufferData`.
4. Make it point to a variable in the fragment shader, with `glVertexAttribPointer`.
5. Enable it, with `glEnableVertexAttribArray`.

Then, in the vertex shader, I had to add the `in` variable I wanted to use (`in vec4 in_Color`). As I wanted to pass this info to the fragment shader, I added an output and input variables in vertex shader (`out vec4 vertex_Color`) and fragment shader (`in vec4 vertex_Color`) with the same name to pipe the values.

## The "in" and "out" modifiers are now used for something different. What?

They link values from the previous (`in`) or to the next (`out`) stage in the pipeline.

## What is this kind of shading called? What could we use otherwise?

Gouraud / phong


# 5) Building a cube, visible surface detection

## What problems did you encounter while building the cube?

- I had to change the `glDrawArrays` function to draw more than 3 vertices.
- Drawing all possible triangles by hand is a waste of time and resources. It's better to send vertices and then draw triangles refering to vertices by an index, using `glDrawElements`.
- I had to activate the depth test to draw correctly the box's triangles.
- I also activated the face culling to avoid drawing non-visible triangles.

## How do you change the facing of a polygon?

Triangles have to look to the outside of the box, not the inside. To make this, I have to define triangles in counterclockwise as they are seen from outside the box. You can say to OpenGL how the triangles should look with the function `glFrontFace`, where default value is `GL_CCW`.


# 6) Load a 3D model from disc

## Why do we need normal vectors for a model?

Because we want to know how each face will respond to lights orientation.

## What did you do in your fragment shader?

I'm passing from vertex shader normal info to simulate lights effects. I'm using 3 lights: one white from the camera, one red from the right side, and one blue from the top.

## Should a normal vector always be perpendicular to a certain triangle? If not, why?

Usually it should be, but you can obtain some effects by modifying it. For example you can implement bump mapping by using a modified normal vector for each pixel.

## Now we are using glBindBuffer and glBufferData again. They deal with buffers, but in what way?

`glBindBuffer` selects the buffer we are going to modify, and `glBufferData` modifies it. OpenGL use this functions because all OpenGL objects are hidden inside the API, and only can be accessed through this functions.
