#include "wordhintengine.h"

WordHintEngine::WordHintEngine() {}

float WordHintEngine::getDinstanceLProcent(QString s1, QString s2){
    int len_1 = s1.length() + 1;
    int len_2 = s2.length() + 1;

    int **matrix = new int*[len_1];

    for(int i = 0; i < len_1; i++){
        matrix[i] = new int[len_2];
        for(int j = 0; j < len_2; j++){
            matrix[i][j] = INT_MAX;

            if(i == 0){
                matrix[0][j] = j;
            }
            else if(j == 0){
                matrix[i][0] = i;
            }
        }
    }

    for(int i = 1; i < len_1; i++){
        for(int j = 1; j < len_2; j++){
            int isSameConst = s1[i-1] == s2[j-1] ? 0 : 1;

            int dist = min({matrix[i-1][j]+1, matrix[i][j-1]+1, matrix[i-1][j-1]+isSameConst});
            matrix[i][j] = dist;
        }
    }

    float D = matrix[len_1-1][len_2-1];
    float L = max(s1.length(), s2.length());
    float result = (1 - (D/L)) * 100;

    for(int i = 0; i < len_1; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    return result;
}

void WordHintEngine::UploadDictionary(){
    this->wordDictionary.clear();
    QFile file(this->fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "[!] Can not open file: " << this->fileName;
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()){
        QString word = in.readLine().trimmed();
        if(!word.isEmpty() && word.length() >= 2){
            QString slicedStr = word.left(2);

            this->wordDictionary[slicedStr.toLower()].append(word.toLower());
        }
    }

    qDebug() << "[*] Updated dictionary";

    file.close();
}

QList<QString> WordHintEngine::getHintWords(QString word)
{
    QList<QString> result;

    if(word.length() < 2){
        qDebug() << "[!] Length word must be bigger or equal than 2";
        return result;
    }
    if(this->wordDictionary.empty()) this->UploadDictionary();

    word = word.toLower();
    QList<QString> allWords = this->wordDictionary[word.left(2)];

    for(int i = 0; i < allWords.length(); i++){
        if(allWords[i].length() > word.length() && allWords[i].startsWith(word)){
            result.append(allWords[i]);
        }
    }

    if(result.length() < 3){
        qDebug() << "[!] Result length is too small, start searching using distance L.";
        for(int i = 0; i < allWords.length(); i++){
            if(!result.contains(allWords[i]) && allWords[i] != word && this->getDinstanceLProcent(word, allWords[i]) >= 65){
                result.append(allWords[i]);
            }
        }
    }

    sort(result.begin(), result.end(), [](const QString &a, QString &b){
        return a.length() < b.length();
    });

    return result;
}
