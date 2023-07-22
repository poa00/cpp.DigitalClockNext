#pragma once

#include <QApplication>

#include <memory>

class ApplicationPrivate;

class Application final : public QApplication
{
  Q_OBJECT

public:
  // follows QApplication constructor
  Application(int& argc, char** argv);
  ~Application();

  void init();

private slots:
  // TODO: add optional 'screen' parameter
  // not set (aka from tray menu) - show on default screen
  void showSettingsDialog();
  void showAboutDialog();

private:
  void initConfig();
  void initTray();
  void initCore();

  void createWindows();

private:
  std::unique_ptr<ApplicationPrivate> _impl;
};