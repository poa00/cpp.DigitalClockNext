#pragma once

#include <QDialog>

#include <memory>

class ApplicationPrivate;
class ClockSkin;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(ApplicationPrivate* app,
                          std::size_t idx,
                          QWidget* parent = nullptr);
  ~SettingsDialog();

public slots:
  void accept() override;
  void reject() override;

private slots:
  void on_font_rbtn_clicked();
  void on_skin_rbtn_clicked();

  void on_select_font_btn_clicked();
  void on_skin_cbox_activated(int index);

  void on_is_separator_flashes_clicked(bool checked);

  void on_scaling_x_edit_valueChanged(int arg1);
  void on_scaling_y_edit_valueChanged(int arg1);
  void on_scaling_same_btn_clicked(bool checked);

  void on_use_time_zone_clicked(bool checked);
  void on_time_zone_edit_activated(int index);

  void onAlignmentButtonClicked(int id);

  void on_is_stay_on_top_clicked(bool checked);
  void on_is_transp_for_input_clicked(bool checked);

private:
  void applySkin(std::shared_ptr<ClockSkin> skin);
  void applyFlashingSeparator(bool enable);
  void applyTimeZoneSettings();

  void updateSkinSettingsTab();

private:
  Ui::SettingsDialog* ui;
  struct Impl;
  std::unique_ptr<Impl> impl;
};
