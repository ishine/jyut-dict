#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "logic/database/sqldatabasemanager.h"

#include <QAction>
#include <QEvent>
#include <QLayout>
#include <QMainWindow>
#include <QSettings>
#include <QStackedWidget>
#include <QToolBar>
#include <QToolButton>

#include <memory>
#include <vector>

// The SettingsWindow allows users to modify settings. Surprise!

constexpr auto NUM_OF_TABS = 6;

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(std::shared_ptr<SQLDatabaseManager> manager,
                            QWidget *parent = nullptr);

    void changeEvent(QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUI();
    void translateUI();
    void setStyle(bool use_dark);

    void setButtonIcon(bool use_dark, int index);
    void openTab(int tabIndex);

    QWidget *_parent;

    QLayout *_layout;

    QStackedWidget *_contentStackedWidget;
    QToolBar *_toolBar;

    std::vector<QToolButton *> _toolButtons;
    std::vector<QAction *> _actions;

    std::shared_ptr<SQLDatabaseManager> _manager;
    std::shared_ptr<QSettings> _settings;

    bool _paletteRecentlyChanged = false;

signals:
    void triggerSearch(void);
    void updateStyle(void);

public slots:
    void paintWithApplicationState(Qt::ApplicationState state);

    void searchRequested(void);
    void updateStyleRequested(void);
};

#endif // SETTINGSWINDOW_H
