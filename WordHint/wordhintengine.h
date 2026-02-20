#ifndef WORDHINTENGINE_H
#define WORDHINTENGINE_H

#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <climits>
#include <algorithm>

using namespace std;

class WordHintEngine
{
private:
    QString fileName = "D:\\Rahmannur\\IT\\Work\\my projects\\C++\\QT\\WordHint\\WordHint\\master.txt";
    QMap<QString, QList<QString>> wordDictionary;

public:
    WordHintEngine();

    float getDinstanceLProcent(QString s1, QString s2);

    QList<QString> getHintWords(QString word);

    void UploadDictionary();
};

#endif // WORDHINTENGINE_H
