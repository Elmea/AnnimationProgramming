# Animation Programming

*This project is a isart digital's school project.
The goal is to learn the basics of the animation programming.*

## First steps
---

First, we needed to create our quaternions in ou maths library

![png](screenshots/quat.png)

In parallel, we started to read and draw the skeleton of the character.

![png](screenshots/skeleton.png)

For the moment, we had zero difficulties to implement thoses features.

## Difficulties are comming
---

Once the skeleton was working fine, we manage to animate it, while we were fixing the last bugs on the quaternions interpolations.

![png](screenshots/walk.png)

And then, we managed to use those interpolation and begin the skinning of the skeleton.

![gif](screenshots/Skinning1.gif)

As you can see, that wasn't the easiest part of the project.

So first, we searched in the skinning shader, and then in our matrix. 

The problem was caused by the order of our matrix, in fact, the engine was considerating
our matrix like collumn majors ones, but they are row majors. 

So we needed to adapt ou matrices's product to get the expected result.
![gif](screenshots/result.gif)

## Some cutted feature
---

- We tried to implement IMGUI on the custom render engine, but we didn't manage to have a result that was satisfying us.

- Animation blending, due to the short time we had to implement it.