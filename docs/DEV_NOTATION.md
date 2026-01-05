# About Code Style

The Glacier2 Engine, ZHMModSDK and the ZHMChaosMod use a variation of Hungarian/Systems Notation.
As you'll see this constantly, here's a quick crash-course.

please note that this is what i've been using any may be completely wrong.
i basically tried to learn from the existing code.

also note that not everything follows these rules (the Engine, ZHMModSDK or my own code). 
this is essentially a  



## Methods & Functions

Method and function naming is pretty straight-forward, basically pascal case.   

Example: 
```cpp
void MyFunction();
```

## Classes, Structs and Enums

Classes use a `Z` prefix, the rest of the name in pascal case. 
Structs and Enums follow the same schema, but with `S` and `E` prefix, respectively.  

Example:
```cpp
class ZMyClass;

struct SMyStruct;

enum /*(class)*/ EMyEnum;
```

## Variables, Globals and Constants

Variables use a dual prefix in the schema `<scope>_<type>`, with the remainder of the name in pascal case.

`<scope>` and `<type>` are used as follows (note that these tables may be incomplete):

| `<scope>` | Description           |
| --------- | --------------------- |
| `p`       | parameter             |
| `s`       | local variable        |
| `m`       | member variable       |
| `g`       | (non-constant) global |
| `c`       | constant              |

| `<type>` | Description                                         |
| -------- | --------------------------------------------------- |
| `n`      | integer                                             |
| `f`      | floating-point                                      |
| `b`      | boolean                                             |
| `s`      | string (`std::string` or `ZString`)                 |
| `sz`     | c-style string (`char*`)                            |
| `p`      | pointer (raw or smart)                              |
| `e`      | enum                                                |
| `r`      | (entity) reference                                  |
| `rid`    | resource ID                                         |
| `v`      | vector (e.g. `float4`)                              |
| `m`      | matrix (e.g. `SMatrix`)                             |
| `a`      | list (raw, `std::array`, `std::vector` or similar)  |
| `m`      | map (`std::map` or similar)                         |
| `q`      | queue (`std::queue` or similar)                     |
| none     | classes, structs, and when you don't know anything. |


Example:

```cpp
static const c_sSomeText = "Hello World!";
static constexpr c_ridMyEntity = ResId<"[assembly:/example.entitytemplate].pc_entitytype">;

static std::vector<SMyClass> g_vMyList;

struct SMyStruct
{
    float m_fMyFloat;
    bool m_bMyFlag;
}

class SMyClass
{
    void MyFunction(const ZEntityRef p_rEntity, const SMatrix p_mTransform)
    {
        auto s_bEntityValid = !!p_rEntity;
        // ...
    }

    void MyOtherFunction(const SMyStruct& p_Foo)
    {
        // ...
    }
}
```
