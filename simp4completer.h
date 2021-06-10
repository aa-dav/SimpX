#ifndef SIMP4COMPLETER_H
#define SIMP4COMPLETER_H

#include <QCompleter> // Required for inheritance

class Simp4Completer : public QCompleter
{
    Q_OBJECT

public:

    explicit Simp4Completer( QObject* parent=nullptr );
};

#endif // SIMP4COMPLETER_H
