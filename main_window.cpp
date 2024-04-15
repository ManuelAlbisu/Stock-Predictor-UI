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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // set window size
    resize(1280, 768);

    // populate window
    create_actions();
    create_menubar();
    create_buttons();
    create_toolbar();

    create_console();
    create_graph();
}

MainWindow::~MainWindow() {}

void MainWindow::create_actions() {
    xgboost_action_ = new QAction("Enable XGBoost");
    xgboost_action_->setStatusTip("Apply XGBoost algorithm");
    xgboost_action_->setIcon(QIcon("../../icons/xgboost.svg"));
    connect(xgboost_action_, SIGNAL(triggered(bool)), this, SLOT(toggle_xgboost()));

    save_action_ = new QAction("Save");
    save_action_->setStatusTip("Save file as...");
    save_action_->setIcon(QIcon("../..icons/save.svg"));

    exit_action_ = new QAction("Exit");
    exit_action_->setStatusTip("Exit program");
    exit_action_->setIcon(QIcon("../../icons/exit.svg"));
    connect(exit_action_, SIGNAL(triggered(bool)), this, SLOT(exit()));

    open_action_ = new QAction("Open");
    open_action_->setStatusTip("Open CSV file");
    open_action_->setIcon(QIcon("../../icons/open.svg"));
    connect(open_action_, SIGNAL(triggered(bool)), this, SLOT(open_file()));
}

void MainWindow::create_menubar() {
    file_menu_ = menuBar()->addMenu("&File");
    file_menu_->addAction(open_action_);
    file_menu_->addSeparator();
    file_menu_->addAction(exit_action_);

    edit_menu_ = menuBar()->addMenu("&Edit");
    view_menu_ = menuBar()->addMenu("&View");
    help_menu_ = menuBar()->addMenu("&Help");
}

void MainWindow::create_toolbar() {
    tool_bar_ = addToolBar("Tools");
    tool_bar_->addAction(xgboost_action_);
    tool_bar_->addSeparator();
    tool_bar_->addAction(save_action_);
    tool_bar_->addAction(open_action_);
    tool_bar_->addAction(exit_action_);
}

void MainWindow::create_console() {
    console_ = new QListWidget();
    input_ = new QLineEdit();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(console_);
    layout->addWidget(input_);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    create_dock(widget, "Console", Qt::RightDockWidgetArea);
    console_->addItem("Type 'list' for available commands.");

    connect(input_, SIGNAL(returnPressed()), this, SLOT(console_input()));
}

void MainWindow::console_input() {
    QString command = input_->text();
    console_->addItem(command);
    execute_command(command);
    input_->clear();
}

void MainWindow::execute_command(const QString &command) {
    static QRegularExpression re("\\s+");
    QStringList list = command.split(re);

    if(command.toLower() == "clear")
        console_->clear();
    else if(command.toLower() == "quit" | command.toLower() == "exit")
        exit();
    else
        statusBar()->showMessage(("Command '" + command + "' is unknown."), 5000);
}

void MainWindow::create_graph() {
    // creates the graph and graph view
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart_view_ = new QChartView(chart);
    chart_view_->setRenderHint(QPainter::Antialiasing);

    // graph decorations
    chart->setTitle("Stock Predictor");
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setAnimationDuration(3000);
    chart->setAnimationEasingCurve(QEasingCurve::Linear);

    // create graph axis
    x_axis_ = new QValueAxis();
    x_axis_->setTitleText("Time (days)");
    x_axis_->setGridLineVisible(false);
    chart_view_->chart()->addAxis(x_axis_, Qt::AlignBottom);

    y_axis_ = new QValueAxis();
    y_axis_->setTitleText("Price (USD)");
    y_axis_->setGridLineVisible(false);
    chart_view_->chart()->addAxis(y_axis_, Qt::AlignLeft);

    setCentralWidget(chart_view_);
}

void MainWindow::graph_line(const QVector<int> &dates, const QVector<double> &prices) {
    if(dates.size() != prices.size()) {
        qDebug() << "Error: dates and prices vectors are not the same size.";
        return;
    }

    QVector<QPair<int, double>> date_price_pair;
    for(int i = 0; i < dates.size(); ++i) {
        date_price_pair.append(qMakePair(dates[i], prices[i]));
    }

    std::sort(date_price_pair.begin(), date_price_pair.end(),
              [](const QPair<int, double> &a, const QPair<int, double> &b) {
        return a.first < b.first;
    });

    QLineSeries *series = new QLineSeries();
    for(const auto &pair : date_price_pair) {
        qDebug() << "Appending data points: " << pair.first << ", " << pair.second;
        series->append(pair.first, pair.second);
    }

    chart_view_->chart()->addSeries(series);
    series->attachAxis(x_axis_);
    series->attachAxis(y_axis_);

    if(!date_price_pair.isEmpty()) {
        x_axis_->setRange(date_price_pair.first().first,
                          date_price_pair.last().first);
        y_axis_->setRange(*std::min_element(prices.begin(), prices.end()),
                          *std::max_element(prices.begin(), prices.end()));
    }

    chart_view_->chart()->update();
}

void MainWindow::toggle_xgboost() {
    static bool enabled = false;
    enabled = !enabled;
    if(enabled) {
        xgboost_button_->setText("XGBoost Enabled");
        xgboost_button_->setStyleSheet("background-color: green; color: white;");
    } else {
        xgboost_button_->setText("Enable XGBoost");
        xgboost_button_->setStyleSheet("background-color: gray; color: white;");
    }
}

void MainWindow::create_buttons() {
    xgboost_button_ = new QPushButton("Enable XGBoost");
    xgboost_button_->setGeometry(QRect(QPoint(100, 500), QSize(200, 100)));
    xgboost_button_->setStyleSheet("background-color: gray; color: white;");
    connect(xgboost_button_, SIGNAL(clicked(bool)), this, SLOT(toggle_xgboost()));

    save_button_ = new QPushButton("Save");
    save_button_->setGeometry(QRect(QPoint(100, 500), QSize(200, 100)));
    save_button_->setStyleSheet("background-color: gray; color: white;");

    open_button_ = new QPushButton("Open");
    open_button_->setGeometry(QRect(QPoint(100, 500), QSize(200, 100)));
    open_button_->setStyleSheet("background-color: gray; color: white;");
    connect(open_button_, SIGNAL(clicked(bool)), this, SLOT(open_file()));

    exit_button_ = new QPushButton("Exit");
    exit_button_->setGeometry(QRect(QPoint(100, 500), QSize(200, 100)));
    exit_button_->setStyleSheet("background-color: red; color: white");
    connect(exit_button_, SIGNAL(clicked(bool)), this, SLOT(exit()));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(xgboost_button_);
    layout->addWidget(save_button_);
    layout->addWidget(open_button_);
    layout->addWidget(exit_button_);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    create_dock(widget, "Controls", Qt::BottomDockWidgetArea);
}

void MainWindow::create_dock(QWidget *widget, const QString &name, Qt::DockWidgetArea area) {
    QDockWidget *dock = new QDockWidget(name, this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea
                          | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(widget);
    addDockWidget(area, dock);
    view_menu_->addAction(dock->toggleViewAction());
}

void MainWindow::open_file() {
    QString file_name = QFileDialog::getOpenFileName(this, "Open file",
                                                     "", "CSV Files (*.csv)");
    if(!file_name.isEmpty())
        read_file(file_name);
    else {
        qDebug() << "No file given.\n";
        return;
    }
}

void MainWindow::read_file(const QString &file_name) {
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file.\n";
        QMessageBox::critical(this, "Error", "Failed to open the file.");
        return;
    }

    QTextStream in(&file);
    QString header_line = in.readLine().trimmed();
    QStringList header = header_line.remove('"').split(',');
    int date_index = header.indexOf("date");
    int price_index = header.indexOf("close");

    if(date_index == -1 || price_index == -1) {
        qDebug() << "Required data missing from the selected CSV file.";
        QMessageBox::critical(this, "Error", "Required data missing from the CSV file.");
        file.close();
        return;
    }

    QVector<QDate> dates;
    QVector<double> prices;

    while(!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList data = line.remove('"').split(',');
        if(data.size() > qMax(date_index, price_index)) {
            QDate date = QDate::fromString(data.at(date_index), "yyyy/MM/dd");
            if(date.isValid()) {
                dates.append(date);
                prices.append(data.at(price_index).toDouble());
            } else
                qDebug() << "Invalid date format, skipping: " << line;
        } else
            qDebug() << "Malformed line, skipping: " << line;
    }

    file.close();

    // Now process dates to convert to days from the earliest date
    if(dates.isEmpty() || prices.isEmpty() || dates.size() != prices.size()) {
        QMessageBox::information(this, "Info", "No valid data to plot or data mismatch.");
        return;
    }

    QDate ref_date = *std::min_element(dates.begin(), dates.end());
    QVector<int> daysSinceRef;
    for(QDate date : dates)
        daysSinceRef.append(ref_date.daysTo(date));

    if(!daysSinceRef.isEmpty() && daysSinceRef.size() == prices.size())
        graph_line(daysSinceRef, prices);
    else
        QMessageBox::information(this, "Info", "Data processing error.");
}

void MainWindow::exit() { QApplication::quit(); }
