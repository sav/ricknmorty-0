#include "characters.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextBrowser>
#include <QVBoxLayout>

QString apiUrlCharacter(const QString &characterUrl) {
    return characterUrl;
}

Characters::Characters(const QString &characterUrl, QWidget *parent)
    : QFrame(parent), url(characterUrl) {
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setLineWidth(2);

    layout = new QVBoxLayout(this);

    nameLabel = new QLabel("", this);
    nameLabel->setFont(QFont("sans-serif", 16, QFont::Bold));
    layout->addWidget(nameLabel);

    statusSpeciesLabel = new QLabel("", this);
    layout->addWidget(statusSpeciesLabel);

    genderLabel = new QLabel("", this);
    layout->addWidget(genderLabel);

    originLocationLabel = new QLabel("", this);
    layout->addWidget(originLocationLabel);

    imageLabel = new QLabel("", this);
    imageLabel->setFixedSize(200, 200);
    imageLabel->setScaledContents(true);
    layout->addWidget(imageLabel);

    episodesText = new QTextBrowser(this);
    episodesText->setOpenExternalLinks(true);
    episodesText->setMaximumHeight(200);
    episodesText->setVisible(true);
    layout->addWidget(episodesText);

    client = new HttpClient(this);
    load();
}

void Characters::load() {
    client->get(url, [=](QJsonDocument document) {
        QJsonObject json = document.object();

        nameLabel->setText(json["name"].toString());
        statusSpeciesLabel->setText(QString("%1 - %2").arg(json["status"].toString()).arg(json["species"].toString()));
        genderLabel->setText(json["gender"].toString());
        originLocationLabel->setText(QString("Origin: %1\nLast Known Location: %2").arg(json["origin"].toObject()["name"].toString()).arg(json["location"].toObject()["name"].toString()));

        client->get(json["image"].toString(), [=](QByteArray imageBytes) {
            QPixmap pixmap;
            pixmap.loadFromData(imageBytes);
            imageLabel->setPixmap(pixmap);
        });

        QString episodesHtml = "<b>Episodes:</b><br>";
        QJsonArray episodesArray = json["episode"].toArray();
        for (const QJsonValue &value : episodesArray) {
            episodesHtml += QString("<a href=\"%1\">%1</a><br>").arg(value.toString());
        }
        episodesText->setHtml(episodesHtml);
    });
}
