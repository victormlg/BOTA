#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "bota.h"
#include <stdint.h>

static node_ptr BOTAParserProgram(BOTAContext *ctx);
static node_ptr BOTAParserBlock(BOTAContext *ctx);
static node_ptr BOTAParserStatement(BOTAContext *ctx);
static node_ptr BOTAParserRval(BOTAContext *ctx);
static node_ptr BOTAParserEnum(BOTAContext *ctx);
static node_ptr BOTAParserEnumMembers(BOTAContext *ctx);
static node_ptr BOTAParserStruct(BOTAContext *ctx);
static node_ptr BOTAParserStructMembers(BOTAContext *ctx);
static node_ptr BOTAParserStructMember(BOTAContext *ctx);
static node_ptr BOTAParserModule(BOTAContext *ctx);
static node_ptr BOTAParserUse(BOTAContext *ctx);
static node_ptr BOTAParserLval(BOTAContext *ctx);
static node_ptr BOTAParserType(BOTAContext *ctx);
static node_ptr BOTAParserCompoundType(BOTAContext *ctx);
static node_ptr BOTAParserBOTAParserAdvancedType(BOTAContext *ctx);
static node_ptr BOTAParserTypes(BOTAContext *ctx);
static node_ptr BOTAParserPipeExpression(BOTAContext *ctx);
static node_ptr BOTAParserCondition(BOTAContext *ctx);
static node_ptr BOTAParserAndCondition(BOTAContext *ctx);
static node_ptr BOTAParserNotCondition(BOTAContext *ctx);
static node_ptr BOTAParserEquality(BOTAContext *ctx);
static node_ptr BOTAParserComparison(BOTAContext *ctx);
static node_ptr BOTAParserTerm(BOTAContext *ctx);
static node_ptr BOTAParserFactor(BOTAContext *ctx);
static node_ptr BOTAParserUnary(BOTAContext *ctx);
static node_ptr BOTAParserFunctionCompose(BOTAContext *ctx);
static node_ptr BOTAParserPostfix(BOTAContext *ctx);
static node_ptr BOTAParserListGet(BOTAContext *ctx);
static node_ptr BOTAParserObjectGet(BOTAContext *ctx);
static node_ptr BOTAParserValue(BOTAContext *ctx);
static node_ptr BOTAParserFunction(BOTAContext *ctx);
static node_ptr BOTAParserFunctionArgs(BOTAContext *ctx);
static node_ptr BOTAParserFunctionBody(BOTAContext *ctx);
static node_ptr BOTAParserList(BOTAContext *ctx);
static node_ptr BOTAParserListArgs(BOTAContext *ctx);
static node_ptr BOTAParserObject(BOTAContext *ctx);
static node_ptr BOTAParserObjectArgs(BOTAContext *ctx);
static node_ptr BOTAParserObjectArg(BOTAContext *ctx);
static node_ptr BOTAParserFunctionCall(BOTAContext *ctx);
static node_ptr BOTAParserIfExpression(BOTAContext *ctx);
static node_ptr BOTAParserIfBody(BOTAContext *ctx);
static node_ptr BOTAParserElseIf(BOTAContext *ctx);



static inline Token *PeekToken(BOTAContext *ctx, uint8_t idx)
{
  if (idx >= ctx->num_tokens)
  {
    return NULL;
  }

  uint8_t token_idx = (ctx->head + MAX_LOOKAHEAD - 1 - idx) % MAX_LOOKAHEAD;
  return &ctx->token_buffer[token_idx];
}

static node_ptr BOTAParserProgram(BOTAContext *ctx)
{
  /*
  ScanNext(ctx);

  if (PeekToken(ctx, 0) == NULL)
  {
    node_ptr ret = {.ptr = ARENA_NULL};
    return ret;
  }
  */
  return BOTAParserBlock(ctx);
}

static node_ptr BOTAParserBlock(BOTAContext *ctx)
{
  size_t initial_capacity = 4;
  size_t length = 0;
  array_ptr stmt;

  uint32_t *tmp_array = (uint32_t *) calloc(initial_capacity, sizeof(uint32_t)); // Imagine dynamic array

  while (true) // Imagine other condition
  {
    node_ptr stmt = BOTAParserStatement(ctx);
    tmp_array[length++] = stmt;
  }
  array_ptr array = BOTAContextAllocate(ctx, sizeof(uint32_t) + initial_capacity * sizeof(uint32_t));


  ctx->ast_pool[array] = length;
  memcpy(ctx->ast_pool + array + 1, tmp_array, initial_capacity);

  free(tmp_array);

  return BlockNodeNew(ctx, array, BOTAParserRval(ctx), UNKNWN_TYPE);
}

/*
 * Pseudocode for nodes with arrays:
 *
 * 1. allocate dynamic tmp array
 * 2. while...
 * 3. store node inside tmp dynamic array
 * 4. allocate arena array
 * 5. copy tmp array to arena array
 * 6. return node
 *
 *
 *
 *
 */




