﻿/**
 * File:   window_animator_vtranslate.h
 * Author: AWTK Develop Team
 * Brief:  vertical translate window animator
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-04-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/window_animator.h"

#ifndef TK_WINDOW_ANIMATOR_VTRANSLATE_H
#define TK_WINDOW_ANIMATOR_VTRANSLATE_H

BEGIN_C_DECLS

#define WINDOW_ANIMATOR_VTRANSLATE_PROP_PREV_WIN_INDEX "prev_win_index"
#define WINDOW_ANIMATOR_VTRANSLATE_PROP_PREV_WIN_Y_RANGE "prev_win_y_range"

window_animator_t* window_animator_vtranslate_create(bool_t open, tk_object_t* args);

END_C_DECLS

#endif /*TK_WINDOW_ANIMATOR_VTRANSLATE_H*/
