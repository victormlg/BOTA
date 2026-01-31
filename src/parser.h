#include "utils.h"

#ifndef PARSER_H
#define PARSER_H

#define ARENA_NULL 0

// Convention: arena_ptr of 0 means null/absent
typedef uint32_t arena_ptr;

typedef struct {
  arena_ptr ptr;
} StringRef;

typedef struct {
  arena_ptr ptr;
} ArrayRef;

typedef struct {
  arena_ptr ptr;
} NodeRef;

typedef enum {
  TYPE_STRING,
  TYPE_INTEGER,
  TYPE_FLOAT,
  TYPE_BOOL,
  TYPE_FUNCTION,
  TYPE_DURATION,
  TYPE_TIMESTAMP,
  TYPE_SCHEDULE,
  TYPE_RESULT, // compound types
  TYPE_OPTION,
  TYPE_ASYNC,
  TYPE_LIST,
  TYPE_OBJECT
} TypeType;

typedef struct {
  TypeType type;
  ArrayRef sub_types; // [Type]
} Type;

typedef struct {
  ArrayRef stmts; // [StmtNode]
  NodeRef return_stmt; // RvalNode
  Type return_type;
} BlockNode;

typedef enum {
  STMT_VARIABLE_DEF,
  STMT_RVAL,
  STMT_ENUM_DEF,
  STMT_STRUCT_DEF,
  STMT_MODULE_DEF,
  STMT_USE_MODULE
} StmtType;

typedef struct {
  StmtType type;
  NodeRef stmt; // Any of (EnumDefNode, StructDefNode, ModuleUseNode, ModuleDefNode, VariableDefNode, RvalNode)
} StmtNode;

typedef struct {
  StringRef name;
  NodeRef rval; // RvalNode
  Type type;
} ArgNode;

typedef struct {
  StringRef name;
  ArrayRef args; // [ArgNode]
  NodeRef block; // BlockNode
} FunctionDefNode;

typedef struct {
  StringRef name;
  ArrayRef pos_args; // [Arg]
  ArrayRef key_args; // [Arg]
} FunctionCallNode;

typedef struct {
  NodeRef condition; // RvalNode
  NodeRef block; // BlockNode
} ElseIfNode;

typedef struct {
  NodeRef condition; // RvalNode
  NodeRef if_block; // BlockNode
  ArrayRef else_if_nodes; // [ElseIfNode]
  NodeRef else_block; // BlockNode
} IfNode;

typedef struct {
  ArrayRef piped_values; // [RvalNode]
} PipeNode;

typedef struct {
  StringRef module_name;
} ModuleUseNode;

typedef struct {
  StringRef name;
  ArrayRef stmts; // [StmtNode]
} ModuleDefNode;

typedef struct {
  StringRef name;
  ArrayRef labels; // [StringRef]
} EnumDefNode;

typedef struct {
  StringRef name;
  Type type;
} MemberNode;

typedef struct {
  StringRef name;
  ArrayRef members; // [MemberNode]
} StructDefNode;

typedef struct {
  StringRef name;
  NodeRef rval; // RvalNode
  Type type;
} VariableDefNode;

typedef struct {
  Type type;
  ArrayRef rvals; // [RvalNode]
} ListNewNode;

typedef struct {
  Type type;
  ArrayRef args; // [ArgNode]
} ObjectNewNode;

typedef enum {
  RVAL_FUNCTION_DEF,
  RVAL_FUNCTION_CALL,
  RVAL_LIST_NEW,
  RVAL_OBJECT_NEW,
  RVAL_IF_EXPR,
  RVAL_VARIABLE_DEREF,
  RVAL_STRING,
  RVAL_INTEGER,
  RVAL_FLOAT,
  RVAL_PATH,
  RVAL_DURATION,
  RVAL_TIMESTAMP,
  RVAL_SCHEDULE,
  RVAL_TRUE,
  RVAL_FALSE,
  RVAL_UNARY_OP,
  RVAL_BINARY_OP,
  RVAL_LIST_GET,
  RVAL_OBJECT_GET,
  RVAL_PIPE
} RvalType;

typedef struct {
  RvalType type;
  NodeRef value; // Any of the listed types
} RvalNode;

typedef enum {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_COMPOSE,
  OP_AND,
  OP_OR,
  OP_GT,
  OP_GE,
  OP_LT,
  OP_LE,
  OP_EE,
  OP_NE,
} BinaryOperationType;

typedef struct {
  BinaryOperationType type;
  NodeRef left; // RvalNode
  NodeRef right; // RvalNode
} BinaryOperationNode;

typedef enum {
  OP_NOT,
  OP_MINUS
} UnaryOperationType;

typedef struct {
  UnaryOperationType type;
  NodeRef next; // RvalNode
} UnaryOperationNode;

typedef struct {
  arena_ptr index;
  NodeRef container; // RvalNode
} ListGetNode;

typedef struct {
  StringRef label;
  NodeRef container; // RvalNode
} ObjectGetNode;

#endif
