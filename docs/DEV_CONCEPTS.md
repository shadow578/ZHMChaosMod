# Development Concepts

While most concepts in this mod are common to the ZHMModSDK and the Glacier2 Engine, theres also some helpers specifict to the chaos mod.
This document aims to document some of these features. 
For more information, take a look in the `src/Helpers` folder for all available helpers.


## EntityFinder

`ZEntityFinder` in `Helpers/EntityUtils.h` allows you to easily find entities loaded in the current scene.


### example usage:

```cpp
// find all entities named "Foo"
auto& s_aEntities Utils::ZEntityFinder()
	.EntityName("Foo")
	.Find();

// find the first entity in the scene with entity id `0xf00`
const auto& s_rFirstEntity = Utils::ZEntityFinder()
	.EntityID(0xf00)
	.FindFirst();
```

## Entity Bindings

`Helpers/EntityBinding.h` provides some helpers for creating so-called entity bindings. 
entity bindings provide a easy-to-use api for accessing properties and pins on entities, without having to reverse-engineer the memory layout of the entities.
this is done via the Glacier2 engine's reflection system.


### benefits & drawbacks

the benefit to using entity bindings is clearly the ease and speed at which they can be created.
you simply need to know the name and type of properties, which can easily be found using GlacierKit or the builtin editor mod of ZHMModSDK.
there's no need to figure out the memory layout.

this is further amplified by the `tools/EntityBindingCodegen.html` tool.
this tool allows you to generate large portions of the entity binding, simply from information obtained from GlacierKit.


of course, there's also some drawbacks:
for one, only entities and properties supported by the reflection system are available. 
if a entity has a internal property, or simply one never used by the game, there's no way to access it via a binding.
also, only entities are supported. there's no way to use bindings for, say, the actor manager.
lastly, the performance of the reflection system will be much worse than directly accessing the properties. 
this probably isn't too much of a problem unless access lots of properties in a loop or frame handler.

### example usage:

```cpp
struct SMyEntityBinding
{
  BINDING_CONSTRUCTOR(SMyEntityBinding);

  PROPERTY(int32, m_nSomeIntProperty);
  PROPERTY(float32, m_fSomeFloatProperty);

  INPUT_PIN(SomeInputPin);
  OUTPUT_PIN(SomeOutputPin);
};


// usage:
SMyEntityBinding s_Binding(s_rEntity);

// reading and writing properties
auto s_nValue = s_Binding.m_nSomeIntProperty;
s_Binding.m_nSomeFloatProperty = 0.0f;

// signaling pins
s_Binding.SomeInputPin();
s_Binding.SomeOutputPin_Out();
```



