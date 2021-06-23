#include "simp4highlighter.h"

#include <QSyntaxStyle>
#include <QLanguage>

// Qt
#include <QFile>


Simp4Highlighter::Simp4Highlighter(QTextDocument* document) :
    QStyleSyntaxHighlighter(document),
    m_highlightRules     ()
{
    Q_INIT_RESOURCE(qcodeeditor_resources);
    QFile fl(":/languages/simp4.xml");

    if (!fl.open(QIODevice::ReadOnly))
    {
        return;
    }

    QLanguage language(&fl);

    if (!language.isLoaded())
    {
        return;
    }

    auto keys = language.keys();
    for (auto&& key : keys)
    {
        auto names = language.names(key);
        for (auto&& name : names)
        {
            m_highlightRules.append({
                QRegularExpression(QString(R"(\b%1\b)").arg(name)),
                key
            });
        }
    }

    // Numbers
    m_highlightRules.append({
        QRegularExpression(R"(\$[0-9a-fA-F]+)"),
        "Number"
    });
    m_highlightRules.append({
        QRegularExpression(R"(\b[0-1]+b)"),
        "Number"
    });
    m_highlightRules.append({
        QRegularExpression(R"(\b[0-9]+)"),
        "Number"
    });

    // Strings
    m_highlightRules.append({
        QRegularExpression(R"("[^\n"]*")"),
        "String"
    });

    // Define
    m_highlightRules.append({
        QRegularExpression(R"(#[a-zA-Z_]+)"),
        "Preprocessor"
    });

    // Single line
    m_highlightRules.append({
        QRegularExpression(R"(;[^\n]*)"),
        "Comment"
    });
}

void Simp4Highlighter::highlightBlock(const QString& text)
{
    for (auto& rule : m_highlightRules)
    {
        auto matchIterator = rule.pattern.globalMatch(text);

        while (matchIterator.hasNext())
        {
            auto match = matchIterator.next();

            setFormat(
                match.capturedStart(),
                match.capturedLength(),
                syntaxStyle()->getFormat(rule.formatName)
            );
        }
    }
}
