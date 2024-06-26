/*
 * SPDX-FileCopyrightText: 2024 Nick Korotysh <nick.korotysh@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "time_format_settings.hpp"
#include "ui_time_format_settings.h"

#include <QtCore/QSet>
#include <QtGui/QDesktopServices>

#include "app_config.hpp"
#include "classic_skin.hpp"

namespace {

using namespace Qt::Literals::StringLiterals;

const QSet<QString> standard_formats = {
  u"h:mm"_s, u"H:mm"_s, u"hh:mm"_s, u"HH:mm"_s,
  u"h:mm:ss"_s, u"H:mm:ss"_s, u"hh:mm:ss"_s, u"HH:mm:ss"_s,
  u"h:mmA"_s, u"h:mma"_s, u"h:mm A"_s, u"h:mm a"_s,
  u"hh:mmA"_s, u"hh:mma"_s, u"hh:mm A"_s, u"hh:mm a"_s,
  u"hh:mm:ssA"_s, u"hh:mm:ssa"_s, u"hh:mm:ss A"_s, u"hh:mm:ss a"_s,
};

const QString standard_date_format = u"\\ndd MM yyyy"_s;

} // namespace

struct TimeFormatSettings::Impl {
  WindowConfig* wcfg;
  ClassicSkinConfig* scfg;

  ClassicSkin* skin;

  Impl(ClassicSkin* skin, WindowConfig* wcfg) noexcept
    : wcfg(wcfg)
    , scfg(&wcfg->classicSkin())
    , skin(skin)
  {}
};

TimeFormatSettings::TimeFormatSettings(ClassicSkin* skin, WindowConfig* wcfg, QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::TimeFormatSettings)
  , impl(std::make_unique<Impl>(skin, wcfg))
{
  ui->setupUi(this);

  QString time_format = impl->scfg->getTimeFormat();
  time_format.remove(standard_date_format);

  ui->smaller_seconds->setChecked(impl->scfg->getSecondsScaleFactor() != 100);
  if (ui->smaller_seconds->isChecked())
    ui->seconds_scale_factor_edit->setValue(impl->scfg->getSecondsScaleFactor());

  ui->use_custom_format->setChecked(!standard_formats.contains(time_format));
  ui->format_edit->setText(time_format);

  if (!ui->use_custom_format->isChecked()) {
    ui->rb_12h->setChecked(time_format.contains('h'));
    ui->rb_24h->setChecked(time_format.contains('H'));
    ui->leading_zero->setChecked(time_format.contains("hh", Qt::CaseInsensitive));
    ui->show_seconds->setChecked(time_format.contains(":ss"));

    ui->show_apm->setChecked(time_format.contains('a', Qt::CaseInsensitive));
    ui->add_space_before_apm->setChecked(time_format.contains(' '));
    ui->rb_ucase_apm->setChecked(time_format.contains('A'));
    ui->rb_lcase_apm->setChecked(time_format.contains('a'));

    ui->show_date->setChecked(impl->scfg->getTimeFormat().contains(standard_date_format));

    updateTimeFormat();
  }
  // slot is not called if initial control state matches new state,
  // but the same logic should be triggered on initialization (regardless of the state)
  on_use_custom_format_toggled(ui->use_custom_format->isChecked());

  ui->custom_seps_label->setEnabled(impl->skin->supportsCustomSeparator());
  ui->custom_seps_edit->setEnabled(impl->skin->supportsCustomSeparator());
  ui->custom_seps_edit->setText(impl->scfg->getCustomSeparators());

  ui->layout_cfg_edit->setText(impl->scfg->getLayoutConfig());

  connect(ui->rb_12h, &QRadioButton::clicked, this, &TimeFormatSettings::updateTimeFormat);
  connect(ui->rb_24h, &QRadioButton::clicked, this, &TimeFormatSettings::updateTimeFormat);
  connect(ui->leading_zero, &QCheckBox::clicked, this, &TimeFormatSettings::updateTimeFormat);
  connect(ui->show_seconds, &QCheckBox::clicked, this, &TimeFormatSettings::updateTimeFormat);

  connect(ui->show_apm, &QCheckBox::clicked, this, &TimeFormatSettings::updateTimeFormat);
  connect(ui->add_space_before_apm, &QCheckBox::clicked, this, &TimeFormatSettings::updateTimeFormat);
  connect(ui->rb_ucase_apm, &QRadioButton::clicked, this, &TimeFormatSettings::updateTimeFormat);
  connect(ui->rb_lcase_apm, &QRadioButton::clicked, this, &TimeFormatSettings::updateTimeFormat);
}

TimeFormatSettings::~TimeFormatSettings()
{
  delete ui;
}

void TimeFormatSettings::on_show_date_clicked(bool checked)
{
  if (checked) ui->layout_cfg_edit->setText("01");
  updateTimeFormat();
}

void TimeFormatSettings::on_smaller_seconds_clicked(bool checked)
{
  auto v = checked ? ui->seconds_scale_factor_edit->value() : 100;
  on_seconds_scale_factor_edit_valueChanged(v);
}

void TimeFormatSettings::on_seconds_scale_factor_edit_valueChanged(int arg1)
{
  auto ssf = arg1 / 100.;
  impl->skin->setTokenTransform("ss", QTransform::fromScale(ssf, ssf));
  impl->scfg->setSecondsScaleFactor(arg1);
}

void TimeFormatSettings::on_use_custom_format_toggled(bool checked)
{
  ui->format_group->setDisabled(checked);
  ui->show_date->setDisabled(checked);

  if (checked)
    ui->am_pm_group->setDisabled(true);
  else
    ui->am_pm_group->setEnabled(ui->rb_12h->isChecked());
}

void TimeFormatSettings::on_use_custom_format_clicked(bool checked)
{
  if (!checked) updateTimeFormat();
}

void TimeFormatSettings::on_format_edit_textChanged(const QString& arg1)
{
  bool is_multiline = arg1.contains("\\n");
  ui->layout_cfg_label->setEnabled(is_multiline);
  ui->layout_cfg_edit->setEnabled(is_multiline);
  ui->layout_cfg_help_btn->setEnabled(is_multiline);
}

void TimeFormatSettings::on_format_edit_textEdited(const QString& arg1)
{
  ui->format_apply_btn->setEnabled(!arg1.isEmpty());
}

void TimeFormatSettings::on_format_help_btn_clicked()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Kolcha/DigitalClockNext/wiki/Supported-date-time-formats"));
}

void TimeFormatSettings::on_format_apply_btn_clicked()
{
  auto time_format = ui->format_edit->text();
  impl->skin->setFormat(time_format);
  impl->scfg->setTimeFormat(time_format);
}

void TimeFormatSettings::on_custom_seps_edit_textEdited(const QString& arg1)
{
  impl->skin->setCustomSeparators(arg1);
  impl->scfg->setCustomSeparators(arg1);
}

void TimeFormatSettings::on_layout_cfg_edit_textEdited(const QString& arg1)
{
  impl->skin->setLayoutConfig(arg1);
  impl->scfg->setLayoutConfig(arg1);
}

void TimeFormatSettings::on_layout_cfg_help_btn_clicked()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Kolcha/DigitalClockNext/wiki/Multiple-lines"));
}

void TimeFormatSettings::updateTimeFormat()
{
  QChar hc = ui->rb_24h->isChecked() ? 'H' : 'h';
  qsizetype hs = ui->leading_zero->isChecked() ? 2 : 1;

  QString time_format(hs, hc);
  time_format += QLatin1String(":mm");

  if (ui->show_seconds->isChecked())
    time_format += QLatin1String(":ss");

  if (ui->rb_12h->isChecked() && ui->show_apm->isChecked()) {
    if (ui->add_space_before_apm->isChecked())
      time_format += ' ';

    time_format += ui->rb_ucase_apm->isChecked() ? 'A' : 'a';
  }

  if (ui->show_date->isChecked())
    time_format += standard_date_format;

  ui->format_edit->setText(time_format);
  on_format_apply_btn_clicked();
}
