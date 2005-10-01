/* import.c -  encrypt functions
 *	Copyright (C) 2000 Werner Koch (dd9jn)
 *      Copyright (C) 2001 g10 Code GmbH
 *
 * This file is part of GPGME.
 *
 * GPGME is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GPGME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util.h"
#include "context.h"
#include "ops.h"

static void
import_status_handler ( GpgmeCtx ctx, GpgStatusCode code, char *args )
{
    DEBUG2 ("import_status: code=%d args=`%s'\n", code, args );
    /* FIXME: We have to check here whether the import actually worked 
     * and maybe it is a good idea to save some statistics and provide
     * a progress callback */
}

GpgmeError
gpgme_op_import_start (GpgmeCtx ctx, GpgmeData keydata)
{
  int err = 0;

  fail_on_pending_request (ctx);
  ctx->pending = 1;

  _gpgme_engine_release (ctx->engine);
  ctx->engine = NULL;
  err = _gpgme_engine_new (ctx->use_cms ? GPGME_PROTOCOL_CMS
			   : GPGME_PROTOCOL_OpenPGP, &ctx->engine);
  if (err)
    goto leave;

  /* Check the supplied data */
  if (gpgme_data_get_type (keydata) == GPGME_DATA_TYPE_NONE)
    {
      err = mk_error (No_Data);
      goto leave;
    }
  _gpgme_data_set_mode (keydata, GPGME_DATA_MODE_OUT);

  _gpgme_engine_set_status_handler (ctx->engine, import_status_handler, ctx);
  _gpgme_engine_set_verbosity (ctx->engine, ctx->verbosity);

  _gpgme_engine_op_import (ctx->engine, keydata);

  if (!err)
    err = _gpgme_engine_start (ctx->engine, ctx);

 leave:
  if (err)
    {
      ctx->pending = 0;
      _gpgme_engine_release (ctx->engine);
      ctx->engine = NULL;
    }
  return err;
}

/**
 * gpgme_op_import:
 * @c: Context 
 * @keydata: Data object
 * 
 * Import all key material from @keydata into the key database.
 * 
 * Return value: o on success or an error code.
 **/
GpgmeError
gpgme_op_import ( GpgmeCtx c, GpgmeData keydata )
{
    int rc = gpgme_op_import_start ( c, keydata );
    if ( !rc ) {
        gpgme_wait (c, 1);
        c->pending = 0;
    }
    return rc;
}



