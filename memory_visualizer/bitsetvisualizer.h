#ifndef BITSETVISUALIZER_H
#define BITSETVISUALIZER_H

#include <QWidget>
#include <string>
#include <list>
#include "ui_bitsetvisualizer.h"

struct BitsetMemInfo
{
	std::string regionName_;
	void* memoryBase;
	void* bitsetBase;
	size_t totalBits;
	size_t chunkSize;
	size_t totalMemory;
};

class BitsetVisualizer : public QWidget
{
	Q_OBJECT

public:
	BitsetVisualizer(QWidget *parent = 0);
	~BitsetVisualizer();

	enum bitFlagOption { setIsFree, setIsUsed };
	void AddBitset( BitsetMemInfo bitsetInfo );
private:
	Ui::BitsetVisualizer ui;
	std::list<BitsetMemInfo> bitsets_;
};

#endif // BITSETVISUALIZER_H
