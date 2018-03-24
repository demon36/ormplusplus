# ormplusplus
A basic MySQL ORM for c++11 that does not precompile your code

### plan:

use CRTP pattern for returning instances of child class from parent class (findMany, removeMany, ..)

use sophisticated macros to facilitate user model definitions

use decorator pattern for conditioned queries:  `model::where({loc_id, 20})->orderBy({id, age})->limit(100)->get();`

use camelCase for model user defined members and snake_case for column bindings

use nullable data types
