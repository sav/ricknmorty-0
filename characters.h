#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "network.h"

#include <QFrame>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QTextBrowser>
#include <QVBoxLayout>

class Characters : public QFrame {
    Q_OBJECT
  public:
    Characters(const QString &characterUrl, QWidget *parent = nullptr);

  private:
    void load();

    QVBoxLayout *layout;
    QLabel *nameLabel;
    QLabel *statusSpeciesLabel;
    QLabel *genderLabel;
    QLabel *originLocationLabel;
    QLabel *imageLabel;
    QTextBrowser *episodesText;
    QString url;
    HttpClient *client;
};

#endif // CHARACTER_H_
