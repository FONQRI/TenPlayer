#ifndef STRUCTS_H
#define STRUCTS_H

#include <QObject>

enum Roles
{
	IDRole = Qt::UserRole + 1,
	TrackRole = Qt::UserRole + 2,
	TitleRole = Qt::UserRole + 3,
	AlbumRole = Qt::UserRole + 4,
	ArtistRole = Qt::UserRole + 5,
	AlbumartistRole = Qt::UserRole + 6,
	GenreRole = Qt::UserRole + 7,
	YearRole = Qt::UserRole + 8,
	PathRole = Qt::UserRole + 9,
	ArtworkRole = Qt::UserRole + 10
};

enum Actions
{
	Play = 1,
	AddToQueue = 2,
	AddToPlaylist = 3,
	ShowDetails = 4,
	Remove = 5
};

#endif  // STRUCTS_H
