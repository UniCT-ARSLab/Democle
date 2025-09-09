/*
 * predicates.h
 */

#ifndef __PREDICATES_H
#define __PREDICATES_H

#include "democle_types.h"

predicate(neq,
          {
              return args[0] != args[1];
          });

predicate(eq,
          {
              return args[0] == args[1];
          });

predicate(geq,
          {
              return args[0] >= args[1];
          });

predicate(gt,
          {
              return args[0] > args[1];
          });

predicate(lt,
          {
              return args[0] < args[1];
          });

predicate(leq,
          {
              return args[0] <= args[1];
          });

predicate(multiple,
          {
              int x = (int)args[0].value();
              int y = (int)args[1].value();
              return (x % y) == 0;
          });




#endif
