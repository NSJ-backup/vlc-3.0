/*****************************************************************************
 * block.h
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: vlc_block.h,v 1.1 2003/08/23 22:49:50 fenrir Exp $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

#ifndef _BLOCK_H
#define _BLOCK_H 1


/*
 * block
 */
typedef struct block_sys_t block_sys_t;

struct block_t
{
    block_t     *p_next;

    vlc_bool_t  b_frame_display;
    vlc_bool_t  b_frame_start;
    mtime_t     i_pts;
    mtime_t     i_dts;

    int         i_buffer;
    uint8_t     *p_buffer;

    void        (*pf_release)   ( block_t * );

    block_t    *(*pf_modify)    ( block_t *, vlc_bool_t );
    block_t    *(*pf_duplicate) ( block_t * );
    block_t    *(*pf_realloc)   ( block_t *, int i_prebody, int i_body );

    /* Following fields are private, user should never touch it */
    /* XXX never touch that OK !!! the first that access that will
     * have cvs account removed ;) XXX */

    /* It's an object that should be valid as long as the block_t is valid */
    /* It should become a true block manager to reduce malloc/free */
    vlc_object_t    *p_manager;

    /* private member for block_New, .... manager */
    block_sys_t *p_sys;
};

struct block_fifo_t
{
    vlc_mutex_t         lock;                         /* fifo data lock */
    vlc_cond_t          wait;         /* fifo data conditional variable */

    int                 i_depth;
    block_t             *p_first;
    block_t             **pp_last;
};

/*
 * block
 */
#define block_New( a, b ) __block_New( VLC_OBJECT(a), b )
VLC_EXPORT( block_t *,  __block_New,        ( vlc_object_t *, int ) );
static inline void block_Release( block_t *p_block )
{
    p_block->pf_release( p_block );
}
static inline block_t *block_Modify( block_t *p_block, vlc_bool_t b_willmodify )
{
    return p_block->pf_modify( p_block, b_willmodify );
}
static inline block_t *block_Duplicate( block_t *p_block )
{
    return p_block->pf_duplicate( p_block );
}
static inline block_t *block_Realloc( block_t *p_block, int i_pre, int i_body )
{
    return p_block->pf_realloc( p_block, i_pre, i_body );
}
VLC_EXPORT( void,       block_ChainAppend,  ( block_t **, block_t * ) );
VLC_EXPORT( void,       block_ChainRelease, ( block_t * ) );
VLC_EXPORT( int,        block_ChainExtract, ( block_t *, void *, int ) );
VLC_EXPORT( block_t *,  block_ChainGather,  ( block_t * ) );

/* a bit special, only for new/other block manager */
VLC_EXPORT( block_t *,  block_NewEmpty,     ( void ) );

#define block_FifoNew( a ) __block_FifoNew( VLC_OBJECT(a) )
VLC_EXPORT( block_fifo_t *, __block_FifoNew,    ( vlc_object_t * ) );
VLC_EXPORT( void,           block_FifoRelease,  ( block_fifo_t * ) );
VLC_EXPORT( void,           block_FifoEmpty,    ( block_fifo_t * ) );
VLC_EXPORT( int,            block_FifoPut,      ( block_fifo_t *, block_t * ) );
VLC_EXPORT( block_t *,      block_FifoGet,      ( block_fifo_t * ) );
VLC_EXPORT( block_t *,      block_FifoGetFrame, ( block_fifo_t * ) );
VLC_EXPORT( block_t *,      block_FifoShow,     ( block_fifo_t * ) );

#endif
