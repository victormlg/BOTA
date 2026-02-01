#include "utils.h"
#include "bota.h"

#ifndef AST_H
#define AST_H

#define ARENA_NULL 0

// Convention: uint32_t of 0 means null/absent
typedef uint32_t array_ptr;
typedef uint32_t string_ptr;
typedef uint32_t node_ptr;

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
  array_ptr sub_types; // [Type]
} TypeNode;

typedef struct {
  array_ptr stmts; // [StmtNode]
  node_ptr return_stmt; // RvalNode
  node_ptr return_type;
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
  node_ptr stmt; // Any of (EnumDefNode, StructDefNode, ModuleUseNode, ModuleDefNode, VariableDefNode, RvalNode)
} StmtNode;

typedef struct {
  string_ptr name;
  node_ptr rval; // RvalNode
  node_ptr type;
} ArgNode;

typedef struct {
  string_ptr name;
  array_ptr args; // [ArgNode]
  node_ptr block; // BlockNode
} FunctionDefNode;

typedef struct {
  string_ptr name;
  array_ptr pos_args; // [Arg]
  array_ptr key_args; // [Arg]
} FunctionCallNode;

typedef struct {
  node_ptr condition; // RvalNode
  node_ptr block; // BlockNode
} ElseIfNode;

typedef struct {
  node_ptr condition; // RvalNode
  node_ptr if_block; // BlockNode
  array_ptr else_if_nodes; // [ElseIfNode]
  node_ptr else_block; // BlockNode
} IfNode;

typedef struct {
  array_ptr piped_values; // [RvalNode]
} PipeNode;

typedef struct {
  string_ptr module_name;
} ModuleUseNode;

typedef struct {
  string_ptr name;
  array_ptr stmts; // [StmtNode]
} ModuleDefNode;

typedef struct {
  string_ptr name;
  array_ptr labels; // [string_ptr]
} EnumDefNode;

typedef struct {
  string_ptr name;
  node_ptr type;
} MemberNode;

typedef struct {
  string_ptr name;
  array_ptr members; // [MemberNode]
} StructDefNode;

typedef struct {
  string_ptr name;
  node_ptr rval; // RvalNode
  node_ptr type;
} VariableDefNode;

typedef struct {
  node_ptr type;
  array_ptr rvals; // [RvalNode]
} ListNewNode;

typedef struct {
  array_ptr args; // [ArgNode]
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
  node_ptr value; // Any of the listed types
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
  node_ptr left; // RvalNode
  node_ptr right; // RvalNode
} BinaryOperationNode;

typedef enum {
  OP_NOT,
  OP_MINUS
} UnaryOperationType;

typedef struct {
  UnaryOperationType type;
  node_ptr next; // RvalNode
} UnaryOperationNode;

typedef struct {
  uint32_t index;
  node_ptr container; // RvalNode
} ListGetNode;

typedef struct {
  string_ptr label;
  node_ptr container; // RvalNode
} ObjectGetNode;


node_ptr BlockNodeNew(BOTAContext *ctx, array_ptr stmts, node_ptr return_stmt, node_ptr return_type);
node_ptr StmtNodeNew(BOTAContext *ctx, node_ptr stmt_val, StmtType type);
node_ptr ArgNodeNew(BOTAContext *ctx, string_ptr name, node_ptr rval, node_ptr type);
node_ptr FunctionDefNodeNew(BOTAContext *ctx, string_ptr name, array_ptr args, node_ptr block);
node_ptr FunctionCallNodeNew(BOTAContext *ctx, string_ptr name, array_ptr pos_args, array_ptr key_args);
node_ptr ElseIfNodeNew(BOTAContext *ctx, node_ptr condition, node_ptr block);
node_ptr IfNodeNew(BOTAContext *ctx, node_ptr condition, node_ptr if_block, array_ptr else_if_nodes, node_ptr else_block);
node_ptr PipeNodeNew(BOTAContext *ctx, array_ptr piped_values);
node_ptr ModuleUseNodeNew(BOTAContext *ctx, string_ptr module_name);
node_ptr ModuleUseNodeNew(BOTAContext *ctx, string_ptr module_name);
node_ptr ModuleDefNodeNew(BOTAContext *ctx, string_ptr name, array_ptr stmts);
node_ptr EnumDefNodeNew(BOTAContext *ctx, string_ptr name, array_ptr labels);
node_ptr MemberNodeNew(BOTAContext *ctx, string_ptr name, node_ptr type);
node_ptr StructDefNodeNew(BOTAContext *ctx, string_ptr name, array_ptr members);
node_ptr VariableDefNodeNew(BOTAContext *ctx, string_ptr name, node_ptr rval, node_ptr type);
node_ptr ListNewNodeNew(BOTAContext *ctx, node_ptr type, array_ptr rvals);
node_ptr ObjectNewNodeNew(BOTAContext *ctx, array_ptr args);
node_ptr RvalNodeNew(BOTAContext *ctx, node_ptr value, RvalType type);
node_ptr BinaryOperationNodeNew(BOTAContext *ctx, node_ptr left, BinaryOperationType type, node_ptr right);
node_ptr UnaryOperationNodeNew(BOTAContext *ctx, UnaryOperationType type, node_ptr next);
node_ptr ListGetNodeNew(BOTAContext *ctx, uint32_t index, node_ptr container);
node_ptr ObjectGetNodeNew(BOTAContext *ctx, string_ptr label, node_ptr container);
#endif
