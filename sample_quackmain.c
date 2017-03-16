/*
 * A sample of code that uses the Quack runtime. 
 * We'll write the kind of C we would expect to 
 * produce by code generation. 
 */

#include <stdio.h>
#include <stdlib.h>

#include "Builtins.h"

void quackmain(); 

/* Boilerplate --- 
 * let C build us a main program. 
 */
int main(int argc, char** argv) {
  quackmain();
  printf("--- Terminated successfully (woot!) ---\n");
  exit(0);
}

/* ---- User-written class -----
 * -  Hand-generated, but what we
 * -  would want to generate for 
 * 
 class Pt(x: Int, y: Int) {
    this.x = x;
    this.y = y; 

    def PRINT( ):Pt {
       "( ".PRINT(); 
       this.x.PRINT(); 
       ", ".PRINT();
       ")".PRINT(); 
     }

     def PLUS(other: Pt) {
         return Pt(this.x+other.x, this.y+other.y); 
     }
  }
  *-----------------------------
  */

/* ================
 * Pt
 * Fields: 
 *    x, y: Int
 * Methods: 
 *    STR  (inherit, since we don't have concatenation yet)
 *    PRINT   (override)
 *    EQ  (inherit, since our EQ is busted) 
 *    and introducing
 *    PLUS    (add x and y values pointwise)
 * =================
 */

/* Declarations based on pattern in Builtins.h */ 

// Forward declaration of structs for class Pt
struct class_Pt_struct;
typedef struct class_Pt_struct* class_Pt;
struct class_Pt_struct  the_class_Pt_struct;  /* So I can use it in PLUS */
extern class_Pt the_class_Pt;

struct obj_Pt_struct;
typedef struct obj_Pt_struct* obj_Pt;

// Decleration of obj_Pt
struct obj_Pt_struct {
  class_Pt  clazz;
  obj_Int x;
  obj_Int y;
};

// Decleration of class_Pt_struct
struct class_Pt_struct {
  /* Method table */
  obj_Pt (*constructor) (obj_Int, obj_Int );  
  obj_String (*STR) (obj_Obj);           /* Inherit for now */
  obj_Pt (*PRINT) (obj_Pt);                 /* Overridden */
  obj_Boolean (*EQ) (obj_Obj, obj_Obj); /* Inherited */
  obj_Pt (*PLUS) (obj_Pt, obj_Pt);          /* Introduced */
};


/* Definitions based on pattern in Builtins.c */
/* Constructor */
obj_Pt new_Pt(obj_Int x, obj_Int y ) {
  obj_Pt new_thing = (obj_Pt)
    malloc(sizeof(struct obj_Pt_struct));
  new_thing->clazz = the_class_Pt;
  /* Quack code: 
    this.x = x;
    this.y = y; 
  */
  new_thing->x = x;
  new_thing->y = y; 
  return new_thing; 
}

/* Pt:STR --- omitted for now  */

/* Override PRINT method wit this Quack code: 
    def PRINT( ):Pt {
       "( ".PRINT(); 
       this.x.PRINT(); 
       ", ".PRINT();
       ")".PRINT(); 
     }
*/
obj_Pt Pt_method_PRINT(obj_Pt this) {
  obj_String lparen = str_literal("(");
  lparen->clazz->PRINT(lparen);
  this->x->clazz->PRINT((obj_Obj) this->x);
  obj_String comma=str_literal(",");
  comma->clazz->PRINT(comma);
  this->y->clazz->PRINT((obj_Obj) this->y);
  obj_String rparen = str_literal(")");
  rparen->clazz->PRINT(rparen);
  return this;
}
/* A note on optimization: A good compiler would not create those 
 * string literals each time Pt_method_PRINT is called.  Since they 
 * are "compile time constants", they would be computed once, 
 * stored, and reused. Maybe we'll do this week 9. 
 */ 

/* PLUS (new method) from this Quack code: 
     def PLUS(other: Pt) {
         return Pt(this.x+other.x, this.y+other.y); 
     }
*/
obj_Pt Pt_method_PLUS(obj_Pt this, obj_Pt other) {
  obj_Int this_x = this->x;
  obj_Int other_x = other->x;
  obj_Int this_y = this->y;
  obj_Int other_y = other->y; 
  obj_Int x_sum = this_x->clazz->PLUS(this_x, other_x);
  obj_Int y_sum = this_y->clazz->PLUS(this_y, other_y); 
  return the_class_Pt->constructor(x_sum, y_sum); 
}

/* The Pt Class (a singleton) */
struct  class_Pt_struct  the_class_Pt_struct = {
  new_Pt,     /* Constructor */
  Obj_method_STR, 
  Pt_method_PRINT, 
  Obj_method_EQ,
  Pt_method_PLUS
};

class_Pt the_class_Pt = &the_class_Pt_struct; 


void quackmain() {
  /* Write me some code */
  obj_Int x;
  obj_Int y; 
  obj_Int z;
  x = int_literal(11);
  y = int_literal(31);
  z = x->clazz->PLUS(x,y);
  z->clazz->PRINT((obj_Obj) z);

  obj_String newline_label = str_literal("\n===\n");
  newline_label->clazz->PRINT(newline_label);

  /* Quack code: 
     pt1 = Pt(x,y); 
     pt1.PRINT(); 
     pt2 = Pt(y,x); 
     pt2.PRINT();
     pt3 = pt1 + pt2; 
     pt3.PRINT(); 
  */
  obj_Pt pt1 = the_class_Pt->constructor(x,y);
  pt1->clazz->PRINT(pt1); 
  obj_Pt pt2 = the_class_Pt->constructor(y,x);
  pt2->clazz->PRINT(pt2);
  obj_Pt pt3 = pt1->clazz->PLUS(pt1, pt2);
  pt3->clazz->PRINT(pt3); 

  newline_label->clazz->PRINT(newline_label); 
}
