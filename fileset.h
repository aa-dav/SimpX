#ifndef FILESET_H
#define FILESET_H

#include <QStringList>
#include "simpleton4asm.h"

class FileSetFile: public Simpleton::File
{
    int line = 0;
    QStringList strs;
public:
    FileSetFile( const QStringList &list ): strs( list ) {};

    // File interface
public:
    std::string get_line() override;;
    bool eof() override;;
};

class FileSetProvider: public Simpleton::FileProvider
{
    QStringList names;
    QStringList contents;

public:
    FileSetProvider() {};

    int     size() { return names.size(); }
    QString getName( int i ) { return names.at( i ); }
    void    setName( int i, const QString &val ) { names[ i ] = val; }
    QString getContent( int i ) { return contents.at( i ); }
    void    setContent( int i, const QString &val ) { contents[ i ] = val; }

    int     addContent( const QString &name, const QString &content );
    void    remove( int i )
    {
        names.removeAt( i );
        contents.removeAt( i );
    };

    // FileProvider interface
public:
    std::shared_ptr<Simpleton::File> open(const std::string &name) override;;
};

#endif // FILESET_H
