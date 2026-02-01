#include "ast.h"
#include "bota.h"


NodeRef BlockNodeNew(BOTAContext *ctx, ArrayRef stmts, NodeRef return_stmt, NodeRef return_type)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(BlockNode));

  BlockNode *block = (BlockNode *) &ctx->ast_pool[node_idx];
  Type *block_type = (Type *) &ctx->ast_pool[return_type.ptr];

  block->stmts = stmts;
  block->return_stmt = return_stmt;
  block->return_type.type = block_type->type;
  block->return_type.sub_types = block_type->sub_types;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef StmtNodeNew(BOTAContext *ctx, NodeRef stmt_val, StmtType type)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(StmtNode));
  StmtNode *stmt = (StmtNode *) &ctx->ast_pool[node_idx];
  stmt->stmt = stmt_val;
  stmt->type = type;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef ArgNodeNew(BOTAContext *ctx, StringRef name, NodeRef rval, NodeRef type)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(ArgNode));


  ArgNode *arg = (ArgNode *) &ctx->ast_pool[node_idx];
  Type *arg_type = (Type *) &ctx->ast_pool[type.ptr];
  arg->name = name;
  arg->rval = rval;
  arg->type.type = arg_type->type;
  arg->type.sub_types = arg_type->sub_types;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef FunctionDefNodeNew(BOTAContext *ctx, StringRef name, ArrayRef args, NodeRef block)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(StmtNode));
  FunctionDefNode *fdef = (FunctionDefNode *) &ctx->ast_pool[node_idx];

  fdef->name = name;
  fdef->args = args;
  fdef->block = block;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef FunctionCallNodeNew(BOTAContext *ctx, StringRef name, ArrayRef pos_args, ArrayRef key_args)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(StmtNode));
  FunctionCallNode *fcall = (FunctionCallNode *) &ctx->ast_pool[node_idx];

  fcall->name = name;
  fcall->key_args = key_args;
  fcall->pos_args = pos_args;
  
  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef ElseIfNodeNew(BOTAContext *ctx, NodeRef condition, NodeRef block)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(ElseIfNode));
  ElseIfNode *else_if = (ElseIfNode *) &ctx->ast_pool[node_idx];

  else_if->condition = condition;
  else_if->block = block;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef IfNodeNew(BOTAContext *ctx, NodeRef condition, NodeRef if_block, ArrayRef else_if_nodes, NodeRef else_block)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(IfNode));
  IfNode *if_node = (IfNode *) &ctx->ast_pool[node_idx];

  if_node->condition = condition;
  if_node->if_block = if_block;
  if_node->else_if_nodes = else_if_nodes;
  if_node->else_block = else_block;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef PipeNodeNew(BOTAContext *ctx, ArrayRef piped_values)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(PipeNode));
  
  PipeNode *pipe = (PipeNode *) &ctx->ast_pool[node_idx]; 
  pipe->piped_values = piped_values;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef ModuleUseNodeNew(BOTAContext *ctx, StringRef module_name)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(ModuleUseNode));

  ModuleUseNode *use = (ModuleUseNode *) &ctx->ast_pool[node_idx];

  use->module_name = module_name;
  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef ModuleDefNodeNew(BOTAContext *ctx, StringRef name, ArrayRef stmts)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(ModuleDefNode));
  
  ModuleDefNode *module = (ModuleDefNode *) &ctx->ast_pool[node_idx];

  module->name = name;
  module->stmts = stmts;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef EnumDefNodeNew(BOTAContext *ctx, StringRef name, ArrayRef labels)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(EnumDefNode));

  EnumDefNode *enum_def = (EnumDefNode *) &ctx->ast_pool[node_idx];

  enum_def->name = name;
  enum_def->labels = labels;
  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef MemberNodeNew(BOTAContext *ctx, StringRef name, NodeRef type)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(MemberNode));

  MemberNode *member = (MemberNode *) &ctx->ast_pool[node_idx];
  Type *member_type = (Type *) &ctx->ast_pool[type.ptr];

  member->name = name;
  member->type.type = member_type->type;
  member->type.sub_types = member_type->sub_types;
  
  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef StructDefNodeNew(BOTAContext *ctx, StringRef name, ArrayRef members)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(StructDefNode));

  StructDefNode *struct_def = (StructDefNode *) &ctx->ast_pool[node_idx];

  struct_def->name = name;
  struct_def->members = members;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef VariableDefNodeNew(BOTAContext *ctx, StringRef name, NodeRef rval, NodeRef type)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(VariableDefNode));

  VariableDefNode *variable = (VariableDefNode *) &ctx->ast_pool[node_idx];
  Type *var_type = (Type *) &ctx->ast_pool[type.ptr];

  variable->name = name;
  variable->rval = rval;
  variable->type.type = var_type->type;
  variable->type.sub_types = var_type->sub_types;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef ListNewNodeNew(BOTAContext *ctx, NodeRef type, ArrayRef rvals)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(ListNewNode));

  ListNewNode *list_new = (ListNewNode *) &ctx->ast_pool[node_idx];
  Type *list_type = (Type *) &ctx->ast_pool[type.ptr];

  list_new->type.type = list_type->type;
  list_new->type.sub_types = list_type->sub_types;
  list_new->rvals = rvals;
  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef ObjectNewNodeNew(BOTAContext *ctx, ArrayRef args)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(ObjectNewNode));
  ObjectNewNode *object_new = (ObjectNewNode *) &ctx->ast_pool[node_idx];

  object_new->args = args;
  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef RvalNodeNew(BOTAContext *ctx, NodeRef value, RvalType type)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(RvalNode));
  RvalNode *rval = (RvalNode *) &ctx->ast_pool[node_idx];

  rval->type = type;
  rval->value = value;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef BinaryOperationNodeNew(BOTAContext *ctx, NodeRef left, BinaryOperationType type, NodeRef right)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(BinaryOperationNode));
  BinaryOperationNode *node = (BinaryOperationNode *) &ctx->ast_pool[node_idx];

  node->left = left;
  node->right = right;
  node->type = type;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef UnaryOperationNodeNew(BOTAContext *ctx, UnaryOperationType type, NodeRef next)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(UnaryOperationNode));
  UnaryOperationNode *node = (UnaryOperationNode *) &ctx->ast_pool[node_idx];

  node->type = type;
  node->next = next;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef ListGetNodeNew(BOTAContext *ctx, arena_ptr index, NodeRef container)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(ListGetNode));
  ListGetNode *node = (ListGetNode*) &ctx->ast_pool[node_idx];

  node->container = container;
  node->index = index;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}

NodeRef ObjectGetNodeNew(BOTAContext *ctx, StringRef label, NodeRef container)
{
  arena_ptr node_idx = (arena_ptr) BOTAContextAllocate(ctx, sizeof(ObjectGetNode));
  ObjectGetNode *node = (ObjectGetNode *) &ctx->ast_pool[node_idx];

  node->container = container;
  node->label = label;

  NodeRef ret = {.ptr = node_idx};
  return ret;
}



