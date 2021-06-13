#include "fileset.h"

std::shared_ptr<Simpleton::File> FileSetProvider::open(const std::string &name)
{
    for ( int i = 0; i < list->count(); i++ )
    {
        QListWidgetItem *item = list->item( i );
        if ( item->text().toStdString() == name )
        {
            QString s = item->data( Qt::ItemDataRole::UserRole ).toString();
            return std::make_shared<FileSetFile>( s.split( '\n' ) );
        }
    }
    return nullptr; // nothing was found
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
