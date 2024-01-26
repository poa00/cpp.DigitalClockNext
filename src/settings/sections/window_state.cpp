/*
 * SPDX-FileCopyrightText: 2023 Nick Korotysh <nick.korotysh@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "window_state.hpp"

#include <QPainter>

QFont WindowState::default_font()
{
  using namespace Qt::Literals::StringLiterals;
  QFont fnt(u"Comic Sans MS"_s, 96);
#if defined(Q_OS_MACOS)
  fnt.setFamily(u"SignPainter"_s);
#elif defined(Q_OS_WINDOWS)
  fnt.setFamily(u"MV Boli"_s);
#endif
  return fnt;
}

QGradient WindowState::sample_conical_gradient()
{
  QConicalGradient g(0.5, 0.5, 45.0);
  g.setStops({
    {0.00, {170,   0,   0}},  // #aa0000
    {0.20, {  0,  85, 255}},  // #0055ff
    {0.45, {  0, 170,   0}},  // #00aa00
    {0.65, {255, 255,   0}},  // #ffff00
    {1.00, {170,   0,   0}},  // #aa0000
  });
  g.setCoordinateMode(QGradient::ObjectMode);
  return g;
}

QGradient WindowState::sample_linear_gradient()
{
  QLinearGradient g(0., 0., 0., 1.);
  g.setColorAt(0.0, Qt::transparent);
  g.setColorAt(0.6, Qt::transparent);
  g.setColorAt(1.0, QColor(85, 0, 255));
  g.setCoordinateMode(QGradient::ObjectMode);
  return g;
}

QPixmap WindowState::sample_pattern()
{
  QPixmap pxm(8, 8);
  pxm.fill(QColor(160, 0, 160));
  QPainter p(&pxm);
  p.fillRect(0, 0, 4, 4, QColor(224, 0, 224));
  p.fillRect(4, 4, 8, 8, QColor(224, 0, 224));
  return pxm;
}
