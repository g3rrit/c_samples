// test for integer constant expression that returns itself a integer constant expression
// only works with gdb

#define ICE_P(x) (sizeof(int) == sizeof(*(1 ? ((void*)((x) * 0l)) : (int*)1)))

// Ok, so I can see that (void *)((x)*0l)) turns into NULL when x is an
// ICE. Fine. So with a constant, we have

//    sizeof( 1 ? NULL : (int *) 1)

// and the rule is that if one of the sides of a ternary operation with
// pointers is NULL, the end result is the other type (int *).

// So yes, the above returns 'sizeof(int)'.

// And if it is *not* an ICE that first pointer is still of type '(void
// *)', but it is not NULL.

// And yes, the type conversion rules for a ternary op with two non-NULL
// pointers is different,  and it now returns "void *".

// So now the end result is (sizeof(*(void *)(x)), which on gcc is
// generally *different* from 'int'.

// So I see two issues:

//  - "sizeof(*(void *)1)" is not necessalily well-defined. For gcc it is
// 1. But it could cause warnings.

//  - this will break the minds of everybody who ever sees that expression.

// Those two issues might be fine, though.

// > This also does not evaluate x itself on gcc although this is
// > not guaranteed by the standard. (And I haven't tried any older
// > gcc.)

// Oh, I think it's guaranteed by the standard that 'sizeof()' doesn't
// evaluate the argument value, only the type.

// I'm in awe of your truly marvelously disgusting hack. That is truly a
// work of art.

// I'm sure it doesn't work or causes warnings for various reasons, but
// it's still a thing of beaty.

//               Linus

