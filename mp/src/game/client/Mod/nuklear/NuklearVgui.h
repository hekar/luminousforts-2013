/* ***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1/LGPL 2.1/GPL 2.0

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
...
for the specific language governing rights and limitations under the
License.

The Original Code is for LuminousForts.

The Initial Developer of the Original Code is Hekar Khani.
Portions created by the Hekar Khani are Copyright (C) 2010
Hekar Khani. All Rights Reserved.

Contributor(s):
  Hekar Khani <hekark@gmail.com>

Alternatively, the contents of this file may be used under the terms of
either of the GNU General Public License Version 2 or later (the "GPL"),
...
the terms of any one of the MPL, the GPL or the LGPL.

***** END LICENSE BLOCK ***** */

#include "nuklear.h"

struct vgui_context {
    struct nk_context ctx;
    struct nk_rect scissors;
    struct nk_font_atlas atlas;
};

NK_API void
nk_vgui_draw_text(const struct vgui_context *vguictx,
    const struct nk_user_font *font, const struct nk_rect rect,
    const char *text, const int len, const float font_height,
    const struct nk_color fg);

NK_API void
nk_vgui_drawimage(const struct vgui_context *vguictx,
    const int x, const int y, const int w, const int h,
    const struct nk_image *img, const struct nk_color *col);

NK_API void
nk_vgui_shutdown(struct vgui_context *vguictx);

NK_API void
nk_vgui_resize_fb(struct vgui_context *vguictx,
                   void *fb,
                   const unsigned int w,
                   const unsigned int h,
                   const unsigned int pitch);

NK_API void
nk_vgui_render(const struct vgui_context *vguictx,
                const struct nk_color clear,
                const unsigned char enable_clear);
