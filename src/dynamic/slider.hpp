/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman
   Copyright (c) 2020 Jean Pierre Cimalando

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#pragma once
#include <elements.hpp>

namespace dynamic { namespace elements {

   using namespace cycfi::elements;

   ////////////////////////////////////////////////////////////////////////////
   // Basic Thumb (You can use this as the slider's thumb)
   ////////////////////////////////////////////////////////////////////////////
   class basic_thumb_element : public element
   {
   public:

                              basic_thumb_element(unsigned size,
                                                  color c = colors::black)
                               : _size(size), _color(c)
                              {}

      view_limits             limits(basic_context const& ctx) const override;
      void                    draw(context const& ctx) override;

   private:

      unsigned                _size;
      color                   _color;
   };

   inline view_limits basic_thumb_element
      ::limits(basic_context const& ctx) const
   {
      auto pt = point{ float(_size), float(_size) };
      return view_limits{ pt, pt };
   }

   inline void basic_thumb_element::draw(context const& ctx)
   {
      auto& thm = get_theme();
      auto& cnv = ctx.canvas;
      auto  indicator_color = thm.indicator_color.level(1.5);
      auto  cp = circle{ center_point(ctx.bounds), _size/2.0f };

      draw_thumb(cnv, cp, _color, indicator_color);
   }

   inline basic_thumb_element basic_thumb(unsigned size, color c = colors::black)
   {
      return {size, c};
   }

   ////////////////////////////////////////////////////////////////////////////
   // Basic Track (You can use this as the slider's track)
   ////////////////////////////////////////////////////////////////////////////
   class basic_track_element : public element
   {
   public:

      static unsigned const min_length = 64;

                              basic_track_element(unsigned size, bool vertical,
                                                  color c = colors::black)
                               : _size(size), _vertical(vertical), _color(c)
                              {}

      view_limits             limits(basic_context const& ctx) const override;
      void                    draw(context const& ctx) override;

   private:

      unsigned                _size;
      bool                    _vertical;
      color                   _color;
   };

   inline view_limits basic_track_element
      ::limits(basic_context const& ctx) const
   {
      auto sz = float(_size);
      auto min_length_ = float(min_length);
      auto p1 = _vertical ? point{ sz, min_length_ } : point{ min_length_, sz };
      auto p2 = _vertical ? point{ sz, full_extent } : point{ full_extent, sz };
      return view_limits{ p1, p2 };
   }

   inline void basic_track_element
      ::draw(context const& ctx)
   {
      draw_track(ctx.canvas, ctx.bounds);
   }

   inline basic_track_element
   basic_track(unsigned size, bool vertical = false, color c = colors::black)
   {
      return {size, vertical, c};
   }

} // namespace elements
} // namespace dynamic
