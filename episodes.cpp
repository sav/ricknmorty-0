#include "episodes.h"
#include "characters.h"

QString apiUrlEpisodes(int page = 1) {
    return QString(API "/episode?page=%1").arg(page);
}

EpisodeCard::EpisodeCard(const QJsonObject &episodeData, QWidget *parent)
    : QFrame(parent) {
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setLineWidth(2);

    layout = new QVBoxLayout(this);

    titleLabel = new QLabel(QString("<b>%1 - %2</b>")
                                .arg(episodeData["episode"].toString())
                                .arg(episodeData["name"].toString()),
                            this);
    titleLabel->setFont(QFont("sans-serif", 14, QFont::Bold));
    layout->addWidget(titleLabel);

    detailsLabel = new QLabel(
        QString("Air Date: %1").arg(episodeData["air_date"].toString()), this);
    layout->addWidget(detailsLabel);

    expandButton = new QPushButton("Show Characters", this);
    connect(expandButton, &QPushButton::clicked, this,
            &EpisodeCard::toggleExpansion);
    layout->addWidget(expandButton);

    charactersWidget = new QWidget(this);
    charactersLayout = new QVBoxLayout(charactersWidget);
    charactersWidget->setVisible(false);
    layout->addWidget(charactersWidget);

    addCharacters(episodeData["characters"].toArray());
}

void EpisodeCard::toggleExpansion() {
    expanded = !expanded;
    charactersWidget->setVisible(expanded);
    expandButton->setText(expanded ? "Hide Characters" : "Show Characters");
}

void EpisodeCard::addCharacters(const QJsonArray &characterUrls) {
    for (const QJsonValue &value : characterUrls) {
        Characters *characterCard =
            new Characters(value.toString(), charactersWidget);
        charactersLayout->addWidget(characterCard);
    }
}

Episodes::Episodes(int pageSize, int initialEpisodesToFetch, QWidget *parent)
    : QWidget(parent), pageSize(pageSize), currentPageIndex(0) {
    layout = new QVBoxLayout();
    scrollArea = new QScrollArea(this);
    scrollContent = new QWidget(this);
    scrollLayout = new QVBoxLayout(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollContent);
    layout->addWidget(scrollArea);

    fetchBtn = new QPushButton("Fetch more...", this);
    fetchBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(fetchBtn, &QPushButton::clicked, this, &Episodes::fetch);
    layout->addWidget(fetchBtn);

    setLayout(layout);

    client = new HttpClient(this);
    if (initialEpisodesToFetch > 0) {
        fetch();
    }
}

void Episodes::fetch() {
    auto handler = [=] (QJsonDocument document) {
        this->document = document;
        QJsonObject root = document.object();
        json = root["results"].toArray();
        url = root["info"].toObject()["next"].toString();
	currentPageIndex = 0;
        currentPage++;
        nextPage();
    };
    if (currentPage == 0) {
        client->get(apiUrlEpisodes(1), handler);
    } else if (!url.isEmpty()) {
        client->get(url, handler);
    } else {
        fetchBtn->setEnabled(false);
        fetchBtn->setText("No more episodes...");
    }
}

void Episodes::nextPage() {
    int count = qMin(pageSize, json.size() - currentPageIndex);

    if (currentPage == 1 && initialEpisodesToFetch > 0) {
        count = qMin(initialEpisodesToFetch, count);
    }

    for (int i = 0; i < count; ++i) {
        EpisodeCard *card = new EpisodeCard(json[currentPageIndex + i].toObject(), scrollContent);
        scrollLayout->addWidget(card);
    }
    currentPageIndex += count;

    if (url.isEmpty()) {
        fetchBtn->setEnabled(false);
        fetchBtn->setText("No more episodes...");
    }
}
