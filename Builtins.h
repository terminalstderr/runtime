/*
 * The Quack runtime needs a base class Obj and a handful 
 * of built-in classes  (Int, String, Nothing, Boolean)
 *
 * Getting the LLVM typing right is a bit of a pain.  We'll 
 * try to do it mainly by casting. 
 */
#ifndef Builtins_h
#define Builtins_h

/* Naming conventions:  
 * class_X means a reference to the class structure for class X, 
 * i.e., pointer to the struct that contains the method table. 
 * 
 * obj_X means the corresponding object structure reference, 
 * i.e., a pointer to the struct where instance variables live.
 * 
 * For each, we'll define a struct tag with *_struct and
 * and then define a reference to that struct with a typedef, 
 * e.g., class_Obj will be typedefed to struct class_Obj_struct *. 
 * Each class will have a single instantiation as the_class_X. 
 * the_class_X is a pointer to a statically allocated structure. 
 * 
 * All programmer-visible fields of objects are references to 
 * other objects. Boolean, Int, and String have "hidden" fields 
 * that contain their unboxed primitives. 
 * Classes are essentially vectors of function pointers, but for 
 * convenience and checking we declare them as (pointers to) 
 * structs containing function pointers with particular signatures. 
 * The receiver object ('this' in Quack) is an implicit argument 
 * in Quack but an explicit argument in the runtime. 
 */ 

/* The following object types are "known" from Obj, in the 
 * sense that there are Obj methods that return these types. 
 */
struct obj_String_struct;
typedef struct obj_String_struct* obj_String;

struct obj_Boolean_struct;
typedef struct obj_Boolean_struct* obj_Boolean;

struct obj_Nothing_struct;
typedef struct obj_Nothing_struct* obj_Nothing;

/* ==============
 * Obj 
 * Fields: None
 * Methods: 
 *    Constructor  (called after allocation) 
 *    STR 
 *    PRINT
 *    EQ
 * 
 * ==============
 */

struct class_Obj_struct;
typedef struct class_Obj_struct* class_Obj; 

typedef struct obj_Obj_struct {
  struct class_Obj_struct *clazz;
} * obj_Obj;

struct class_Obj_struct {
  /* Method table */
  obj_Obj (*constructor) ( void );
  obj_String (*STR) (obj_Obj);
  obj_Nothing (*PRINT) (obj_Obj);
  obj_Boolean (*EQ) (obj_Obj, obj_Obj);
}; 

extern class_Obj the_class_Obj; /* Initialized in Builtins.c */

/* ================
 * String
 * Fields: 
 *    One hidden field, currently holding char*
 *    (change later to a rope for garbage collection)
 * Methods: 
 *    Those of Obj, plus ordering, concatenation 
 *    (Incomplete for now.)
 * ==================
 */

struct class_String_struct;
typedef struct class_String_struct* class_String;

typedef struct obj_String_struct {
  class_String clazz;
  char *text;     /* FIXME: Make this a garbage-collectable rope */ 
} * obj_String;

struct class_String_struct {
  /* Method table: Inherited or overridden */
  obj_String (*constructor) ( void );
  obj_String (*STR) (obj_String);
  obj_Nothing (*PRINT) (obj_String);
  obj_Boolean (*EQ) (obj_String, obj_Obj);
  /* Method table: Introduced in String */
  obj_Boolean (*LESS) (obj_String, obj_String);
  obj_String (*PLUS) (obj_String, obj_String);
};

extern class_String the_class_String;

/* Construct an object from a string literal. 
 * This is not available to the Quack programmer, but 
 * is used by the compiler to create a literal string
 * from a Quack literal string. 
 */ 
extern obj_String str_literal(char *s);

/* ================
 * Boolean
 * Fields: 
 *    One hidden field, an int (0 for False, -1 for True)
 * Methods: 
 *    
 * =================
 */

struct class_Boolean_struct;
typedef struct class_Boolean_struct* class_Boolean; 

typedef struct obj_Boolean_struct {
  class_Boolean  clazz;
  int value; 
} * obj_Boolean;

struct class_Boolean_struct {
  /* Method table: Inherited or overridden */
  obj_Boolean (*constructor) ( void );
  obj_String (*STR) (obj_Boolean);
  obj_Nothing (*PRINT) (obj_Obj);               /* Inherit */
  obj_Boolean (*EQ) (obj_Obj, obj_Obj);         /* Inherit */ 
//  obj_Boolean (*NOT) (obj_Boolean);
//  obj_Boolean (*AND) (obj_Boolean, obj_Boolean);
//  obj_Boolean (*OR)  (obj_Boolean, obj_Boolean);
};

extern class_Boolean the_class_Boolean; 

/* There are only two instances of Boolean, 
 * lit_true and lit_false
 * (i.e., the values of the literals true and false)
 * The constructor should return one of them; 
 * maybe lit_false. 
 */
extern obj_Boolean lit_false;
extern obj_Boolean lit_true;


/* ==============
 * Nothing (really just a singleton Obj)
 * Fields: None
 * Methods: 
 *    Constructor  (called after allocation) 
 *    STR 
 *    PRINT
 *    EQ
 * 
 * ==============
 */

struct class_Nothing_struct;
typedef struct class_Nothing_struct* class_Nothing; 

typedef struct obj_Nothing_struct {
  class_Nothing clazz;
} * obj_Nothing;

/* I don't THINK we can ever call a method on Nothing, 
 * but we'll give it a real method table just in case. 
 */ 
struct class_Nothing_struct {
  /* Method table */
  obj_Nothing (*constructor) ( void );
  obj_String (*STR) (obj_Nothing);
  obj_Nothing (*PRINT) (obj_Obj);               /* Inherited */
  obj_Boolean (*EQ) (obj_Obj, obj_Obj); /* Inherited */
}; 

extern class_Nothing the_class_Nothing;

/* There is a single instance of Nothing, 
 * called nothing
 */
extern obj_Nothing nothing;

/* ================
 * Int
 * Fields: 
 *    One hidden field, an int
 * Methods: 
 *    STR  (override)
 *    PRINT   (inherit)
 *    EQ  (override)
 *    and introducing
 *    LESS
 *    PLUS    
 *    (add more later) 
 * =================
 */

struct class_Int_struct;
typedef struct class_Int_struct* class_Int; 

typedef struct obj_Int_struct {
  class_Int  clazz;
  int value; 
} * obj_Int;

struct class_Int_struct {
  /* Method table: Inherited or overridden */
  obj_Int (*constructor) ( void );
  obj_String (*STR) (obj_Int);  /* Overridden */
  obj_Nothing (*PRINT) (obj_Obj);      /* Inherited */
  obj_Boolean (*EQ) (obj_Int, obj_Int); /* Overridden */
  obj_Boolean (*LESS) (obj_Int, obj_Int);   /* Introduced */
  obj_Boolean (*GREATER) (obj_Int, obj_Int);   /* Introduced */
  obj_Boolean (*LESSEQ) (obj_Int, obj_Int);   /* Introduced */
  obj_Boolean (*GREATEREQ) (obj_Int, obj_Int);   /* Introduced */
  obj_Int (*PLUS) (obj_Int, obj_Int);       /* Introduced */
  obj_Int (*MINUS) (obj_Int, obj_Int);       /* Introduced */
  obj_Int (*MULTIPLY) (obj_Int, obj_Int);       /* Introduced */
  obj_Int (*DIVIDE) (obj_Int, obj_Int);       /* Introduced */
  obj_Int (*NEGATE) (obj_Int);            /* Introduced */
};

extern class_Int the_class_Int; 

/* Integer literals constructor, 
 * used by compiler and not otherwise available in 
 * Quack programs. 
 */
extern obj_Int int_literal(int n);


/* ===============================
 * Make all the methods we might 
 * inherit visible to user code 
 *================================
 */
obj_String Obj_method_STR(obj_Obj this);
obj_Nothing Obj_method_PRINT(obj_Obj this);
obj_Boolean Obj_method_EQ(obj_Obj this, obj_Obj other); 
obj_String String_method_STR(obj_String this);
obj_Nothing String_method_PRINT(obj_String this);
obj_Boolean String_method_EQ(obj_String this, obj_Obj other); 
obj_String Boolean_method_STR(obj_Boolean this); 
obj_String Nothing_method_STR(obj_Nothing this);
obj_String Int_method_STR(obj_Int this); 
obj_Boolean Int_method_EQ(obj_Int this, obj_Int other);
obj_Boolean Int_method_LESS(obj_Int this, obj_Int other);
obj_Int Int_method_PLUS(obj_Int this, obj_Int other);
obj_Int Int_method_MINUS(obj_Int this, obj_Int other);
obj_Int Int_method_MULTIPLY(obj_Int this, obj_Int other);
obj_Int Int_method_DIVIDE(obj_Int this, obj_Int other);
obj_Int Int_method_NEGATE(obj_Int this);

#endif
