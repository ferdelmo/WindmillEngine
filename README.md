# Windmill Engine
Personal project to build a game engine from scratch, using vulkan as graphic APi.

![](/screenshots/DragonsWall.png)

## Render Engine
The render engine is the most worked part. The main features that are implemented are:
- Basic Shading: phong shading with difuse and specular parts with a color for each part and an alpha for the specular component. This ilumination model take into account the direction of the light and the normal of the surface to calculate the ilumination of each fragment. This also include basic color (the same color for the whole object) or textures, an object with only one texture for now.
- Lights: both point lights and directional lights are supported. Each light is caracterized by a color and its intensity. The shadow map techinc is used to allow objects to cast shadow, although for now only works with directional lights.
- Normal mapping: using the phong model, normal mapping use to improve visual quality, where a material can be generated with a texture with the normals values in tangent space, instead of using the per vertex normals. This allow to reproduce better objects like brick walls, adding details to the lighting calculation 

(add images of each thing here)

## Game Engine
The game engine is based in a component system. The world inside the engine can have any GameObjects and every GameObject can have multiple components. For now, only components for render and physics are made, but components to define behaviur can be easly created by create a component that inherite for the parent of all components, the component class.


## Physics Engine
For now the physics engine is very simple, it has two posible collider shapes, obb and spheres. The two components for triggers that have functions to overload or events to add for BeginOverlap, IsOverlapping and EndOverlap.

