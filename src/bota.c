#include "bota.h"
#include "utils.h"
#include <stdint.h>

void BOTAContextInit(BOTAContext *ctx)
{
  ctx->lineno = 0;
  ctx->token_start = 0;
  ctx->text_pos = 0;
  ctx->text_length = 0;

  ctx->num_tokens = 0;
  ctx->head = 0;

  ctx->ast_pool = (uint8_t *) calloc(AST_POOL_SIZE, sizeof(uint8_t));
  ctx->pool_capacity = AST_POOL_SIZE;
  ctx->pool_pos = 0;
}

void BOTAContextDestroy(BOTAContext *ctx)
{
 free(ctx->ast_pool);
}

uint32_t BOTAContextAllocate(BOTAContext *ctx, size_t size)
{
  while (ctx->pool_pos + size > ctx->pool_capacity)
  {
    ctx->pool_capacity *= 2;
    uint8_t *new_pool = realloc(ctx->ast_pool, ctx->pool_capacity);
    if (new_pool == NULL)
    {
      printf("Fatal: Out of Memory\n");
      exit(1);
    }
    ctx->ast_pool = new_pool;
  }

  uint32_t ptr = ctx->pool_pos;
  ctx->pool_pos += size;

  return ptr;
}


