#ifndef SONGLOADER_H
#define SONGLOADER_H

#include "loader.h"

class SongLoader : public Loader
{
	Q_OBJECT
public:
	explicit SongLoader(QObject *parent = nullptr);

	// Loader interface
public:
	void load();

	virtual void clicked(const int &index) {}

	virtual void actionTriggered(const int &type, const int &index,
								 const QVariant &extra = QVariant())
	{
	}
};

#endif  // SONGLOADER_H
