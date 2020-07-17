/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman
   Copyright (c) 2020 Jean Pierre Cimalando

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#pragma once
#include <elements.hpp>

namespace dynamic { namespace elements {

   using namespace cycfi;
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

   ////////////////////////////////////////////////////////////////////////////
   // Slider Element Base (common base class for slider elements)
   ////////////////////////////////////////////////////////////////////////////
   template <typename Subject>
   class slider_element_base : public proxy<Subject>
   {
   public:

      using base_type = proxy<Subject>;

                              slider_element_base(Subject subject, unsigned size);

      view_limits             limits(basic_context const& ctx) const override;
      void                    prepare_subject(context& ctx) override;

   protected:

      unsigned                _size;
   };

   template <typename Subject>
   slider_element_base<Subject>::slider_element_base(Subject subject, unsigned size)
      : base_type(std::move(subject)), _size(size)
   {
   }

   template <typename Subject>
   inline view_limits
   slider_element_base<Subject>::limits(basic_context const& ctx) const
   {
      auto sl = this->subject().limits(ctx);
      if (sl.min.x < sl.min.y) // is vertical?
      {
         sl.min.x += _size;
         sl.max.x += _size;
         clamp_max(sl.max.x, full_extent);
      }
      else
      {
         sl.min.y += _size;
         sl.max.y += _size;
         clamp_max(sl.max.y, full_extent);
      }
      return sl;
   }

   template <typename Subject>
   inline void
   slider_element_base<Subject>::prepare_subject(context& ctx)
   {
      if (ctx.bounds.width() < ctx.bounds.height()) // is vertical?
      {
         ctx.bounds.left += _size/2;
         ctx.bounds.right -= _size/2;
      }
      else
      {
         ctx.bounds.top += _size/2;
         ctx.bounds.bottom -= _size/2;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   // Slider Marks (You can use this to place tick marks on slider)
   ////////////////////////////////////////////////////////////////////////////
   template <typename Subject>
   class slider_marks_element : public slider_element_base<Subject>
   {
   public:

                              slider_marks_element(
                                 Subject subject, std::size_t size,
                                 std::size_t num_divs, std::size_t major_divs);

      using base_type = slider_element_base<Subject>;

      void                    draw(context const& ctx) override;

   private:

      std::size_t             _num_divs;
      std::size_t             _major_divs;
   };

   template <typename Subject>
   slider_marks_element<Subject>::
   slider_marks_element(Subject subject, std::size_t size,
                        std::size_t num_divs, std::size_t major_divs)
      : base_type(std::move(subject), size)
      , _num_divs(num_divs), _major_divs(major_divs)
   {
   }

   template <typename Subject>
   inline void
   slider_marks_element<Subject>
      ::draw(context const& ctx)
   {
      // Draw linear lines
      draw_slider_marks(
         ctx.canvas, ctx.bounds, base_type::_size, _num_divs
       , _major_divs, colors::light_gray);

      // Draw the subject
      base_type::draw(ctx);
   }

   template <typename Subject>
   inline slider_marks_element<remove_cvref_t<Subject>>
   slider_marks(Subject&& subject, std::size_t _size,
                std::size_t _num_divs = 50, std::size_t _major_divs = 10)
   {
      return {std::forward<Subject>(subject), _size, _num_divs, _major_divs};
   }

   ////////////////////////////////////////////////////////////////////////////
   // Slider Labels (You can use this to place slider labels with sliders)
   ////////////////////////////////////////////////////////////////////////////
   template <typename Subject>
   class slider_labels_element
    : public slider_element_base<Subject>
   {
   public:

      using base_type = slider_element_base<Subject>;
      using string_vector = std::vector<std::string>;

                              slider_labels_element(
                                 Subject subject
                               , unsigned size
                               , float font_size
                              )
                               : base_type(std::move(subject), size)
                               , _font_size(font_size)
                              {}

      void                    draw(context const& ctx) override;

      string_vector           _labels;
      float                   _font_size;
   };

   template <typename Subject>
   inline void
   slider_labels_element<Subject>::draw(context const& ctx)
   {
      // Draw the subject
      base_type::draw(ctx);

      // Draw the labels
      draw_slider_labels(
         ctx.canvas, ctx.bounds, base_type::_size
       , _font_size, _labels.data(), _labels.size());
   }

   template <typename Subject, typename... S>
   inline slider_labels_element<remove_cvref_t<Subject>>
   slider_labels(Subject&& subject, unsigned size, float font_size, S&&... s)
   {
      auto r = slider_labels_element<remove_cvref_t<Subject>>
         {std::forward<Subject>(subject), size, font_size};
      r._labels = {{ std::move(s)... }};
      return r;
   }

} // namespace elements
} // namespace dynamic
