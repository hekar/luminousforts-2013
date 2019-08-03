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

#include "cbase.h"

NK_API void
nk_vgui_draw_text(const struct vgui_context *vguictx,
    const struct nk_user_font *font, const struct nk_rect rect,
    const char *text, const int len, const float font_height,
    const struct nk_color fg)
{
    float x = 0;
    int text_len = 0;
    nk_rune unicode = 0;
    nk_rune next = 0;
    int glyph_len = 0;
    int next_glyph_len = 0;
    struct nk_user_font_glyph g;
    if (!len || !text) return;

    x = 0;
    glyph_len = nk_utf_decode(text, &unicode, len);
    if (!glyph_len) return;

    /* draw every glyph image */
    while (text_len < len && glyph_len) {
        struct nk_rect src_rect;
        struct nk_rect dst_rect;
        float char_width = 0;
        if (unicode == NK_UTF_INVALID) break;

        /* query currently drawn glyph information */
        next_glyph_len = nk_utf_decode(text + text_len + glyph_len, &next, (int)len - text_len);
        nk_vgui_font_query_font_glyph(font->userdata, font_height, &g, unicode,
                    (next == NK_UTF_INVALID) ? '\0' : next);

        /* calculate and draw glyph drawing rectangle and image */
        char_width = g.xadvance;
        src_rect.x = g.uv[0].x * vguictx->font_tex.w;
        src_rect.y = g.uv[0].y * vguictx->font_tex.h;
        src_rect.w = g.uv[1].x * vguictx->font_tex.w - g.uv[0].x * vguictx->font_tex.w;
        src_rect.h = g.uv[1].y * vguictx->font_tex.h - g.uv[0].y * vguictx->font_tex.h;

        dst_rect.x = x + g.offset.x + rect.x;
        dst_rect.y = g.offset.y + rect.y;
        dst_rect.w = ceilf(g.width);
        dst_rect.h = ceilf(g.height);

        /* TODO: account fg */
        /* Use software rescaling to blit glyph from font_text to framebuffer */
        nk_vgui_stretch_image(&vguictx->fb, &vguictx->font_tex, &dst_rect, &src_rect, &vguictx->scissors);

        /* offset next glyph */
        text_len += glyph_len;
        x += char_width;
        glyph_len = next_glyph_len;
        unicode = next;
    }
}

NK_API void
nk_vgui_drawimage(const struct vgui_context *vguictx,
    const int x, const int y, const int w, const int h,
    const struct nk_image *img, const struct nk_color *col)
{
    struct nk_rect src_rect;
    struct nk_rect dst_rect;

    src_rect.x = img->region[0];
    src_rect.y = img->region[1];
    src_rect.w = img->region[2];
    src_rect.h = img->region[3];

    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = w;
    dst_rect.h = h;
    nk_vgui_stretch_image(&vguictx->fb, &vguictx->font_tex, &dst_rect, &src_rect, &vguictx->scissors);
}

NK_API void
nk_vgui_shutdown(struct vgui_context *vguictx)
{
    nk_free(&vguictx->ctx);
    nk_memset(vguictx, 0, sizeof(struct vgui_context));
    free(vguictx);
}

NK_API void
nk_vgui_resize_fb(struct vgui_context *vguictx,
                   void *fb,
                   const unsigned int w,
                   const unsigned int h,
                   const unsigned int pitch)
{
    vguictx->fb.w = w;
    vguictx->fb.h = h;
    vguictx->fb.pixels = fb;
    vguictx->fb.pitch = pitch;
}

NK_API void
nk_vgui_render(const struct vgui_context *vguictx,
                const struct nk_color clear,
                const unsigned char enable_clear)
{
    const struct nk_command *cmd;
    if (enable_clear)
        nk_vgui_clear(vguictx, clear);

    nk_foreach(cmd, (struct nk_context*)&vguictx->ctx) {
        switch (cmd->type) {
        case NK_COMMAND_NOP: break;
        case NK_COMMAND_SCISSOR: {
            const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
            nk_vgui_scissor((struct vgui_context *)vguictx, s->x, s->y, s->w, s->h);
        } break;
        case NK_COMMAND_LINE: {
            const struct nk_command_line *l = (const struct nk_command_line *)cmd;

            nk_vgui_stroke_line(vguictx, l->begin.x, l->begin.y, l->end.x,
                l->end.y, l->line_thickness, l->color);
        } break;
        case NK_COMMAND_RECT: {
            const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
            nk_vgui_stroke_rect(vguictx, r->x, r->y, r->w, r->h,
                (unsigned short)r->rounding, r->line_thickness, r->color);
        } break;
        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
            nk_vgui_fill_rect(vguictx, r->x, r->y, r->w, r->h,
                (unsigned short)r->rounding, r->color);
        } break;
        case NK_COMMAND_CIRCLE: {
            const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
            nk_vgui_stroke_circle(vguictx, c->x, c->y, c->w, c->h, c->line_thickness, c->color);
        } break;
        case NK_COMMAND_CIRCLE_FILLED: {
            const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
            nk_vgui_fill_circle(vguictx, c->x, c->y, c->w, c->h, c->color);
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
            nk_vgui_stroke_triangle(vguictx, t->a.x, t->a.y, t->b.x, t->b.y,
                t->c.x, t->c.y, t->line_thickness, t->color);
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
            nk_vgui_fill_triangle(vguictx, t->a.x, t->a.y, t->b.x, t->b.y,
                t->c.x, t->c.y, t->color);
        } break;
        case NK_COMMAND_POLYGON: {
            const struct nk_command_polygon *p =(const struct nk_command_polygon*)cmd;
            nk_vgui_stroke_polygon(vguictx, p->points, p->point_count, p->line_thickness,p->color);
        } break;
        case NK_COMMAND_POLYGON_FILLED: {
            const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled *)cmd;
            nk_vgui_fill_polygon(vguictx, p->points, p->point_count, p->color);
        } break;
        case NK_COMMAND_POLYLINE: {
            const struct nk_command_polyline *p = (const struct nk_command_polyline *)cmd;
            nk_vgui_stroke_polyline(vguictx, p->points, p->point_count, p->line_thickness, p->color);
        } break;
        case NK_COMMAND_TEXT: {
            const struct nk_command_text *t = (const struct nk_command_text*)cmd;
            nk_vgui_draw_text(vguictx, t->font, nk_rect(t->x, t->y, t->w, t->h),
                t->string, t->length, t->height, t->foreground);
        } break;
        case NK_COMMAND_CURVE: {
            const struct nk_command_curve *q = (const struct nk_command_curve *)cmd;
            nk_vgui_stroke_curve(vguictx, q->begin, q->ctrl[0], q->ctrl[1],
                q->end, 22, q->line_thickness, q->color);
        } break;
        case NK_COMMAND_RECT_MULTI_COLOR:
        case NK_COMMAND_IMAGE: {
            const struct nk_command_image *q = (const struct nk_command_image *)cmd;
            nk_vgui_drawimage(vguictx, q->x, q->y, q->w, q->h, &q->img, &q->col);
        } break;
        case NK_COMMAND_ARC: {
            assert(0 && "NK_COMMAND_ARC not implemented\n");
        } break;
        case NK_COMMAND_ARC_FILLED: {
            assert(0 && "NK_COMMAND_ARC_FILLED not implemented\n");
        } break;
        default: break;
        }
    } nk_clear((struct nk_context*)&vguictx->ctx);
}
