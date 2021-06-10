#include "fileset.h"

int FileSetProvider::addContent(const QString &name, const QString &content)
{
    int pos = names.indexOf( name );
    if ( pos < 0 )
    {
        names.push_back( name );
        contents.push_back( content );
        return names.size() - 1;
    }
    else
    {
        contents[ pos ] = content;
        return pos;
    }
}

std::shared_ptr<Simpleton::File> FileSetProvider::open(const std::string &name)
{
    int pos = names.indexOf( QString::fromStdString( name ) );
    if ( pos < 0 )
        return nullptr;
    else
        return std::make_shared<FileSetFile>( contents.at( pos ).split( '\n' ) );
}

std::string FileSetFile::get_line()
{
    if ( line < strs.size() )
        return strs[ line++ ].toStdString();
    else
        return std::string();
}

bool FileSetFile::eof()
{
    return line >= strs.size();
}
