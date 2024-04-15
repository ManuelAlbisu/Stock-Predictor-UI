#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QLineEdit>
#include <QListWidget>
#include <QApplication>
#include <QValueAxis>
#include <QFile>
#include <QFileDialog>
#include <QLineSeries>
#include <QChartView>
#include <QPushButton>

struct csv_data {
    QVector<int> dates;
    QVector<double> prices;
};
typedef struct csv_data csv_data;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    QMenu *view_menu_;

private slots:
    void open_file();
    void exit();
    void console_input();
    void execute_command(const QString &command);
    void toggle_xgboost();

private:
    void create_actions();
    void create_menubar();
    void create_toolbar();
    void create_console();
    void create_graph();
    void graph_line(const QVector<int> &dates, const QVector<double> &prices);
    void create_buttons();
    void create_dock(QWidget *widget, const QString &name, Qt::DockWidgetArea area);
    void read_file(const QString &file_name);

    QAction *xgboost_action_;
    QAction *save_action_;
    QAction *exit_action_;
    QAction *open_action_;

    QMenu *file_menu_;
    QMenu *edit_menu_;
    QMenu *help_menu_;

    QToolBar *tool_bar_;
    QChartView *chart_view_;
    QValueAxis *x_axis_;
    QValueAxis *y_axis_;

    QPushButton *xgboost_button_;
    QPushButton *save_button_;
    QPushButton *open_button_;
    QPushButton *exit_button_;

    QLineEdit *input_;
    QListWidget *console_;
};

#endif // MAIN_WINDOW_H
