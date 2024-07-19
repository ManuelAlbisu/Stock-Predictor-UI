#include "main_window.h"
#include <QDockWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QIcon>
#include <QVBoxLayout>
#include <QEasingCurve>
#include <QStatusBar>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QToolButton>

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    open_series_visible_(true),
    high_series_visible_(true),
    low_series_visible_(true),
    close_series_visible_(true),
    volume_series_visible_(true),
    open_series_(nullptr),
    high_series_(nullptr),
    low_series_(nullptr),
    close_series_(nullptr),
    volume_series_(nullptr),
    dotted_line_(nullptr)
{
    resize(1280, 768);
    create_actions();
    create_menubar();
    create_buttons();
    create_toggle_buttons();
    create_toolbar();
    create_console();
    create_graph();
}

MainWindow::~MainWindow() {}

// Create actions
void MainWindow::create_actions(void)
{
    save_action_ = new QAction("Save");
    save_action_->setStatusTip("Save file as...");
    save_action_->setIcon(load_icon("save.svg"));
    connect(save_action_, &QAction::triggered, this, &MainWindow::save_file);

    exit_action_ = new QAction("Exit");
    exit_action_->setStatusTip("Exit program");
    exit_action_->setIcon(load_icon("exit.svg"));
    connect(exit_action_, &QAction::triggered, this, &MainWindow::exit);

    open_action_ = new QAction("Open");
    open_action_->setStatusTip("Open CSV file");
    open_action_->setIcon(load_icon("open.svg"));
    connect(open_action_, &QAction::triggered, this, &MainWindow::open_file);

    open_series_action_ = new QAction("Show Open Series");
    open_series_action_->setCheckable(true);
    open_series_action_->setChecked(true);
    open_series_action_->setIcon(load_icon("open_series.svg"));
    connect(open_series_action_, &QAction::toggled, this, &MainWindow::toggle_series);

    high_series_action_ = new QAction("Show High Series");
    high_series_action_->setCheckable(true);
    high_series_action_->setChecked(true);
    high_series_action_->setIcon(load_icon("high_series.svg"));
    connect(high_series_action_, &QAction::toggled, this, &MainWindow::toggle_series);

    low_series_action_ = new QAction("Show Low Series");
    low_series_action_->setCheckable(true);
    low_series_action_->setChecked(true);
    low_series_action_->setIcon(load_icon("low_series.svg"));
    connect(low_series_action_, &QAction::toggled, this, &MainWindow::toggle_series);

    close_series_action_ = new QAction("Show Close Series");
    close_series_action_->setCheckable(true);
    close_series_action_->setChecked(true);
    close_series_action_->setIcon(load_icon("close_series.svg"));
    connect(close_series_action_, &QAction::toggled, this, &MainWindow::toggle_series);

    volume_series_action_ = new QAction("Show Volume Series");
    volume_series_action_->setCheckable(true);
    volume_series_action_->setChecked(true);
    volume_series_action_->setIcon(load_icon("volume_series.svg"));
    connect(volume_series_action_, &QAction::toggled, this, &MainWindow::toggle_series);

    dotted_line_action_ = new QAction("Show Prediction Line");
    dotted_line_action_->setCheckable(true);
    dotted_line_action_->setChecked(true);
    dotted_line_action_->setIcon(load_icon("prediction_line.svg"));
    connect(dotted_line_action_, &QAction::toggled, this, &MainWindow::toggle_dotted_line);

    theme_light_action_ = new QAction("Light");
    theme_dark_action_ = new QAction("Dark");
    theme_blue_cerulean_action_ = new QAction("Blue Cerulean");
    theme_brown_sand_action_ = new QAction("Brown Sand");
    theme_blue_ncs_action_ = new QAction("Blue NCS");
    theme_high_contrast_action_ = new QAction("High Contrast");
    theme_blue_icy_action_ = new QAction("Blue Icy");
    theme_qt_action_ = new QAction("Qt");

    connect(theme_light_action_, &QAction::triggered, this, [this]() {
        change_theme(QChart::ChartThemeLight);
    });
    connect(theme_dark_action_, &QAction::triggered, this, [this]() {
        change_theme(QChart::ChartThemeDark);
    });
    connect(theme_blue_cerulean_action_, &QAction::triggered, this, [this]() {
        change_theme(QChart::ChartThemeBlueCerulean);
    });
    connect(theme_brown_sand_action_, &QAction::triggered, this, [this]() {
        change_theme(QChart::ChartThemeBrownSand);
    });
    connect(theme_blue_ncs_action_, &QAction::triggered, this, [this]() {
        change_theme(QChart::ChartThemeBlueNcs);
    });
    connect(theme_high_contrast_action_, &QAction::triggered, this, [this]() {
        change_theme(QChart::ChartThemeHighContrast);
    });
    connect(theme_blue_icy_action_, &QAction::triggered, this, [this]() {
        change_theme(QChart::ChartThemeBlueIcy);
    });
    connect(theme_qt_action_, &QAction::triggered, this, [this]() {
        change_theme(QChart::ChartThemeQt);
    });
}

// Create menubar
void MainWindow::create_menubar(void)
{
    file_menu_ = menuBar()->addMenu("&File");
    file_menu_->addAction(save_action_);
    file_menu_->addAction(open_action_);
    file_menu_->addSeparator();
    file_menu_->addAction(exit_action_);

    edit_menu_ = menuBar()->addMenu("&Edit");
    QMenu *themes_menu = edit_menu_->addMenu("Themes");
    themes_menu->addAction(theme_light_action_);
    themes_menu->addAction(theme_dark_action_);
    themes_menu->addAction(theme_blue_cerulean_action_);
    themes_menu->addAction(theme_brown_sand_action_);
    themes_menu->addAction(theme_blue_ncs_action_);
    themes_menu->addAction(theme_high_contrast_action_);
    themes_menu->addAction(theme_blue_icy_action_);
    themes_menu->addAction(theme_qt_action_);

    view_menu_ = menuBar()->addMenu("&View");
    view_menu_->addAction(open_series_action_);
    view_menu_->addAction(high_series_action_);
    view_menu_->addAction(low_series_action_);
    view_menu_->addAction(close_series_action_);
    view_menu_->addAction(volume_series_action_);
    view_menu_->addAction(dotted_line_action_);

    help_menu_ = menuBar()->addMenu("&Help");
}

// Create toolbar
void MainWindow::create_toolbar(void)
{
    tool_bar_ = new QToolBar("Tools");
    addToolBar(Qt::LeftToolBarArea, tool_bar_);

    tool_bar_->addAction(save_action_);
    tool_bar_->addAction(open_action_);
    tool_bar_->addAction(exit_action_);

    tool_bar_->addSeparator();

    tool_bar_->addWidget(open_toggle_button_);
    tool_bar_->addWidget(high_toggle_button_);
    tool_bar_->addWidget(low_toggle_button_);
    tool_bar_->addWidget(close_toggle_button_);
    tool_bar_->addWidget(volume_toggle_button_);
    tool_bar_->addWidget(dotted_line_toggle_button_);
}

// Create console
void MainWindow::create_console(void)
{
    console_ = new QListWidget();
    input_ = new QLineEdit();

    console_->setStyleSheet("background-color: #e0e0e0; color: black;");
    input_->setStyleSheet("background-color: #d0d0d0; color: black;");
    input_->setPlaceholderText("Type 'list' for available commands.");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(console_);
    layout->addWidget(input_);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    create_dock(widget, "Console", Qt::RightDockWidgetArea);

    connect(input_, SIGNAL(returnPressed()), this, SLOT(console_input()));
}

// Handle console input
void MainWindow::console_input(void)
{
    QString command = input_->text();
    console_->addItem(command);
    execute_command(command);
    input_->clear();
}

// Execute console command
void MainWindow::execute_command(const QString &command)
{
    static QRegularExpression re("\\s+");
    QStringList list = command.split(re);

    if (command.toLower() == "clear") {
        console_->clear();
    } else if (command.toLower() == "quit" || command.toLower() == "exit" || command.toLower() == "close") {
        exit();
    } else if (command.toLower() == "current file") {
        console_display_file();
    } else if (command.toLower() == "list") {
        console_list_commands();
    } else if (list.size() > 1 && list[0].toLower() == "save") {
        QString file_path = list.mid(1).join(" ");
        console_save_file(file_path);
    } else if (list.size() > 1 && (list[0].toLower() == "open" || list[0].toLower() == "read")) {
        QString file_name = list.mid(1).join(" ");
        console_open_file(file_name);
    } else if (list.size() == 2 && list[0].toLower() == "toggle") {
        QString series = list[1].toLower();
        console_toggle_series(series);
    } else if (list.size() == 2 && list[0].toLower() == "seek") {
        QString date = list[1];
        console_seek(date);
    } else if (list.size() == 3 && list[0].toLower() == "make" && list[1].toLower() == "prediction") {
        int months = list[2].toInt();
        if (months > 0 && months <= 12)
            make_prediction(months);
        else
            console_->addItem("Invalid number of months.\nPlease enter a number between 1 and 12.");
    } else if (list.size() == 3 && list[0].toLower() == "average") {
        QString start_date = list[1];
        QString end_date = list[2];
        console_average(start_date, end_date);
    } else {
        statusBar()->showMessage(("Command '" + command + "' is unknown."), 5000);
    }
}

// Create graph
void MainWindow::create_graph(void)
{
    // Create the graph and graph view
    QChart *chart = new QChart();
    chart->legend()->setVisible(true);
    chart_view_ = new QChartView(chart);
    chart_view_->setRenderHint(QPainter::Antialiasing);

    // Decorate the graph
    chart->setTitle("");
    chart->setTheme(QChart::ChartThemeDark);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setAnimationDuration(3000);
    chart->setAnimationEasingCurve(QEasingCurve::Linear);

    // Create time axis
    x_axis_ = new QDateTimeAxis();
    x_axis_->setTitleText("Date");
    x_axis_->setFormat("yyyy-MM-dd");
    chart->addAxis(x_axis_, Qt::AlignBottom);

    // Create value axis
    y_axis_ = new QValueAxis();
    y_axis_->setTitleText("Price (USD)");
    y_axis_->setGridLineVisible(true);
    y_axis_->setTickCount(15);
    chart->addAxis(y_axis_, Qt::AlignLeft);

    setCentralWidget(chart_view_);
}

// Add a line series to the graph
void MainWindow::graph_line(QLineSeries *series, const QVector<double> &values)
{
    if (dates_.size() != values.size()) {
        qDebug() << "Error: Date and value vectors have differing sizes.";
        return;
    }

    for (int i = 0; i < dates_.size(); ++i)
        series->append(dates_[i].toMSecsSinceEpoch(), values[i]);

    QChart *chart = chart_view_->chart();
    chart->addSeries(series);
    series->attachAxis(x_axis_);
    series->attachAxis(y_axis_);

    if (!dates_.isEmpty()) {
        QDateTime min_date = dates_.first();
        QDateTime max_date = dates_.last();
        double min_value = *std::min_element(values.begin(), values.end());
        double max_value = *std::max_element(values.begin(), values.end());

        x_axis_->setRange(min_date, max_date);
        y_axis_->setRange(min_value, max_value);
    }
}

// Create buttons
void MainWindow::create_buttons(void)
{
    save_button_ = new QPushButton("Save");
    save_button_->setGeometry(QRect(QPoint(100, 500), QSize(200, 100)));
    save_button_->setStyleSheet("background-color: gray; color: white;");
    connect(save_button_, SIGNAL(clicked(bool)), this, SLOT(save_file()));

    open_button_ = new QPushButton("Open");
    open_button_->setGeometry(QRect(QPoint(100, 500), QSize(200, 100)));
    open_button_->setStyleSheet("background-color: gray; color: white;");
    connect(open_button_, SIGNAL(clicked(bool)), this, SLOT(open_file()));

    exit_button_ = new QPushButton("Exit");
    exit_button_->setGeometry(QRect(QPoint(100, 500), QSize(200, 100)));
    exit_button_->setStyleSheet("background-color: red; color: white;");
    connect(exit_button_, SIGNAL(clicked(bool)), this, SLOT(exit()));

    predict_button_ = new QPushButton("Make Prediction");
    predict_button_->setGeometry(QRect(QPoint(100, 500), QSize(200, 100)));
    predict_button_->setStyleSheet("background-color: blue; color: white;");
    connect(predict_button_, &QPushButton::clicked, this, [this]() {
        int months = months_input_->text().toInt();
        make_prediction(months);
    });

    months_input_ = new QLineEdit();
    months_input_->setPlaceholderText(" Number of months");
    months_input_->setValidator(new QIntValidator(1, 12, this)); // limit: 12 months
    months_input_->setFixedWidth(120);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(exit_button_);
    layout->addWidget(open_button_);
    layout->addWidget(save_button_);
    layout->addWidget(predict_button_);
    layout->addWidget(months_input_);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    create_dock(widget, "Controls", Qt::BottomDockWidgetArea);
}

// Create toggle buttons for series visibility
void MainWindow::create_toggle_buttons(void)
{
    // Open series toggle button
    open_toggle_button_ = new QToolButton();
    open_toggle_button_->setIcon(load_icon("open_series.svg"));
    open_toggle_button_->setText("Open Series");
    open_toggle_button_->setToolTip("Toggle Open Series");
    open_toggle_button_->setCheckable(true);
    open_toggle_button_->setChecked(open_series_visible_);
    connect(open_toggle_button_, &QToolButton::toggled, this, [this](bool checked) {
        open_series_visible_ = checked;
        if (open_series_)
            open_series_->setVisible(checked);
    });

    // High series toggle button
    high_toggle_button_ = new QToolButton();
    high_toggle_button_->setIcon(load_icon("high_series.svg"));
    high_toggle_button_->setText("High Series");
    high_toggle_button_->setToolTip("Toggle high Series");
    high_toggle_button_->setCheckable(true);
    high_toggle_button_->setChecked(high_series_visible_);
    connect(high_toggle_button_, &QToolButton::toggled, this, [this](bool checked) {
        high_series_visible_ = checked;
        if (high_series_)
            high_series_->setVisible(checked);
    });

    // Low series toggle button
    low_toggle_button_ = new QToolButton();
    low_toggle_button_->setIcon(load_icon("low_series.svg"));
    low_toggle_button_->setText("Low Series");
    low_toggle_button_->setToolTip("Toggle Low Series");
    low_toggle_button_->setCheckable(true);
    low_toggle_button_->setChecked(low_series_visible_);
    connect(low_toggle_button_, &QToolButton::toggled, this, [this](bool checked) {
        low_series_visible_ = checked;
        if (low_series_)
            low_series_->setVisible(checked);
    });

    // Close series toggle button
    close_toggle_button_ = new QToolButton();
    close_toggle_button_->setIcon(load_icon("close_series.svg"));
    close_toggle_button_->setText("Toggle Close Series");
    close_toggle_button_->setCheckable(true);
    close_toggle_button_->setChecked(close_series_visible_);
    connect(close_toggle_button_, &QToolButton::toggled, this, [this](bool checked) {
        close_series_visible_ = checked;
        if (close_series_)
            close_series_->setVisible(checked);
    });

    // Volume series toggle button
    volume_toggle_button_ = new QToolButton();
    volume_toggle_button_->setIcon(load_icon("volume_series.svg"));
    volume_toggle_button_->setText("Volume Series");
    volume_toggle_button_->setText("Toggle Volume Series");
    volume_toggle_button_->setCheckable(true);
    volume_toggle_button_->setChecked(volume_series_visible_);
    connect(volume_toggle_button_, &QToolButton::toggled, this, [this](bool checked) {
        volume_series_visible_ = checked;
        if (volume_series_)
            volume_series_->setVisible(checked);
    });

    // Prediction line toggle button
    dotted_line_toggle_button_ = new QToolButton();
    dotted_line_toggle_button_->setIcon(load_icon("prediction_line.svg"));
    dotted_line_toggle_button_->setText("Prediction Line");
    dotted_line_toggle_button_->setToolTip("Toggle Prediction Line");
    dotted_line_toggle_button_->setCheckable(true);
    dotted_line_toggle_button_->setChecked(true);
    connect(dotted_line_toggle_button_, &QToolButton::toggled, this, &MainWindow::toggle_dotted_line);
}

// Create dock widget
void MainWindow::create_dock(QWidget *widget, const QString &name, Qt::DockWidgetArea area)
{
    QDockWidget *dock = new QDockWidget(name, this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(widget);
    addDockWidget(area, dock);
    view_menu_->addAction(dock->toggleViewAction());
}

// Toggle series visibility
void MainWindow::toggle_series(bool checked)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action == open_series_action_) {
        open_series_visible_ = checked;
        if (open_series_)
            open_series_->setVisible(checked);
    } else if (action == high_series_action_) {
        high_series_visible_ = checked;
        if (high_series_)
            high_series_->setVisible(checked);
    } else if (action == low_series_action_) {
        low_series_visible_ = checked;
        if (low_series_)
            low_series_->setVisible(checked);
    } else if (action == close_series_action_) {
        close_series_visible_ = checked;
        if (close_series_)
            close_series_->setVisible(checked);
    } else if (action == volume_series_action_) {
        volume_series_visible_ = checked;
        if (volume_series_)
            volume_series_->setVisible(checked);
    }
}

// Toggle dotted line visibility
void MainWindow::toggle_dotted_line(bool checked)
{
    if (dotted_line_)
        dotted_line_->setVisible(checked);
}

// Open file and parse CSV data
void MainWindow::open_file(void)
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open file", "", "CSV Files (*.csv)");
    if (!file_name.isEmpty()) {
        current_file_path_ = file_name;
        clear_graph();
        dates_.clear();
        parse_csv(file_name);

        QFileInfo file_info(file_name);
        QString base_name = file_info.baseName();
        chart_view_->chart()->setTitle(base_name);

        if (!dates_.isEmpty())
            source_last_entry_ = dates_.last();
    } else {
        qDebug() << "No file given.\n";
        return;
    }
}

// Save prediction file
void MainWindow::save_file(void)
{
    QString prediction_file = "/tmp/predictions.csv";
    if (!QFile::exists(prediction_file)) {
        QMessageBox::warning(this, "Warning", "No predictions have been made.");
        console_->addItem("No predictions have been made.");
        console_->addItem("");
        return;
    }

    QString dest_path = QFileDialog::getSaveFileName(this, "Save Prediction File", "", "CSV Files (*.csv)");
    if (dest_path.isEmpty())
        return;

    if (!dest_path.endsWith(".csv", Qt::CaseInsensitive))
        dest_path += ".csv";

    if (!QFile::copy(prediction_file, dest_path)) {
        QMessageBox::warning(this, "warning", "Failed to save file.");
        console_->addItem("Failed to save file.");
        console_->addItem("");
    }
}

// Console command to open file
void MainWindow::console_open_file(const QString &file_path)
{
    if (file_path.isEmpty()) {
        console_->addItem("No file name given.");
        console_->addItem("");
        return;
    }

    if (!QFile::exists(file_path)) {
        console_->addItem("File does not exist: " + file_path);
        console_->addItem("");
        return;
    }

    current_file_path_ = file_path;
    clear_graph();
    dates_.clear();
    parse_csv(file_path);

    QFileInfo file_info(file_path);
    QString base_name = file_info.baseName();
    chart_view_->chart()->setTitle(base_name);

    if (!dates_.isEmpty())
        source_last_entry_ = dates_.last();

    console_->addItem("File opened: " + file_path);
    console_->addItem("");
}

// Console command to save file
void MainWindow::console_save_file(const QString &file_path)
{
    QString prediction_file = "/tmp/predictions.csv";
    if (!QFile::exists(prediction_file)) {
        console_->addItem("No predictions have been made.");
        console_->addItem("");
        return;
    }

    if (file_path.isEmpty()) {
        console_->addItem("No file path provided.");
        console_->addItem("");
        return;
    }

    QString dest_path = file_path;
    if (!dest_path.endsWith(".csv", Qt::CaseInsensitive))
        dest_path += ".csv";

    if (!QFile::copy(prediction_file, dest_path)) {
        console_->addItem("Failed to save file.");
        console_->addItem("");
    } else {
        console_->addItem("File saved: " + dest_path);
        console_->addItem("");
    }
}

// Display current file in console
void MainWindow::console_display_file(void)
{
    if (current_file_path_.isEmpty()) {
        console_->addItem("No file is currently loaded.");
        console_->addItem("");
    } else {
        console_->addItem("Current file: " + current_file_path_);
        console_->addItem("");
    }
}

// Toggle series visibility via console command
void MainWindow::console_toggle_series(const QString &series)
{
    if (series == "open") {
        open_series_visible_ = !open_series_visible_;
        if (open_series_)
            open_series_->setVisible(open_series_visible_);
        console_->addItem("Toggled open series.");
        console_->addItem("");
    } else if (series == "high") {
        high_series_visible_ = !high_series_visible_;
        if (high_series_)
            high_series_->setVisible(high_series_visible_);
        console_->addItem("Toggled high series.");
        console_->addItem("");
    } else if (series == "low") {
        low_series_visible_ = !low_series_visible_;
        if (low_series_)
            low_series_->setVisible(low_series_visible_);
        console_->addItem("Toggled low series.");
        console_->addItem("");
    } else if (series == "close") {
        close_series_visible_ = !close_series_visible_;
        if (close_series_)
            close_series_->setVisible(close_series_visible_);
        console_->addItem("Toggled close series.");
        console_->addItem("");
    } else if (series == "volume") {
        volume_series_visible_ = !volume_series_visible_;
        if (volume_series_)
            volume_series_->setVisible(volume_series_visible_);
        console_->addItem("Toggled volume series.");
        console_->addItem("");
    } else if (series == "line") {
        if (dotted_line_) {
            dotted_line_->setVisible(!dotted_line_->isVisible());
            console_->addItem("Toggled prediction line.");
            console_->addItem("");
        } else {
            console_->addItem("Prediction line is not available.");
            console_->addItem("");
        }
    } else {
        console_->addItem("Unknown series: " + series);
        console_->addItem("");
    }
}

// List available console commands
void MainWindow::console_list_commands(void)
{
    console_->addItem("Available commands:");
    console_->addItem("- list - List all available commands");
    console_->addItem("- clear - Clear the console");
    console_->addItem("- quit | exit | close - Exit the program");
    console_->addItem("- current file - Display the current file path");
    console_->addItem("- save <file_path> - Save the current predictions to the specified file path");
    console_->addItem("- (open | read) <file_path> - Open a CSV file from the specified path");
    console_->addItem("- make prediction <months> - Make a prediction for the specified number of months");
    console_->addItem("- toggle <series> - Toggle the visibility of the specified series (open, high, low, close, volume, line)");
    console_->addItem("- seek <date> - Seek and display the values for the specified date (format: yyyy-MM-dd)");
    console_->addItem("- average <start_date> <end_date> - Calculate the average values for the series in the specified date range (format: yyyy-MM-dd)"
                      "or use (first | start) and (last | end) for the entire range");
    console_->addItem("");
}

// Seek and display values for a specific date
void MainWindow::console_seek(const QString &date_str)
{
    QDateTime date = QDateTime::fromString(date_str, "yyyy-MM-dd");
    if (!date.isValid()) {
        console_->addItem("Invalid date format. Please use yyyy-MM-dd");
        console_->addItem("");
        return;
    }

    int index = -1;
    for (int i = 0; i < dates_.size(); ++i) {
        if (dates_[i].date() == date.date()) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        console_->addItem("Date not found.");
        console_->addItem("");
        return;
    }

    double open = open_series_->at(index).y();
    double high = high_series_->at(index).y();
    double low = low_series_->at(index).y();
    double close = close_series_->at(index).y();
    double volume = volume_series_->at(index).y();

    console_->addItem(QString("Values for %1:").arg(date_str));
    console_->addItem(QString("- Open: %1").arg(open));
    console_->addItem(QString("- High: %1").arg(high));
    console_->addItem(QString("- Low: %1").arg(low));
    console_->addItem(QString("- Close: %1").arg(close));
    console_->addItem(QString("- Volume: %1").arg(volume));
    console_->addItem("");
}

// Calculate and display average values for a date range
void MainWindow::console_average(const QString &start_date_str, const QString &end_date_str)
{
    QDateTime start_date, end_date;
    if (start_date_str.toLower() == "first" || start_date_str.toLower() == "start") {
        start_date = dates_.first();
    } else {
        start_date = QDateTime::fromString(start_date_str, "yyyy-MM-dd");
        if (!start_date.isValid()) {
            console_->addItem("Invalid start date format. Expected format: yyyy-MM-dd.");
            console_->addItem("");
            return;
        }
    }

    if (end_date_str.toLower() == "last" || end_date_str.toLower() == "end") {
        end_date = dates_.last();
    } else {
        end_date = QDateTime::fromString(end_date_str, "yyyy-MM-dd");
        if (!end_date.isValid()) {
            console_->addItem("Invalid end date format. Expected format: yyyy-MM-dd.");
            console_->addItem("");
            return;
        }
    }

    if (start_date >= end_date) {
        console_->addItem("Start date must be earlier than end date.");
        return;
    }

    double open_sum, high_sum, low_sum, close_sum, volume_sum;
    int count = 0;
    open_sum = high_sum = low_sum = close_sum = volume_sum = 0;

    for (int i = 0; i < dates_.size(); ++i) {
        if (dates_[i] >= start_date && dates_[i] <= end_date) {
            open_sum += open_series_->at(i).y();
            high_sum += high_series_->at(i).y();
            low_sum += low_series_->at(i).y();
            close_sum += close_series_->at(i).y();
            volume_sum += volume_series_->at(i).y();
            ++count;
        }
    }

    if (count == 0) {
        console_->addItem("No data points found in the specified date range.");
        console_->addItem("");
        return;
    }

    console_->addItem(QString("Average values from %1 to %2:").arg(start_date_str, end_date_str));
    console_->addItem(QString("- Open: %1").arg(open_sum / count));
    console_->addItem(QString("- High: %1").arg(high_sum / count));
    console_->addItem(QString("- Low: %1").arg(low_sum / count));
    console_->addItem(QString("- Close: %1").arg(close_sum / count));
    console_->addItem(QString("- Volume: %1").arg(volume_sum / count));
    console_->addItem("");
}

// Load icon from multiple paths
QIcon MainWindow::load_icon(const QString &icon_name)
{
    QStringList paths = {"../../icons/", "./icons/"};
    for (const QString &path : paths) {
        QString full_path = path + icon_name;
        if (QFile::exists(full_path))
            return QIcon(full_path);
    }
    return QIcon();
}

// Change the theme of the graph
void MainWindow::change_theme(QChart::ChartTheme theme)
{
    QChart *chart = chart_view_->chart();
    chart->setTheme(theme);
}

// Clear the graph
void MainWindow::clear_graph(void)
{
    QChart *chart = chart_view_->chart();

    if (open_series_) {
        chart->removeSeries(open_series_);
        delete open_series_;
        open_series_ = nullptr;
    }
    if (high_series_) {
        chart->removeSeries(high_series_);
        delete high_series_;
        high_series_ = nullptr;
    }
    if (low_series_) {
        chart->removeSeries(low_series_);
        delete low_series_;
        low_series_ = nullptr;
    }
    if (close_series_) {
        chart->removeSeries(close_series_);
        delete close_series_;
        close_series_ = nullptr;
    }
    if (volume_series_) {
        chart->removeSeries(volume_series_);
        delete volume_series_;
        volume_series_ = nullptr;
    }
    if (dotted_line_) {
        chart->removeSeries(dotted_line_);
        delete dotted_line_;
        dotted_line_ = nullptr;
    }

    // Reset axis ranges
    x_axis_->setRange(QDateTime(), QDateTime());
    y_axis_->setRange(0, 0);

    dates_.clear();
}

// Parse CSV file and populate series data
void MainWindow::parse_csv(const QString &file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file for reading.";
        return;
    }

    QVector<double> open_values, high_values, low_values, close_values, volume_values;

    QTextStream in(&file);
    bool first_line = true;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (first_line) {
            first_line = false;
            continue;
        }

        QStringList fields = line.split(',');

        QDateTime date = QDateTime::fromString(fields[0], "yyyy-MM-dd");
        if (!date.isValid())
            date = QDateTime::fromString(fields[0], "yyyy/MM/dd");

        if (date.isValid()) {
            dates_.append(date);
            open_values.append(fields[1].toDouble());
            high_values.append(fields[2].toDouble());
            low_values.append(fields[3].toDouble());
            close_values.append(fields[4].toDouble());
            volume_values.append(fields[5].remove(',').toDouble());
        }
    }

    file.close();

    open_series_ = new QLineSeries();
    open_series_->setName("Open");
    high_series_ = new QLineSeries();
    high_series_->setName("High");
    low_series_ = new QLineSeries();
    low_series_->setName("Low");
    close_series_ = new QLineSeries();
    close_series_->setName("Close");
    volume_series_ = new QLineSeries();
    volume_series_->setName("Volume");

    graph_line(open_series_, open_values);
    graph_line(high_series_, high_values);
    graph_line(low_series_, low_values);
    graph_line(close_series_, close_values);
    graph_line(volume_series_, volume_values);

    open_series_->setVisible(open_series_visible_);
    high_series_->setVisible(high_series_visible_);
    low_series_->setVisible(low_series_visible_);
    close_series_->setVisible(close_series_visible_);
    volume_series_->setVisible(volume_series_visible_);
}

// Make a prediction
void MainWindow::make_prediction(int months)
{
    if (current_file_path_.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No file is currently loaded.");
        console_->addItem("No file is currently loaded.");
        console_->addItem("");
        return;
    }

    if (months <= 0 || months > 12) {
        QMessageBox::warning(this, "Warning", "Invalid number. Please enter a value between 1 and 12.");
        return;
    }

    months_input_->clear();

    QString script_path;
    QStringList possible_paths = {"../../predict_stock.py", "./predict_stock.py"};
    for (const QString &path : possible_paths) {
        if (QFile::exists(path)) {
            script_path = path;
            break;
        }
    }

    if (script_path.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Prediction script not found.");
        console_->addItem("Prediction script not found.");
        console_->addItem("");
        return;
    }

    QProcess process;
    QStringList arguments;
    arguments << current_file_path_ << QString::number(months);
    process.start("python3", QStringList() << script_path << arguments);
    process.waitForFinished(-1);

    QString stdout = process.readAllStandardOutput();
    QString stderr = process.readAllStandardError();

    qDebug() << "Python script output:" << stdout;
    if (!stderr.isEmpty())
        qDebug() << "Python script error:" << stderr;

    console_->addItem("Prediction made for " + QString::number(months) + " month(s).");
    console_->addItem("");

    // Clear the current graph
    clear_graph();

    // Load and display the predictions
    QString predictions_path = "/tmp/predictions.csv";
    if (QFile::exists(predictions_path)) {
        parse_csv(predictions_path);
        // Draw the dotted line at the end of the original data
        if (!source_last_entry_.isNull()) {
            draw_dotted_line(source_last_entry_);
        }
    } else {
        QMessageBox::warning(this, "Warning", "Predictions file not found");
        console_->addItem("Prediction file not found.");
        console_->addItem("");
    }
}

// Draw a dotted line indicating the start of the prediction
void MainWindow::draw_dotted_line(QDateTime date)
{
    if (dotted_line_) {
        chart_view_->chart()->removeSeries(dotted_line_);
        delete dotted_line_;
        dotted_line_ = nullptr;
    }

    dotted_line_ = new QLineSeries();
    dotted_line_->setName("Prediction Start");
    dotted_line_->append(date.toMSecsSinceEpoch(), y_axis_->min());
    dotted_line_->append(date.toMSecsSinceEpoch(), y_axis_->max());

    QPen pen(Qt::DashLine);
    pen.setColor(Qt::gray);
    pen.setWidth(2);
    dotted_line_->setPen(pen);

    chart_view_->chart()->addSeries(dotted_line_);
    dotted_line_->attachAxis(x_axis_);
    dotted_line_->attachAxis(y_axis_);

    // Update the axis ranges to ensure the dotted line is visible
    x_axis_->setRange(x_axis_->min(), x_axis_->max());
    y_axis_->setRange(y_axis_->min(), y_axis_->max());
}

// Exit the application and remove temporary files
void MainWindow::exit(void)
{
    QFile::remove("/tmp/predictions.csv");
    QFile::remove("/tmp/stock-price-copy.csv");
    QApplication::quit();
}
