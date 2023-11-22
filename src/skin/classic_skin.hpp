#pragma once

#include "skin/clock_skin.hpp"

#include <memory>

#include "clock/datetime_formatter.hpp"
#include "core/linear_layout.hpp"
#include "render/effects/composite.hpp"
#include "skin/renderable_factory.hpp"

// TODO: consider to add setter for factory object -
// this will allow to change classic skins only with changing factory
class ClassicSkin : public ClockSkin {
public:
  explicit ClassicSkin(std::shared_ptr<RenderableFactory> provider)
    : _factory(std::move(provider))
    , _layout_alg(std::make_shared<LinearLayout>())
    , _item_effects(std::make_shared<CompositeEffect>())
    , _layout_effects(std::make_shared<CompositeEffect>())
    , _formatter(std::make_unique<DateTimeFormatter>("hh:mm a"))
  {}

  std::shared_ptr<ClockRenderable> process(const QDateTime& dt) override;

  bool supportsCustomSeparator() const noexcept override
  {
    return _supports_custom_separator;
  }

  bool supportsSeparatorAnimation() const noexcept override
  {
    return _supports_separator_animation;
  }

  void setSeparatorAnimationEnabled(bool enabled) override
  {
    _animate_separator = enabled;
    _separator_visible = _separator_visible || !enabled;
  }

  void animateSeparator() override
  {
    if (!_animate_separator) return;
    _separator_visible = !_separator_visible;
  }

  void setSupportsCustomSeparator(bool supports) noexcept
  {
    _supports_custom_separator = supports;
  }

  void setSupportsSeparatorAnimation(bool supports) noexcept
  {
    _supports_separator_animation = supports;
  }

  void setOrientation(Qt::Orientation orientation)
  {
    _layout_alg->setOrientation(orientation);
  }

  void setSpacing(qreal spacing)
  {
    _layout_alg->setSpacing(spacing);
  }

  void addItemEffect(std::shared_ptr<Effect> effect)
  {
    _item_effects->addEffect(std::move(effect));
  }

  void addLayoutEffect(std::shared_ptr<Effect> effect)
  {
    _layout_effects->addEffect(std::move(effect));
  }

  void clearItemEffects()
  {
    _item_effects->clearEffects();
  }

  void clearLayoutEffects()
  {
    _layout_effects->clearEffects();
  }

  const auto& formatter() const noexcept { return _formatter; }

private:
  bool isSeparator(QChar ch) const
  {
    return ch == ':';
  }

private:
  std::shared_ptr<RenderableFactory> _factory;
  std::shared_ptr<LinearLayout> _layout_alg;
  std::shared_ptr<CompositeEffect> _item_effects;
  std::shared_ptr<CompositeEffect> _layout_effects;
  std::unique_ptr<DateTimeFormatter> _formatter;
  // public properties
  bool _supports_custom_separator = false;
  bool _supports_separator_animation = false;
  // internal state
  bool _animate_separator = true;
  bool _separator_visible = true;
};
