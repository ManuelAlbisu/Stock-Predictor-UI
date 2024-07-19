#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QLineEdit>
#include <QListWidget>
#include <QApplication>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QFile>
#include <QFileDialog>
#include <QLineSeries>
#include <QChartView>
#include <QPushButton>
#include <QEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    QMenu *view_menu_;

private slots:
    void open_file(void);
    void save_file(void);
    void console_open_file(const QString &file_path);
    void console_save_file(const QString &file_path);
    void console_toggle_series(const QString &series);
    void exit(void);
    void console_input(void);
    void execute_command(const QString &command);
    void toggle_series(bool checked);
    void toggle_dotted_line(bool checked);
    void make_prediction(int months);
    void draw_dotted_line(QDateTime date);
    void open_readme();

private:
    void create_actions(void);
    void create_menubar(void);
    void create_toolbar(void);
    void create_console(void);
    void console_display_file(void);
    void create_graph(void);
    void graph_line(QLineSeries *series, const QVector<double> &values);
    void create_buttons(void);
    void create_toggle_buttons(void);
    void create_dock(QWidget *widget,
                     const QString &name,
                     Qt::DockWidgetArea area);
    QIcon load_icon(const QString &icon_name);
    void change_theme(QChart::ChartTheme theme);
    void console_list_commands();
    void console_seek(const QString &date_str);
    void console_average(const QString &start_date_str, const QString &end_date_str);
    void clear_graph(void);
    void parse_csv(const QString &file_name);

    QLineSeries *open_series_;
    QLineSeries *high_series_;
    QLineSeries *low_series_;
    QLineSeries *close_series_;
    QLineSeries *volume_series_;
    QLineSeries *dotted_line_;

    QAction *save_action_;
    QAction *exit_action_;
    QAction *open_action_;
    QAction *open_series_action_;
    QAction *high_series_action_;
    QAction *low_series_action_;
    QAction *close_series_action_;
    QAction *volume_series_action_;
    QAction *dotted_line_action_;
    QAction *readme_action_;

    QAction *theme_light_action_;
    QAction *theme_dark_action_;
    QAction *theme_blue_cerulean_action_;
    QAction *theme_brown_sand_action_;
    QAction *theme_blue_ncs_action_;
    QAction *theme_high_contrast_action_;
    QAction *theme_blue_icy_action_;
    QAction *theme_qt_action_;

    QMenu *file_menu_;
    QMenu *edit_menu_;
    QMenu *help_menu_;

    QToolBar *tool_bar_;
    QChartView *chart_view_;

    QDateTimeAxis *x_axis_;
    QValueAxis *y_axis_;
    QValueAxis *volume_axis_;

    QPushButton *save_button_;
    QPushButton *open_button_;
    QPushButton *exit_button_;
    QPushButton *predict_button_;
    QLineEdit *months_input_;

    QToolButton *open_toggle_button_;
    QToolButton *high_toggle_button_;
    QToolButton *low_toggle_button_;
    QToolButton *close_toggle_button_;
    QToolButton *volume_toggle_button_;
    QToolButton *dotted_line_toggle_button_;

    bool open_series_visible_;
    bool high_series_visible_;
    bool low_series_visible_;
    bool close_series_visible_;
    bool volume_series_visible_;

    QLineEdit *input_;
    QListWidget *console_;

    QString current_file_path_;
    QVector<QDateTime> dates_;
    QDateTime source_last_entry_;
};

#endif // MAIN_WINDOW_H
