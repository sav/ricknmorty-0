#ifndef EPISODES_H_
#define EPISODES_H_

#include "network.h"

#include <QLabel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVector>
#include <QString>

class EpisodeCard : public QFrame {
    Q_OBJECT

  public:
    EpisodeCard(const QJsonObject &episodeData, QWidget *parent = nullptr);

  private:
    QVBoxLayout *layout;
    QLabel *titleLabel;
    QLabel *detailsLabel;
    QPushButton *expandButton;
    QWidget *charactersWidget;
    QVBoxLayout *charactersLayout;
    bool expanded = false;

    void toggleExpansion();
    void addCharacters(const QJsonArray &characterUrls);
};

class Episodes : public QWidget {
    Q_OBJECT

  public:
    Episodes(int pageSize = 0, int initialEpisodesToFetch = 0, QWidget *parent = nullptr);

  private slots:
    void fetch();

  private:
    void nextPage();

    QVBoxLayout *layout;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *scrollLayout;
    QPushButton *fetchBtn;
    HttpClient *client;
    QJsonDocument document;
    QJsonArray json;
    QString url;
    int currentPage = 0;
    int pageSize;
    int currentPageIndex;
    int initialEpisodesToFetch;
};

#endif // EPISODES_H_
