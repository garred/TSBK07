#ifndef _helpers_hpp_
#define _helpers_hpp_

#include "VectorUtils3.h"


mat4 transformation(vec3 pos, vec3 rot, vec3 sca);

mat4 inverse_transformation(vec3 pos, vec3 rot, vec3 sca);

mat4 look_at_transformation(vec3 position, vec3 look_at, vec3 v);

#endif
