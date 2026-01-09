/*
 * Rick & Morty Episode Browser
 * Copyright (C) 2026 Savio Sena <savio.sena@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "episodes.h"

#include <QAction>
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextBrowser>
#include <QVBoxLayout>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QUrl>

void addMenuBar(QApplication &app, QMainWindow &win) {
    QMenuBar *menu = new QMenuBar(&win);
    QMenu *file = menu->addMenu("&File");
    QAction *exit = file->addAction("E&xit");
    QObject::connect(exit, &QAction::triggered, &app, &QApplication::quit);
    win.setMenuBar(menu);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("ricknmorty");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("ZRP");

    QCommandLineParser parser;
    parser.setApplicationDescription("Rick & Morty Episode Browser");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption pageSizeOption(
        QStringList() << "p" << "page-size",
        "Number of episodes to load per page (default: 50)", "size", "50");
    parser.addOption(pageSizeOption);
    parser.process(app);

    int pageSize = parser.value(pageSizeOption).toInt();
    Episodes episodes(pageSize);

    QMainWindow win;
    win.setWindowTitle("Rick and Morty Episode Browser");
    win.resize(800, 600);
    win.setCentralWidget(&episodes);
    addMenuBar(app, win);
    win.show();

    return app.exec();
}
