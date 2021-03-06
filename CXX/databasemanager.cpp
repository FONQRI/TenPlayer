#include "databasemanager.h"
#include <QTime>

DatabaseManager::DatabaseManager(const QString &databasePath, QObject *parent)
    : QObject(parent)
{
    m_databasePath = databasePath;

    createDatabase();
    createTables();
}

DatabaseManager::~DatabaseManager() { delete m_query; }

void DatabaseManager::createTables()
{
    m_query = new QSqlQuery(m_database);

    bool x = m_query->exec(
        "CREATE TABLE IF NOT EXISTS music"
        "(title STRING, album STRING, artist STRING, "
        "albumartist STRING, track INTEGER, disc INTEGER, "
        "genre STRING, year INTEGER, path STRING);");

    if (!x) qDebug() << "Couldn't Create Music Table!";

    x = m_query->exec("CREATE TABLE IF NOT EXISTS playlist(name STRING);");

    if (!x) qDebug() << "Couldn't Create Playlist Table!";

    x = m_query->exec(
        "CREATE TABLE IF NOT EXISTS mpjoin(pid INTEGER, mid INTEGER, row "
        "INTEGER);");

    if (!x) qDebug() << "Couldn't Create Join Table!";
}

void DatabaseManager::createDatabase()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(m_databasePath);
    if (!m_database.open()) qDebug() << "Couldn't Open Database!";
}

QSqlQuery *DatabaseManager::query() const { return m_query; }

void DatabaseManager::clearTables()
{
    m_query->exec("DELETE FROM music;");
    m_query->exec("DELETE FROM playlist;");
    m_query->exec("DELETE FROM mpjoin;");
}

void DatabaseManager::import(const QSize &artworkSize, const QStringList &files)
{
    if (files.isEmpty()) return;

    QVariantList title;
    QVariantList album;
    QVariantList artist;
    QVariantList albumartist;
    QVariantList track;
    QVariantList disc;
    QVariantList genre;
    QVariantList year;
    QVariantList path;

    QVector<QString> artworks = QDir(Loader::ArtworkPath)
                                    .entryList(QStringList(), QDir::Files)
                                    .toVector();

    Loader::Status->setTop(files.count() - 1);

    int count = 0;

    for (const QString &file : files)
    {
        TagData T;
        if (!T.parse(file)) continue;

        Loader::Status->setFile(file);
        Loader::Status->setValue(count++);

        QString artworkName = (T.album + " " + T.albumArtist)
                                  .toUtf8()
                                  .toBase64(QByteArray::Base64UrlEncoding);

        if (!artworks.contains(artworkName))
        {
            QImage img;
            T.getArtwork(file, img);

            if (!img.isNull())
            {
                QString coverPath =
                    QString(Loader::ArtworkPath).append(artworkName);

                img = img.scaled(artworkSize, Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation);

                bool x = img.save(coverPath, "PNG");
                if (x) artworks.prepend(artworkName);
            }
        }

        track.append(T.track);
        disc.append(T.disc);
        title.append(T.title);
        album.append(T.album);
        artist.append(T.artist);
        albumartist.append(T.albumArtist);
        genre.append(T.genre);
        year.append(T.year);
        path.append(file);
    }

    m_query->exec("BEGIN TRANSACTION");
    m_query->prepare("INSERT INTO music VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?);");

    m_query->addBindValue(title);
    m_query->addBindValue(album);
    m_query->addBindValue(artist);
    m_query->addBindValue(albumartist);
    m_query->addBindValue(track);
    m_query->addBindValue(disc);
    m_query->addBindValue(genre);
    m_query->addBindValue(year);
    m_query->addBindValue(path);

    m_query->execBatch();
    m_query->exec("COMMIT;");
}
