#pragma once

#include "settings/config_base_qvariant.hpp"

#include <QColor>
#include <QGradient>
#include <QFont>
#include <QPixmap>
#include <QPoint>
#include <QString>
#include <QTimeZone>

class WindowState final : public ConfigBaseQVariant {
  CONFIG_OPTION_Q(QPoint, Pos, QPoint(75, 50))
  CONFIG_OPTION_Q(Qt::Alignment, Alignment, Qt::AlignTop | Qt::AlignLeft)
  CONFIG_OPTION_Q(QFont, TextSkinFont, QFont("Segoe Script", 48))
  CONFIG_OPTION_Q(QString, LastUsedSkin, QString("Electronic (built-in)"))
  CONFIG_OPTION_Q(QColor, TextureColor, QColor(112, 96, 240))
  CONFIG_OPTION_Q(QGradient, TextureGradient, sample_conical_gradient())
  CONFIG_OPTION_Q(QPixmap, TexturePattern, sample_pattern())
  CONFIG_OPTION_Q(QColor, BackgroundColor, QColor(0, 0, 0, 160))
  CONFIG_OPTION_Q(QGradient, BackgroundGradient, sample_linear_gradient())
  CONFIG_OPTION_Q(QPixmap, BackgroundPattern, sample_pattern())
  CONFIG_OPTION_Q(QTimeZone, TimeZone, QTimeZone::systemTimeZone())
public:
  using ConfigBaseQVariant::ConfigBaseQVariant;
private:
  static QGradient sample_conical_gradient();
  static QGradient sample_linear_gradient();
  static QPixmap sample_pattern();
};
