#ifndef FILESET_H
#define FILESET_H

#include <QStringList>
#include <QListWidget>
#include "simpleton4asm.h"

class FileSetFile: public Simpleton::File
{
    int line = 0;
    QStringList strs;
public:
    explicit FileSetFile( const QStringList &list ): strs( list ) {};

    // File interface
public:
    std::string get_line() override;;
    bool eof() override;;
};

class FileSetProvider: public Simpleton::FileProvider
{
    QListWidget *list = nullptr;

public:
    explicit FileSetProvider( QListWidget *_list ): list( _list ) {};

    // FileProvider interface
public:
    std::shared_ptr<Simpleton::File> open(const std::string &name) override;;
};

#endif // FILESET_H
